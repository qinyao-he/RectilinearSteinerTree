#include "RST.h"
#include "RSTStrategy.h"

#include <cstdlib>
#include <fstream>

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


void RST::loadPoints(std::vector<Point> &gen_p) {
    v_op.clear();
    for (int i = 0; i < gen_p.size(); i++)
        v_op.push_back(gen_p[i]);
    isNew_ = 1;

    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}


int RST::changeStrategy(int s_idx) {
    if (s_idx == myStrategyIdx_) return 0;
    if (myStrategyIdx_)
        delete myStrategy_;

    myStrategyIdx_ = s_idx;
    switch (s_idx) {
        case 1  : {
            myStrategy_ = new LMSTStrategy;
            break;
        }
        case 2  : {
            myStrategy_ = new ZMSTStrategy;
            break;
        }
        default : {
            myStrategyIdx_ = 0;
        }
    }
    // strategyChanged(s_idx);
    return myStrategyIdx_;
}

void RST::solve() {
    myStrategy_->solveRST(this);
}


void RST::addPoint(int x, int y) {
    v_op.push_back(Point(x, y));
    pointsCert_ = rand();
}
