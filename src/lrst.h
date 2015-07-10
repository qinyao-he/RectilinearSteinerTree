//
// Created by hqythu on 7/10/2015.
//

#ifndef RST_LRST_H
#define RST_LRST_H

#include <vector>

#include "graph.hpp"
#include "point.h"


class LRST {
public:
    void solve(const std::vector<Point>& points, int root, const Graph<int>& tree);
private:
    std::vector<Point> points;
    Graph<int> tree;
    void cal_max_overlap(int root);
};


#endif //RST_LRST_H
