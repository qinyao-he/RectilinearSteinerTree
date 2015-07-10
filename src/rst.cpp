#include "rst.h"
#include "rststrategy.h"

#include <cstdlib>
#include <fstream>

/* About Point & Segment */

inline bool dEq(DTYPE x, DTYPE y) { return (x - eps < y) && (x + eps > y); }

inline bool dLe(DTYPE x, DTYPE y) { return (x + eps < y) ? 1 : 0; }

bool operator==(Point2D p, Point2D q) { return dEq(p.x, q.x) && dEq(p.y, q.y); }

bool operator<(Point2D p, Point2D q) { return dLe(p.x, q.x) || (dEq(p.x, q.x) && dLe(p.y, q.y)); }

bool operator==(Segment2D a, Segment2D b) { return a.u == b.u && a.v == b.v; }

bool operator<(Segment2D a, Segment2D b) { return a.u < b.u || (a.u == b.u && a.v < b.v); }

Point2D mkPoint(DTYPE x, DTYPE y) {
    Point2D p;
    p.x = x, p.y = y;
    return p;
}

Segment2D mkSegment(DTYPE x1, DTYPE y1, DTYPE x2, DTYPE y2) {
    Segment2D s;
    s.u.x = x1, s.u.y = y1;
    s.v.x = x2, s.v.y = y2;
    return s;
}

Segment2D mkSegment(Point2D u, Point2D v) {
    Segment2D s;
    s.u = u, s.v = v;
    return s;
}

/* About RST */

RST::RST() :
        isSolved_(0),
        myStrategyIdx_(0),
        isNew_(1) {
    v_op.clear();
    v_seg.clear();
    myStrategy_ = NULL;
}

RST::~RST() {
    if (myStrategyIdx_)
        delete myStrategy_;
}


void RST::loadPoints(std::vector<Point2D> &gen_p) {
    v_op.clear();
    for (int i = 0; i < gen_p.size(); i++)
        v_op.push_back(gen_p[i]);
    isNew_ = 1;

    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}


int RST::changeStrategy(int s_idx) {
    if (s_idx == myStrategyIdx_) return 0;
    if (myStrategyIdx_) delete myStrategy_;

    myStrategyIdx_ = s_idx;
    switch (s_idx) {
        case 1  : {
            myStrategy_ = new LMST_Strategy;
            break;
        }
        case 2  : {
            myStrategy_ = new ZMST_strategy;
            break;
        }
        default : {
            myStrategyIdx_ = 0;
        }
    }
    // strategyChanged(s_idx);
    return myStrategyIdx_;
}

int RST::solve() {
    myStrategy_->solveRST(this);
}


void RST::addPoint(DTYPE x, DTYPE y) {
    v_op.push_back(mkPoint(x, y));
    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}
