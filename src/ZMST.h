//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_ZMST_H
#define RECTILINEARSTEINERTREE_ZMST_H

#include <cstdio>
#include <vector>
#include "DataFormat.h"
#include "SMST.h"


using std::vector;

class ZMST {
private:
    //A structure storing all possible midpoints and the best answer under the
    //restrict. The best layout would be in bestLay[] after exec() is called.
    struct layout {
        Point midPoint;
        int subAns;
        int bestLay[8];

        layout() : subAns((~0u) >> 1) { }

        layout(const Point &mid_) : midPoint(mid_), subAns((~0u) >> 1) { }
    };

    //Use smst to get data and the mst
    SMST smst;
    //The result would be in m_lines. The edges is sorted by start().
    vector<Line_Z> lines_;

    //SubProcedure for exec(). It would examine the subProb and look for
    //best solutions for each node.
    void getAns(int root, const layout &lay,
                const vector<vector<layout> > subProb,
                const int *head);

    //SubProcedure for exec(). It would examine all the layouts for the sons of a
    //node and get the best one.layout::subAns and layout::bestLay should be
    //filled after this procedure.
    void dfs(int root, int father, int stat, ZMST::layout &lay,
             const vector<Line_Z> &lines, vector<vector<layout> > &subProb,
             const int *head, int *stack);

public:
    //accessor for smst.m_vertexs
    const vector<Point> &points() const {
        return smst.points();
    }

    //accessor for m_lines
    const vector<Line_Z> &lines() const {
        return lines_;
    }

    //access a single point
    const Point &point(int i) const {
#ifdef DEBUG
        return points().at(i);
#else
        return points()[i];
#endif
    }

    //access a single edge
    const Line &line(int i) const {
#ifdef DEBUG
        return lines().at(i);
#else
        return lines()[i];
#endif
    }

    //The main algorithm
    void exec();

    // print the result to a file
    void print(const char *fileName) const;

    // print the result to a FILE*
    void print(FILE *out) const;

    //for debug use. generates data randomly
    void setPointsByRandom(int num = 100, int maxRange = 1000);

    //read data from a file
    void setPointsFromFile(const char *fileName);

public:
    void setPointsFromRST(RST *rst);

    void getResult(RST *rst);
};


#endif //RECTILINEARSTEINERTREE_ZMST_H
