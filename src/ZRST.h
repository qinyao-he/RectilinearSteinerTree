//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_ZMST_H
#define RECTILINEARSTEINERTREE_ZMST_H

#include <cstdio>
#include <vector>
#include "Line.h"
#include "MST.h"


using std::vector;

class ZRST {
private:
    //A structure storing all possible midpoints and the best answer under the
    //restrict. The best layout would be in bestLay[] after solve() is called.
    struct layout {
        Point midPoint;
        int subAns;
        int bestLay[8];

        layout() : subAns((~0u) >> 1) { }

        layout(const Point &mid_) : midPoint(mid_), subAns((~0u) >> 1) { }
    };

    //Use smst to get data and the mst
    MST mst;
    //The result would be in m_lines. The edges is sorted by start().
    vector<Line_Z> m_lines;

    //SubProcedure for solve(). It would examine the subProb and look for
    //best solutions for each node.
    void getAns(int root, const layout &lay,
                const vector<vector<layout> > subProb,
                const vector<int>& head);

    //SubProcedure for solve(). It would examine all the layouts for the sons of a
    //node and get the best one.layout::subAns and layout::bestLay should be
    //filled after this procedure.
    void dfs(int root, int father, int stat, ZRST::layout &lay,
             const vector<Line_Z> &lines, vector<vector<layout> > &subProb,
             const vector<int>& head, vector<size_t>& stack);

public:
    //accessor for smst.m_vertexs
    const vector<Point> &points() const {
        return mst.points();
    }

    //accessor for m_lines
    const vector<Line_Z> &lines() const {
        return m_lines;
    }

    //access a single point
    const Point &point(int i) const {
        return points()[i];
    }

    //access a single edge
    const Line &line(int i) const {
        return lines()[i];
    }

    //The main algorithm
    void solve();

public:
    void setPointsFromRST(RST *rst);

    void getResult(RST *rst);
};


#endif //RECTILINEARSTEINERTREE_ZMST_H
