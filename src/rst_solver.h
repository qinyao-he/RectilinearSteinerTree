//
// Created by hqythu on 7/8/2015.
//

#ifndef RST_RST_SOLVER_H
#define RST_RST_SOLVER_H

#include <vector>

#include "point.h"


class DistType {
public:
    DistType(int dist, int y, int x) : dist(dist), y(y), x(x) {}

    static DistType max, zero;

    bool operator < (const DistType& op) {
        return (dist < op.dist) || (dist == op.dist && y < op.y) || (dist == op.dist && y == op.y && x < op.x);
    }
    bool operator > (const DistType& op) {
        return (dist > op.dist) || (dist == op.dist && y > op.y) || (dist == op.dist && y == op.y && x > op.x);
    }
    bool operator == (const DistType& op) {
        return (dist == op.dist) && (y == op.y) && (x == op.x);
    }
private:
    int dist, y, x;
};


class RSTSolver {
public:
    void solve(std::vector<Point> points);
};


#endif //RST_RST_SOLVER_H
