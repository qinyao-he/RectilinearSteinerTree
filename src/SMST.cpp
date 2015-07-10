//
// Created by ZhuangTianYi on 15/7/10.
//

#include "SMST.h"
#include "DataFormat.h"
#include "RST.h"
#include "LineStatus.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

SMST::SMST() { }

SMST::~SMST() { }

void SMST::save(const std::string &filename) const {
    std::ofstream fout(filename);
    fout << m_vertexs.size() << ' ' << m_lines.size() << std::endl;
    for (int i = 0; i < m_vertexs.size(); i++) {
        m_vertexs[i].print(fout);
    }
    for (int i = 0; i < m_lines.size(); i++) {
        m_lines[i].print(fout);
    }
    fout.close();
}

void SMST::mst() {
    m_lines.clear();

    LineStatus *const dist = new LineStatus[m_vertexs.size()];
    int *const parent = new int[m_vertexs.size()];

    for (int i = 1; i < m_vertexs.size(); i++) {
        dist[i] = LineStatus(&m_vertexs[0], &m_vertexs[i]);
        parent[i] = 0;
    }

    for (int i = 1; i < m_vertexs.size(); i++) {
        LineStatus bestDist;
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
                LineStatus tmpStatus(&m_vertexs[bestChild], &m_vertexs[j]);
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

void SMST::set_rst(RST *rst) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromRST(rst);
}


