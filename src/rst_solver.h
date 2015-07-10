//
// Created by hqythu on 7/8/2015.
//

#ifndef RST_RST_SOLVER_H
#define RST_RST_SOLVER_H

#include <vector>

#include "point.h"
#include "graph.hpp"


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
    void solve(const std::vector<Point>& points);
private:
    int find_root(size_t n, const std::vector<Graph<DistType>::Edge>& edges);

    void dfs(int node, const Graph<int>& origin_tree, Graph<int> &tree);
};


#endif //RST_RST_SOLVER_H
