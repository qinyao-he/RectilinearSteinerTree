/// Project name: New Algorithms for the Rectilinear Steiner Tree Problem
/// Sub-project: LMST  
//  LMST.cpp
//  
//
//  Created by Tony Soong on 2013/6/9
//  Modified by Tony Soong on 2013/6/9
//  Copyright (c) Tony Soong(Jiaming Song) All rights reserved.
//
//	This file implements the functions of class LMST, namely the sections.
//  TODO(Tony): I need more tests, and I need to make the code more beautiful.
//

#include "LMST.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <set>

#include "RST.h"

const int INF = 0x7FFFFFFF;


void LMST::init() {
    std::vector<int> ivec;
    smst_.calculate();
    points_ = smst_.points();
    lines_ = smst_.lines();
    assert(points_.size() > 1);
    has_set_.resize(points_.size(), false);
    tree_.resize(points_.size(), ivec);
    father_.resize(points_.size(), -1);
    psi_l_.resize(points_.size(), -1);
    psi_u_.resize(points_.size(), -1);
    choice_l_.resize(points_.size(), -1);
    choice_u_.resize(points_.size(), -1);
    FindRoot();
    OrganizeTree(root_);
    assert(tree_[root_].size() == 1);
    DesperseData();
    FindPsi();
    Result();
    OutputResultToVector();
}

LMST::LMST() : smst_(), psi_result_(INF) { }


int LMST::Result() {
    if (psi_result_ != INF) return psi_result_;
    int result = 0;
    for (unsigned i = 0; i < lines_.size(); i++) {
        result += dist(points_[lines_[i].start()], points_[lines_[i].end()]);
    }
    int diff = std::max(psi_l_[tree_[root_][0]], psi_u_[tree_[root_][0]]);
    psi_result_ = result - diff;
    return psi_result_;
}

void LMST::Output(const char *filename) {
    OutputResultToFile(filename);
}

void LMST::FindPsi() {
    FindPsiL(tree_[root_][0]);
    FindPsiU(tree_[root_][0]);
}

void LMST::FindRoot() {
    std::vector<int> degree_(points_.size(), 0);
    typedef std::vector<Line>::iterator li_iter;
    for (li_iter it = lines_.begin(); it != lines_.end(); ++it) {
        degree_[(it->start())]++;
        degree_[(it->end())]++;
    }
    typedef std::vector<int>::iterator it_iter;
    for (it_iter it = degree_.begin(); it != degree_.end(); ++it) {
        if ((*it) == 1) {
            root_ = (it - degree_.begin());
            return;
        }
    }
    assert(false);
}

void LMST::OrganizeTree(int father) {
    typedef std::vector<Line>::iterator li_iter;
    int son;
    has_set_[father] = true;
    for (li_iter it = lines_.begin(); it != lines_.end(); ++it) {
        if (it->start() != father && it->end() != father) continue;
        if (it->start() == father) son = it->end();
        else son = it->start();
        if (has_set_[son]) continue;
        tree_[father].push_back(son);
        father_[son] = father;
        OrganizeTree(son);
    }
    return;
}

void LMST::DesperseData() {
    typedef std::vector<Point>::iterator pt_iter;
    typedef std::set<int>::iterator st_iter;
    std::set<int> x_coord, y_coord;
    for (pt_iter it = points_.begin(); it != points_.end(); ++it) {
        int x = it->x, y = it->y;
        if (x_coord.find(x) == x_coord.end()) x_coord.insert(x);
        if (y_coord.find(y) == y_coord.end()) y_coord.insert(y);
    }
    int xlen = x_coord.size(), ylen = y_coord.size();
    for (st_iter it = x_coord.begin(); it != x_coord.end(); ++it) {
        x_coord_.push_back((*it));
    }
    for (st_iter it = y_coord.begin(); it != y_coord.end(); ++it) {
        y_coord_.push_back((*it));
    }
    for (pt_iter it = points_.begin(); it != points_.end(); ++it) {
        int x = it->x, y = it->y;
        int disp_x = find(x_coord_.begin(), x_coord_.end(), x) - x_coord_.begin();
        int disp_y = find(y_coord_.begin(), y_coord_.end(), y) - y_coord_.begin();
        disp_points_.push_back(Point(disp_x, disp_y));
    }
    for (unsigned i = 0; i < x_coord_.size(); i++) {
        for (unsigned j = 0; j < y_coord_.size(); j++) {
            hori_line_.insert(std::pair<Point, int>(Point(i, j), 0));
            verti_line_.insert(std::pair<Point, int>(Point(i, j), 0));
        }
    }
    return;
}

void LMST::PaintHori(int u, int v, int y, int color, int &value) {
    assert(u <= v && (color * color == 1));
    for (int i = u; i < v; i++) {
        int hori = x_coord_[i + 1] - x_coord_[i];
        assert(hori_line_.find(Point(i, y))->second + color >= 0);
        if (color > 0 && hori_line_.find(Point(i, y))->second > 0)
            value += hori;
        hori_line_.find(Point(i, y))->second += color;
        if (color < 0 && hori_line_.find(Point(i, y))->second > 0)
            value -= hori;
    }
}

void LMST::PaintVerti(int u, int v, int x, int color, int &value) {
    assert(u <= v && (color * color == 1));
    for (int i = u; i < v; i++) {
        int verti = y_coord_[i + 1] - y_coord_[i];
        assert(verti_line_.find(Point(x, i))->second + color >= 0);
        if (color > 0 && verti_line_.find(Point(x, i))->second > 0)
            value += verti;
        verti_line_.find(Point(x, i))->second += color;
        if (color < 0 && verti_line_.find(Point(x, i))->second > 0)
            value -= verti;
    }
}

void LMST::Paint(Point start, Point finish, bool direction, int color, int &value) {
    int x1 = start.x, y1 = start.y;
    int x2 = finish.x, y2 = finish.y;
    int x3, y3;
    if (!direction) {
        x3 = x1, y3 = y2;
        int u = std::min(y1, y3), v = std::max(y1, y3);
        // draw from (x1, u) to (x1, v)
        PaintVerti(u, v, x1, color, value);
        u = std::min(x2, x3), v = std::max(x2, x3);
        PaintHori(u, v, y2, color, value);
    }
    else {
        x3 = x2, y3 = y1;
        int u = std::min(x1, x3), v = std::max(x1, x3);
        PaintHori(u, v, y1, color, value);
        u = std::min(y2, y3), v = std::max(y2, y3);
        PaintVerti(u, v, x2, color, value);
    }
    return;
}

void LMST::Draw(int parent, std::vector<int> &kids,
                int num,
                int &value, int &result, int choice, int &decision) {
    if (num == kids.size()) {
        if (result < value) {
            result = value;
            decision = choice;
        }
        return;
    }
    // if the shape is L
    Paint(disp_points_[parent], disp_points_[kids[num]], false, 1, value);
    value += psi_l_[kids[num]];
    Draw(parent, kids, num + 1, value, result, choice, decision);
    value -= psi_l_[kids[num]];
    Paint(disp_points_[parent], disp_points_[kids[num]], false, -1, value);
    // if the shape is U
    Paint(disp_points_[parent], disp_points_[kids[num]], true, 1, value);
    value += psi_u_[kids[num]];
    Draw(parent, kids, num + 1, value, result, choice + (1 << num), decision);
    value -= psi_u_[kids[num]];
    Paint(disp_points_[parent], disp_points_[kids[num]], true, -1, value);
    return;
}

int LMST::FindPsiL(int label) {
    if (psi_l_[label] != -1)
        return psi_l_[label];
    if (tree_[label].empty()) {
        psi_l_[label] = 0;
        return 0;
    }
    std::vector<int> &kids = tree_[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        FindPsiL(kids[i]);
        FindPsiU(kids[i]);
    }
    int value = 0, choice = 0;
    Paint(disp_points_[father_[label]], disp_points_[label], false, 1, value);
    Draw(label, kids, 0, value, psi_l_[label], choice, choice_l_[label]);
    Paint(disp_points_[father_[label]], disp_points_[label], false, -1, value);
    return psi_l_[label];
}

int LMST::FindPsiU(int label) {
    if (psi_u_[label] != -1)
        return psi_u_[label];
    if (tree_[label].empty()) {
        psi_u_[label] = 0;
        return 0;
    }
    std::vector<int> &kids = tree_[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        FindPsiL(kids[i]);
        FindPsiU(kids[i]);
    }
    int value = 0, choice = 0;
    Paint(disp_points_[father_[label]], disp_points_[label], true, 1, value);
    Draw(label, kids, 0, value, psi_u_[label], choice, choice_u_[label]);
    Paint(disp_points_[father_[label]], disp_points_[label], true, -1, value);
    return psi_u_[label];
}

void LMST::OutputResultToVector() {
    if (result_.size() != 0) return;
    int child_ = tree_[root_][0];
    bool choice = psi_l_[child_] < psi_u_[child_];
    OutputResultToVectorOfLabel(child_, choice);
}

void LMST::OutputResultToVectorOfLabel(int label, bool choice) {
    result_.push_back(Line_L(father_[label], label, choice));
    std::vector<int> &kids = tree_[label];
    int choice_record_ = choice ? choice_u_[label] : choice_l_[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        bool i_th = ((choice_record_ >> i) & 1) == 1;
        OutputResultToVectorOfLabel(kids[i], i_th);
    }
}

void LMST::OutputResultToFile(const char *filename) {
    assert(result_.size() > 0);
    std::ofstream out(filename);
    out << points_.size() << " " << result_.size() << std::endl;
    for (unsigned i = 0; i < points_.size(); i++) {
        out << points_[i].x << " " << points_[i].y << std::endl;
    }
    for (unsigned i = 0; i < result_.size(); i++) {
        result_[i].print(out);
    }
    return;
}

void LMST::setPointsFromRST(RST *rst) {
    //qDebug("set data for LMST");
    smst_.set_rst(rst);
}

void LMST::getResult(RST *rst) {
    //qDebug("LMST end");
    rst->v_seg.clear();
    for (int i = 0; i < result_.size(); i++) {
        Point A(points_[result_[i].start()].x, points_[result_[i].start()].y);
        Point B(points_[result_[i].end()].x, points_[result_[i].end()].y);
        Point C;
        if (result_[i].direction()) // horizontal
            C = Point(B.x, A.y);
        else
            C = Point(A.x, B.y);
        rst->v_seg.push_back(Segment(A, C));
        rst->v_seg.push_back(Segment(C, B));
    }
    //qDebug("LMST with %d seg", rst->v_seg.size());
}


