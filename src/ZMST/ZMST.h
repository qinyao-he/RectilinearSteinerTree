//
//  ZMST.h
//  RectilinearSteinerTree
//
//
//
/* Author : Wang_Tianyi
 * This file declares a class ZMST, which reads data from SMST and save the
 * stable Z-MST to lines_, and the result could be printed to text files.
 */
#ifndef __RectilinearSteinerTree__Z_MST__
#define __RectilinearSteinerTree__Z_MST__
#define DEBUG

#include <cstdio>
#include <vector>
#include "../Headers/data_format.h"
#include "../SMST/SMST.h"

namespace imp_a {

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
    //The result would be in lines_. The edges is sorted by start().
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
    //accessor for smst.points_
    const vector<Point> &points() const {
        return smst.points();
    }

    //accessor for lines_
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
}
#endif /* defined(__RectilinearSteinerTree__Z_MST__) */
