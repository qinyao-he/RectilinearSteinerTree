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


bool SMST::priorLineCompare(Line *line1, Line *line2) {
    if (m_vertexs[line1->start()].x < m_vertexs[line2->start()].x)
        return true;

    if (m_vertexs[line1->start()].x == m_vertexs[line2->start()].x
        && m_vertexs[line1->start()].y < m_vertexs[line2->start()].y)
        return true;

    if (m_vertexs[line1->start()].x == m_vertexs[line2->start()].x
        && m_vertexs[line1->start()].y == m_vertexs[line2->start()].y
        && m_vertexs[line1->end()].x < m_vertexs[line2->end()].x)
        return true;

    if (m_vertexs[line1->start()].x == m_vertexs[line2->start()].x
        && m_vertexs[line1->start()].y == m_vertexs[line2->start()].y
        && m_vertexs[line1->end()].x == m_vertexs[line2->end()].x
        && m_vertexs[line1->end()].y < m_vertexs[line2->end()].y)
        return true;

    return false;
}

void SMST::calculate() {
    m_lines.clear();

    LineStatus *const minDist = new LineStatus[m_vertexs.size()];
    int *const parentIndex = new int[m_vertexs.size()];

    for (int i = 1; i < m_vertexs.size(); i++) {
        minDist[i] = LineStatus(&m_vertexs[0], &m_vertexs[i]);
        parentIndex[i] = 0;
    }

    for (int i = 1; i < m_vertexs.size(); i++) {
        LineStatus bestDist;
        int bestChild = -1;

        for (int j = 1; j < m_vertexs.size(); j++) {
            if ((minDist[j].dist() != 0) && ((bestChild == -1) || (minDist[j] < bestDist))) {
                bestDist = minDist[j];
                bestChild = j;
            }
        }

        m_lines.push_back(Line(parentIndex[bestChild], bestChild));

        for (int j = 1; j < m_vertexs.size(); j++) {
            if (minDist[j].dist() != 0) {
                LineStatus tmpStatus(&m_vertexs[bestChild], &m_vertexs[j]);
                if (tmpStatus < minDist[j]) {
                    minDist[j] = tmpStatus;
                    parentIndex[j] = bestChild;
                }
            }
        }
    }

    delete[] minDist;
    delete[] parentIndex;
}

void SMST::set_rst(RST *rst) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromRST(rst);
}


