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

//Compare the line by the start node.
inline bool lineCmp(const Line_Z &a, const Line_Z &b) {
    return a.start() < b.start();
}

//SubProcedure for solve(). It would examine all the layouts for the sons of a
//node and get the best one.layout::subAns and layout::bestLay should be
//filled after this procedure.
void ZRST::dfs(int root, int father, int stat, layout &lay,
               const vector<Line_Z> &lines, vector<vector<layout> > &subProb,
               const int *head, int *stack) {
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
    smst.mst();
    //reserve before mass data pushing back
    m_lines.reserve(smst.lines().size());
    for (size_t i = 0; i < smst.lines().size(); i++)
        m_lines.push_back(Line_Z(smst.lines()[i].start(), smst.lines()[i].end(),
                                Point()));
    //lines is a sorted line vector, while smst.m_lines is in parent-child order
    sort(m_lines.begin(), m_lines.end(), lineCmp);
    //make the positive direction table
    int *head = new int[points().size() + 1];
    for (size_t i = 0, j = 0; i <= points().size() && j <= lines().size();) {
        if (j == lines().size() || line(j).start() != i - 1)
            head[i++] = j;
        else
            j++;
    }
    //Discretization
    int *father = new int[points().size()],
            *xgrids = new int[points().size()],
            *ygrids = new int[points().size()],
            xgridsize, ygridsize;
    for (size_t i = 0; i < m_lines.size(); i++) {
        father[m_lines[i].end()] = m_lines[i].start();
    }
    father[0] = -1;
    for (size_t i = 0; i < points().size(); i++) {
        xgrids[i] = point(i).x;
        ygrids[i] = point(i).y;
    }
    sort(xgrids, xgrids + points().size());
    sort(ygrids, ygrids + points().size());
    xgridsize = unique(xgrids, xgrids + points().size()) - xgrids;
    ygridsize = unique(ygrids, ygrids + points().size()) - ygrids;
    vector<vector<layout> > subProb(vector<vector<layout> >(points().size(),
                                                            vector<layout>()));
    //Enumerate all layouts
    for (vector<Line_Z>::iterator it = m_lines.begin();
         it != m_lines.end(); ++it) {
        int minx = min(point(it->start()).x, point(it->end()).x),
                maxx = max(point(it->start()).x, point(it->end()).x),
                miny = min(point(it->start()).y, point(it->end()).y),
                maxy = max(point(it->start()).y, point(it->end()).y);
        int minxid = lower_bound(xgrids, xgrids + xgridsize, minx) - xgrids,
                maxxid = lower_bound(xgrids, xgrids + xgridsize, maxx) - xgrids,
                minyid = lower_bound(ygrids, ygrids + ygridsize, miny) - ygrids,
                maxyid = lower_bound(ygrids, ygrids + ygridsize, maxy) - ygrids;
        for (int i = minxid; i <= maxxid; i++) {
            subProb[it->end()].push_back(Point(xgrids[i], point(it->start()).y));
        }
        for (int i = minyid; i <= maxyid; i++) {
            subProb[it->end()].push_back(Point(point(it->start()).x, ygrids[i]));
        }
    }
    //Caculate from leaves to root
    int stack[6];
    for (vector<Line>::const_reverse_iterator it = smst.lines().rbegin();
         it != smst.lines().rend(); ++it) {
        for (vector<layout>::iterator lit = subProb[it->end()].begin();
             lit != subProb[it->end()].end(); ++lit) {
            dfs(it->end(), father[it->end()], head[it->end()], *lit, lines(),
                subProb, head, stack);
        }
    }
    subProb[0].push_back(point(0));
    dfs(0, 0, head[0], subProb[0][0], lines(), subProb, head, stack);
    getAns(0, subProb[0][0], subProb, head);
    delete[] head;
    delete[] xgrids;
    delete[] ygrids;
    delete[] father;
}

//SubProcedure for solve(). It would examine the subProb and look for
//best solutions for each node.
void ZRST::getAns(int root, const layout &lay,
                  const vector<vector<layout> > subProb,
                  const int *head) {
    for (int i = head[root]; i < head[root + 1]; i++) {
        m_lines[i].mid_point_set() =
                subProb[line(i).end()][lay.bestLay[i - head[root]]].midPoint;
        getAns(line(i).end(), subProb[line(i).end()][lay.bestLay[i - head[root]]],
               subProb, head);
    }
}

void ZRST::setPointsFromRST(RST *rst) {
    m_lines.clear();
    smst.set_rst(rst);
}

void ZRST::getResult(RST *rst) {
    rst->v_seg.clear();
    for (size_t i = 0; i < m_lines.size(); i++) {
        Point A(smst.points()[m_lines[i].start()].x, smst.points()[m_lines[i].start()].y);
        Point B(smst.points()[m_lines[i].end()].x, smst.points()[m_lines[i].end()].y);
        Point C(m_lines[i].mid_point().x, m_lines[i].mid_point().y);
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