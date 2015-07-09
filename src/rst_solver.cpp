//
// Created by hqythu on 7/8/2015.
//

#include "rst_solver.h"

#include <cstdlib>
#include <utility>
#include <tuple>

#include "graph.hpp"


void RSTSolver::solve(std::vector<Point> points) {
    typedef std::tuple<int, int, int> dist_type;

    size_t n = points.size();
    Graph<dist_type> graph(n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            dist_type dist = dist_type(points[i].distance(points[j]),
                                       -std::abs(points[i].y - points[j].y),
                                       -std::max(points[i].x, points[j].x));
            graph.add_edge(i, j, dist);
            graph.add_edge(j, i, dist);
        }
    }

    auto mst_edges = graph.mst();
}
