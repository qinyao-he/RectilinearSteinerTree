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


void MST::mst() {
    m_lines.clear();

    Dist *const dist = new Dist[m_vertexs.size()];
    int *const parent = new int[m_vertexs.size()];

    for (int i = 1; i < m_vertexs.size(); i++) {
        dist[i] = Dist(&m_vertexs[0], &m_vertexs[i]);
        parent[i] = 0;
    }

    for (int i = 1; i < m_vertexs.size(); i++) {
        Dist bestDist;
        int bestChild = -1;

        for (int j = 1; j < m_vertexs.size(); j++) {
            if ((dist[j].dist() != 0) && ((bestChild == -1) || (dist[j] < bestDist))) {
                bestDist = dist[j];
                bestChild = j;
            }
        }

        m_lines.push_back(Line(parent[bestChild], bestChild));

        for (int j = 1; j < m_vertexs.size(); j++) {
            if (dist[j].dist() != 0) {
                Dist tmpStatus(&m_vertexs[bestChild], &m_vertexs[j]);
                if (tmpStatus < dist[j]) {
                    dist[j] = tmpStatus;
                    parent[j] = bestChild;
                }
            }
        }
    }

    delete[] dist;
    delete[] parent;
}

void MST::set_rst(RST *rst) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromRST(rst);
}


// Dist

MST::Dist::Dist(const Point *point1, const Point *point2) {
    m_dist = std::abs(point1->x - point2->x) + std::abs(point1->y - point2->y);
    m_dist_x = -std::abs(point1->y - point2->y);
    m_dist_y = -std::max(point1->x, point2->x);
}


bool MST::Dist::operator < (const Dist &op) {
    return (m_dist < op.m_dist) || (m_dist ==op.m_dist && m_dist_x < op.m_dist_x)
            || (m_dist == op.m_dist && m_dist_x == op.m_dist_x && m_dist_y < op.m_dist_y);
}