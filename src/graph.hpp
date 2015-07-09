//
// Created by hqythu on 6/20/2015.
//

#ifndef RST_GRAPH_HPP
#define RST_GRAPH_HPP

#include <cstddef>
#include <vector>
#include <list>
#include <limits>


template <typename W>
class Graph {
public:
    struct Edge {
        Edge(int u = 0, int v = 0, W w = 0) : u(u), v(v), w(w) {}
        int u, v;
        W w;
    };

    Graph(size_t n = 0) {
        n_vertex = n;
        adjlist.resize(n_vertex);
    }

    void add_edge(int u, int v, W w) {
        Edge e(u, v, w);
        adjlist[u].push_back(e);
    }

    std::list<Edge> edge_list(int u) const { return adjlist[u]; }

    size_t get_n_vertex() const { return n_vertex; }

    std::vector<Edge> mst() const {
        using std::vector;
        size_t n = get_n_vertex();
        vector<bool> in_tree(n, false);
        vector<W> dist(n, W((std::numeric_limits<double>::max)()));
        vector<Edge> pre(n);
        vector<Edge> edges;
        int t = 0;
        dist[t] = 0;
        for(size_t k = 1; k < n; k++) {
            in_tree[t] = true;
            for (const auto& edge : edge_list(t)) {
                if(dist[edge.v] > edge.w && !in_tree[edge.v]) {
                    dist[edge.v] = edge.w;
                    pre[edge.v] = edge;
                }
            }
            W min = W((std::numeric_limits<double>::max)());
            int minv = 0;
            for(size_t i = 0; i < n; i++) {
                if (min > dist[i] && !in_tree[i]) {
                    min = dist[i];
                    minv = i;
                }
            }
            edges.push_back(pre[minv]);
            t = minv;
        }
        return edges;
    }

private:
    size_t n_vertex;
    std::vector<std::list<Edge>> adjlist;
};


#endif //RST_GRAPH_HPP
