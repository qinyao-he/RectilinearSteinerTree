//
// Created by ZhuangTianYi on 15/7/10.
//

#include "ZRST.h"
#include "RST.h"
#include "Overlap.h"

#include <ctime>
#include <cassert>

using std::abs;
using std::copy;
using std::lower_bound;
using std::max;
using std::min;
using std::sort;
using std::unique;
using std::vector;


//SubProcedure for solve(). It would examine all the layouts for the sons of a
//node and get the best one.layout::subAns and layout::bestLay should be
//filled after this procedure.
void ZRST::dfs(int root, int father, int stat, layout &lay,
               const vector<Line_Z> &lines, vector<vector<layout> > &subProb,
               const vector<int>& head, int *stack) {
    using Overlap::overlap;
    //current node is a leaf
    if (head[root] == head[root + 1]) {
        lay.subAns = 0;
        return;
    }
    int id = stat - head[root], son = lines[stat].end();
    //A node has no more than 6 childs
    assert(id < 6);
    if (stat == head[root + 1]) {
        int ans = 0;
        vector<Line_Z> overLap;
        for (int i = 0; i < id; i++) {
            ans += subProb[son = lines[head[root] + i].end()][stack[i]].subAns;
            overLap.push_back(Line_Z(root, son, subProb[son][stack[i]].midPoint));
        }
        overLap.push_back(Line_Z(father, root, lay.midPoint));
        //current answer exclude the edge upon the node itself
        ans += overlap(points(), overLap) - point(root).distance(point(father));
        if (ans < lay.subAns) {
            lay.subAns = ans;
            copy(stack, stack + id, lay.bestLay);
        }
    } else {
        for (size_t i = 0; (stack[id] = i) < subProb[son].size(); i++) {
            dfs(root, father, stat + 1, lay, lines, subProb, head, stack);
        }
    }
}

void ZRST::solve() {
    mst.mst();
    //reserve before mass data pushing back
    m_lines.reserve(mst.lines().size());
    for (const auto& line : mst.lines()) {
        m_lines.push_back(Line_Z(line.start(), line.end(), Point()));
    }
    //lines is a sorted line vector, while smst.m_lines is in parent-child order
    sort(m_lines.begin(), m_lines.end(), [](const Line_Z &a, const Line_Z &b) {
        return a.start() < b.start();
    });
    //make the positive direction table
    vector<int> head(points().size() + 1);
    for (size_t i = 0, j = 0; i <= points().size() && j <= lines().size();) {
        if (j == lines().size() || line(j).start() != i - 1)
            head[i++] = j;
        else
            j++;
    }
    //Discretization
    vector<int> parent(points().size());
    vector<int> x_grids(points().size()), y_grids(points().size());
    for (size_t i = 0; i < m_lines.size(); i++) {
        parent[m_lines[i].end()] = m_lines[i].start();
    }
    parent[0] = -1;
    for (size_t i = 0; i < points().size(); i++) {
        x_grids[i] = point(i).x;
        y_grids[i] = point(i).y;
    }
    sort(x_grids.begin(), x_grids.end());
    sort(y_grids.begin(), y_grids.end());
    auto x_f = unique(x_grids.begin(), x_grids.end());
    auto y_f = unique(y_grids.begin(), y_grids.end());
    x_grids.erase(x_f, x_grids.end());
    y_grids.erase(y_f, y_grids.end());
    vector<vector<layout> > subProb(vector<vector<layout> >(points().size(),
                                                            vector<layout>()));
    //Enumerate all layouts
    for (vector<Line_Z>::iterator it = m_lines.begin();
         it != m_lines.end(); ++it) {
        int minx = min(point(it->start()).x, point(it->end()).x),
                maxx = max(point(it->start()).x, point(it->end()).x),
                miny = min(point(it->start()).y, point(it->end()).y),
                maxy = max(point(it->start()).y, point(it->end()).y);
        int minxid = lower_bound(x_grids.begin(), x_grids.end(), minx) - x_grids.begin(),
                maxxid = lower_bound(x_grids.begin(), x_grids.end(), maxx) - x_grids.begin(),
                minyid = lower_bound(y_grids.begin(), y_grids.end(), miny) - y_grids.begin(),
                maxyid = lower_bound(y_grids.begin(), y_grids.end(), maxy) - y_grids.begin();
        for (int i = minxid; i <= maxxid; i++) {
            subProb[it->end()].push_back(Point(x_grids[i], point(it->start()).y));
        }
        for (int i = minyid; i <= maxyid; i++) {
            subProb[it->end()].push_back(Point(point(it->start()).x, y_grids[i]));
        }
    }
    //Caculate from leaves to root
    int stack[6];
    for (vector<Line>::const_reverse_iterator it = mst.lines().rbegin();
         it != mst.lines().rend(); ++it) {
        for (vector<layout>::iterator lit = subProb[it->end()].begin();
             lit != subProb[it->end()].end(); ++lit) {
            dfs(it->end(), parent[it->end()], head[it->end()], *lit, lines(),
                subProb, head, stack);
        }
    }
    subProb[0].push_back(point(0));
    dfs(0, 0, head[0], subProb[0][0], lines(), subProb, head, stack);
    getAns(0, subProb[0][0], subProb, head);
}

//SubProcedure for solve(). It would examine the subProb and look for
//best solutions for each node.
void ZRST::getAns(int root, const layout &lay,
                  const vector<vector<layout> > subProb,
                  const vector<int>& head) {
    for (int i = head[root]; i < head[root + 1]; i++) {
        m_lines[i].mid_point =
                subProb[line(i).end()][lay.bestLay[i - head[root]]].midPoint;
        getAns(line(i).end(), subProb[line(i).end()][lay.bestLay[i - head[root]]],
               subProb, head);
    }
}

void ZRST::setPointsFromRST(RST *rst) {
    m_lines.clear();
    mst.set_rst(rst);
}

void ZRST::getResult(RST *rst) {
    rst->v_seg.clear();
    for (size_t i = 0; i < m_lines.size(); i++) {
        Point A(mst.points()[m_lines[i].start()].x, mst.points()[m_lines[i].start()].y);
        Point B(mst.points()[m_lines[i].end()].x, mst.points()[m_lines[i].end()].y);
        Point C(m_lines[i].mid_point.x, m_lines[i].mid_point.y);
        Point D; // A - C -(default:vertical when A==C) D - B
        if (std::abs(A.y - C.y) < 0.1)
            D = Point(C.x, B.y);
        else
            D = Point(B.x, C.y);
        rst->v_seg.push_back(Segment(A, C));
        rst->v_seg.push_back(Segment(C, D));
        rst->v_seg.push_back(Segment(D, B));
    }
}