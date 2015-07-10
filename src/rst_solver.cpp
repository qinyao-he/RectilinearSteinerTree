//
// Created by hqythu on 7/8/2015.
//

#include "rst_solver.h"

#include <cstdlib>
#include <utility>
#include <tuple>
#include <vector>
#include <limits>

#include "graph.hpp"
#include "lrst.h"

using std::vector;


DistType DistType::max(std::numeric_limits<int>::max(), 0, 0);
DistType DistType::zero(0, 0, 0);


void RSTSolver::solve(const std::vector<Point>& points) {
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

    int root = find_root(n, mst_edges);
    Graph<int> g(n);
    for (const auto& edge : mst_edges) {
        g.add_edge(edge.u, edge.v, 0);
        g.add_edge(edge.v, edge.u, 0);
    }

    Graph<int> tree(n);
    dfs(root, g, tree);

    LRST lrst;
    lrst.solve(points, root, tree);
}


int RSTSolver::find_root(size_t n, const std::vector<Graph<DistType>::Edge>& edges) {
    vector<int> degree(n, 0);
    for (const auto& edge : edges) {
        degree[edge.u]++;
        degree[edge.v]++;
    }
    for (size_t i = 0; i < n; i++) {
        if (degree[i] == 1) {
            return i;
        }
    }
}


void RSTSolver::dfs(int node, const Graph<int>& origin_tree, Graph<int> &tree) {
    auto adjlist = origin_tree.edge_list(node);
    for (const auto& e : adjlist) {
        tree.add_edge(e.u, e.v, 0);
    }
    for (const auto& e : adjlist) {
        dfs(e.v, origin_tree, tree);
    }
}
