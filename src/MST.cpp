//
// Created by ZhuangTianYi on 15/7/10.
//

#include "MST.h"
#include "Line.h"
#include "RST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <limits>


using std::vector;


void MST::mst() {
    m_lines.clear();

    const size_t n = m_vertexs.size();

    Dist dist_max(std::numeric_limits<int>::max(), 0, 0);
    vector<Dist> dist(n, dist_max);
    vector<int> parent(n, 0);
    vector<bool> in_tree(n, false);

    int t = 0;
    dist[t] = Dist(m_vertexs[t], m_vertexs[t]);

    for (size_t k = 0; k < n - 1; k++) {
        in_tree[t] = true;
        for (int j = 0; j < n; j++)
            if (j != t) {
                Dist tmp(m_vertexs[t], m_vertexs[j]);
                if (tmp < dist[j]) {
                    dist[j] = tmp;
                    parent[j] = t;
                }
            }
        Dist min = dist_max;
        int min_v;
        for (size_t i = 0; i < n; i++) {
            if (dist[i] < min && !in_tree[i]) {
                min = dist[i];
                min_v = i;
            }
        }
        m_lines.push_back(Line(parent[t], t));
        t = min_v;
    }
}

void MST::set_rst(RST *rst) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromRST(rst);
}


// Dist

MST::Dist::Dist(const Point& point1, const Point& point2) {
    m_dist = std::abs(point1.x - point2.x) + std::abs(point1.y - point2.y);
    m_dist_x = -std::abs(point1.y - point2.y);
    m_dist_y = -std::max(point1.x, point2.x);
}


bool MST::Dist::operator < (const Dist &op) {
    return (m_dist < op.m_dist) || (m_dist ==op.m_dist && m_dist_x < op.m_dist_x)
            || (m_dist == op.m_dist && m_dist_x == op.m_dist_x && m_dist_y < op.m_dist_y);
}