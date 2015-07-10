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

void RST::loadPoints(const char *fileName) {
    std::ifstream fin;
    DTYPE x, y;
    fin.open(fileName);

    int n;
    fin >> n;
    v_op.clear();
    for (int i = 0; i < n; i++) {
        fin >> x >> y;
        v_op.push_back(mkPoint(x, y));
    }
    fin.close();
    setSolved(0);
    isNew_ = 1;

    //qDebug("%d", myStrategyIdx_);
    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}

void RST::loadPoints(std::vector<Point2D> &gen_p) {
    v_op.clear();
    for (int i = 0; i < gen_p.size(); i++)
        v_op.push_back(gen_p[i]);
    setSolved(0);
    isNew_ = 1;

    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}

int RST::isSolved() {
    return isSolved_;
}

void RST::setSolved(int solved) {
    isSolved_ = solved;
}

int RST::hasData() {
    return v_op.size();
}

int RST::getStrategy() {
    return myStrategyIdx_;
}

DTYPE RST::overall() {
    return overall_;
}

/* slots */

int RST::changeStrategy(int s_idx) {
    if (s_idx == myStrategyIdx_) return 0;
    if (myStrategyIdx_) delete myStrategy_;

    myStrategyIdx_ = s_idx;
    switch (s_idx) {
        case 1  : {
            myStrategy_ = new ImpALMST;
            break;
        }
        case 2  : {
            myStrategy_ = new ImpAZMST;
            break;
        }
        default : {
            myStrategyIdx_ = 0;
        }
    }
    setSolved(0);

    // strategyChanged(s_idx);
    return myStrategyIdx_;
}

int RST::toSolveRST() {
    myStrategy_->solveRST(this);
}

void RST::savePoints(const char *fileName) {
    std::ofstream fout(fileName);

    fout << v_op.size() << "\n";
    for (int i = 0; i < v_op.size(); i++) {
        fout << v_op[i].x << ' ' << v_op[i].y << "\n";
        //qDebug("%d %d", v_op[i].x, v_op[i].y);
    }
    fout.close();
}

void RST::addPoint(DTYPE x, DTYPE y) {
    v_op.push_back(mkPoint(x, y));
    setSolved(0);
    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}

void RST::deletePoint(DTYPE x, DTYPE y) {
    int flag = 0;
    for (int i = v_op.size() - 1; i >= 0; i--)
        if (std::abs(v_op[i].x - x) + std::abs(v_op[i].y - y) < 1) {
            v_op.erase(v_op.begin() + i);
            flag = 1;
            break;
        }
    if (flag) {
        setSolved(0);
        pointsCert_ = rand();
        // pointsChanged(pointsCert_);
    }
}

void RST::deleteAndAdd(DTYPE ox, DTYPE oy, DTYPE nx, DTYPE ny) {
    int flag = 0;
    for (int i = v_op.size() - 1; i >= 0; i--)
        if (std::abs(v_op[i].x - ox) + std::abs(v_op[i].y - oy) < 1) {
            v_op.erase(v_op.begin() + i);
            flag = 1;
            break;
        }
    v_op.push_back(mkPoint(nx, ny));
    setSolved(0);
    pointsCert_ = rand();
    // pointsChanged(pointsCert_);
}

int RST::isNew() {
    return isNew_;
}

void RST::setNew(int x) {
    isNew_ = x;
}
