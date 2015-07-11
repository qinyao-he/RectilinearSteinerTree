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


void LMST::solve() {
    std::vector<int> ivec;
    mst.mst();
    m_points = mst.points();
    m_lines = mst.lines();
    assert(m_points.size() > 1);
    has_set.resize(m_points.size(), false);
    tree.resize(m_points.size(), ivec);
    parent.resize(m_points.size(), -1);
    psi_l.resize(m_points.size(), -1);
    psi_u.resize(m_points.size(), -1);
    choice_l.resize(m_points.size(), -1);
    choice_u.resize(m_points.size(), -1);
    root = findRoot();
    organizeTree(root);
    assert(tree[root].size() == 1);
    desperseData();
    FindPsi();
    get_result();
    outputResultToVector();
}

LMST::LMST() : mst(), psi_result(INF) { }


int LMST::get_result() {
    if (psi_result != INF) return psi_result;
    int result = 0;
    for (unsigned i = 0; i < m_lines.size(); i++) {
        result += dist(m_points[m_lines[i].start()], m_points[m_lines[i].end()]);
    }
    int diff = std::max(psi_l[tree[root][0]], psi_u[tree[root][0]]);
    psi_result = result - diff;
    return psi_result;
}

void LMST::FindPsi() {
    findPsiL(tree[root][0]);
    findPsiU(tree[root][0]);
}

int LMST::findRoot() {
    std::vector<int> degree_(m_points.size(), 0);
    typedef std::vector<Line>::iterator li_iter;
    for (li_iter it = m_lines.begin(); it != m_lines.end(); ++it) {
        degree_[(it->start())]++;
        degree_[(it->end())]++;
    }
    typedef std::vector<int>::iterator it_iter;
    for (it_iter it = degree_.begin(); it != degree_.end(); ++it) {
        if ((*it) == 1) {
            return (it - degree_.begin());
        }
    }
    assert(false);
}

void LMST::organizeTree(int father) {
    typedef std::vector<Line>::iterator li_iter;
    int son;
    has_set[father] = true;
    for (li_iter it = m_lines.begin(); it != m_lines.end(); ++it) {
        if (it->start() != father && it->end() != father) continue;
        if (it->start() == father) son = it->end();
        else son = it->start();
        if (has_set[son]) continue;
        tree[father].push_back(son);
        parent[son] = father;
        organizeTree(son);
    }
    return;
}

void LMST::desperseData() {
    typedef std::vector<Point>::iterator pt_iter;
    typedef std::set<int>::iterator st_iter;
    std::set<int> x_coord_set, y_coord_set;
    for (pt_iter it = m_points.begin(); it != m_points.end(); ++it) {
        int x = it->x, y = it->y;
        if (x_coord_set.find(x) == x_coord_set.end()) x_coord_set.insert(x);
        if (y_coord_set.find(y) == y_coord_set.end()) y_coord_set.insert(y);
    }
    for (st_iter it = x_coord_set.begin(); it != x_coord_set.end(); ++it) {
        x_coord.push_back((*it));
    }
    for (st_iter it = y_coord_set.begin(); it != y_coord_set.end(); ++it) {
        y_coord.push_back((*it));
    }
    for (pt_iter it = m_points.begin(); it != m_points.end(); ++it) {
        int x = it->x, y = it->y;
        int disp_x = find(x_coord.begin(), x_coord.end(), x) - x_coord.begin();
        int disp_y = find(y_coord.begin(), y_coord.end(), y) - y_coord.begin();
        disp_points.push_back(Point(disp_x, disp_y));
    }
    for (unsigned i = 0; i < x_coord_set.size(); i++) {
        for (unsigned j = 0; j < y_coord_set.size(); j++) {
            hori_line.insert(std::pair<Point, int>(Point(i, j), 0));
            verti_line.insert(std::pair<Point, int>(Point(i, j), 0));
        }
    }
    return;
}

void LMST::paintHori(int u, int v, int y, int color, int &value) {
    assert(u <= v && (color * color == 1));
    for (int i = u; i < v; i++) {
        int hori = x_coord[i + 1] - x_coord[i];
        assert(hori_line.find(Point(i, y))->second + color >= 0);
        if (color > 0 && hori_line.find(Point(i, y))->second > 0)
            value += hori;
        hori_line.find(Point(i, y))->second += color;
        if (color < 0 && hori_line.find(Point(i, y))->second > 0)
            value -= hori;
    }
}

void LMST::paintVerti(int u, int v, int x, int color, int &value) {
    assert(u <= v && (color * color == 1));
    for (int i = u; i < v; i++) {
        int verti = y_coord[i + 1] - y_coord[i];
        assert(verti_line.find(Point(x, i))->second + color >= 0);
        if (color > 0 && verti_line.find(Point(x, i))->second > 0)
            value += verti;
        verti_line.find(Point(x, i))->second += color;
        if (color < 0 && verti_line.find(Point(x, i))->second > 0)
            value -= verti;
    }
}

void LMST::paint(Point start, Point finish, bool direction, int color, int &value) {
    int x1 = start.x, y1 = start.y;
    int x2 = finish.x, y2 = finish.y;
    int x3, y3;
    if (!direction) {
        x3 = x1, y3 = y2;
        int u = std::min(y1, y3), v = std::max(y1, y3);
        // draw from (x1, u) to (x1, v)
        paintVerti(u, v, x1, color, value);
        u = std::min(x2, x3), v = std::max(x2, x3);
        paintHori(u, v, y2, color, value);
    }
    else {
        x3 = x2, y3 = y1;
        int u = std::min(x1, x3), v = std::max(x1, x3);
        paintHori(u, v, y1, color, value);
        u = std::min(y2, y3), v = std::max(y2, y3);
        paintVerti(u, v, x2, color, value);
    }
    return;
}

void LMST::draw(int parent, std::vector<int> &kids,
                size_t num,
                int &value, int &result, int choice, int &decision) {
    if (num == kids.size()) {
        if (result < value) {
            result = value;
            decision = choice;
        }
        return;
    }
    // if the shape is L
    paint(disp_points[parent], disp_points[kids[num]], false, 1, value);
    value += psi_l[kids[num]];
    draw(parent, kids, num + 1, value, result, choice, decision);
    value -= psi_l[kids[num]];
    paint(disp_points[parent], disp_points[kids[num]], false, -1, value);
    // if the shape is U
    paint(disp_points[parent], disp_points[kids[num]], true, 1, value);
    value += psi_u[kids[num]];
    draw(parent, kids, num + 1, value, result, choice + (1 << num), decision);
    value -= psi_u[kids[num]];
    paint(disp_points[parent], disp_points[kids[num]], true, -1, value);
    return;
}

int LMST::findPsiL(int label) {
    if (psi_l[label] != -1)
        return psi_l[label];
    if (tree[label].empty()) {
        psi_l[label] = 0;
        return 0;
    }
    std::vector<int> &kids = tree[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        findPsiL(kids[i]);
        findPsiU(kids[i]);
    }
    int value = 0, choice = 0;
    paint(disp_points[parent[label]], disp_points[label], false, 1, value);
    draw(label, kids, 0, value, psi_l[label], choice, choice_l[label]);
    paint(disp_points[parent[label]], disp_points[label], false, -1, value);
    return psi_l[label];
}

int LMST::findPsiU(int label) {
    if (psi_u[label] != -1)
        return psi_u[label];
    if (tree[label].empty()) {
        psi_u[label] = 0;
        return 0;
    }
    std::vector<int> &kids = tree[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        findPsiL(kids[i]);
        findPsiU(kids[i]);
    }
    int value = 0, choice = 0;
    paint(disp_points[parent[label]], disp_points[label], true, 1, value);
    draw(label, kids, 0, value, psi_u[label], choice, choice_u[label]);
    paint(disp_points[parent[label]], disp_points[label], true, -1, value);
    return psi_u[label];
}

void LMST::outputResultToVector() {
    if (m_result.size() != 0) return;
    int child_ = tree[root][0];
    bool choice = psi_l[child_] < psi_u[child_];
    outputResultToVectorOfLabel(child_, choice);
}

void LMST::outputResultToVectorOfLabel(int label, bool choice) {
    m_result.push_back(Line_L(parent[label], label, choice));
    std::vector<int> &kids = tree[label];
    int choice_record_ = choice ? choice_u[label] : choice_l[label];
    for (unsigned i = 0; i < kids.size(); i++) {
        bool i_th = ((choice_record_ >> i) & 1) == 1;
        outputResultToVectorOfLabel(kids[i], i_th);
    }
}

void LMST::setPointsFromRST(RST *rst) {
    //qDebug("set data for LMST");
    mst.set_rst(rst);
}

void LMST::getResult(RST *rst) {
    //qDebug("LMST end");
    rst->v_seg.clear();
    for (size_t i = 0; i < m_result.size(); i++) {
        Point A(m_points[m_result[i].start()].x, m_points[m_result[i].start()].y);
        Point B(m_points[m_result[i].end()].x, m_points[m_result[i].end()].y);
        Point C;
        if (m_result[i].direction()) // horizontal
            C = Point(B.x, A.y);
        else
            C = Point(A.x, B.y);
        rst->v_seg.push_back(Segment(A, C));
        rst->v_seg.push_back(Segment(C, B));
    }
    //qDebug("LMST with %d seg", rst->v_seg.size());
}


