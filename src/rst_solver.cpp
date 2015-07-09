//
// Created by hqythu on 7/8/2015.
//

#include "rst_solver.h"

#include <cstdlib>
#include <utility>
#include <tuple>
#include <limits>

#include "graph.hpp"

DistType DistType::max(std::numeric_limits<int>::max(), 0, 0);
DistType DistType::zero(0, 0, 0);


void RSTSolver::solve(std::vector<Point> points) {
    size_t n = points.size();
    Graph<DistType> graph(n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            DistType dist = DistType(points[i].distance(points[j]),
                                     -std::abs(points[i].y - points[j].y),
                                     -std::max(points[i].x, points[j].x));
            graph.add_edge(i, j, dist);
            graph.add_edge(j, i, dist);
        }
    }

    auto mst_edges = graph.mst();
}
