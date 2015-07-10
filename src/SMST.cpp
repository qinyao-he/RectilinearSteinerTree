//
// Created by ZhuangTianYi on 15/7/10.
//

#include "SMST.h"
#include "data_format.h"
#include "rst.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

std::ostream& operator<<(std::ostream& out, const LineStatus& l) {
    out << l.m_dist << ' ' << l.m_dist_y << ' ' << l.m_dist_x << std::endl;
    return out;
}


LineStatus::LineStatus(const Point *point1, const Point *point2) {
    m_dist = pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2);
    m_dist_x = -std::abs(point1->y() - point2->y());
    m_dist_y = -((point1->x() >= point2->x()) ? point1->x() : point2->x());
}

LineStatus::~LineStatus() { };

bool LineStatus::operator<(const LineStatus &_LS) {
    if (this->m_dist < _LS.m_dist) return true;
    if ((this->m_dist == _LS.m_dist) && (this->m_dist_x < _LS.m_dist_x)
        || (this->m_dist == _LS.m_dist) && (this->m_dist_x == _LS.m_dist_x) && (this->m_dist_y < _LS.m_dist_y))
        return true;
    return false;
}

SMST::SMST() { }

SMST::~SMST() { }

void SMST::setPointsByRandom(int num, int maxRange) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.byRandom(num, maxRange);
}

void SMST::setPointsFromFile(const std::string& filename) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromFile(filename);
}

void SMST::printMSTToFile(const std::string& filename) const {
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
    if (m_vertexs[line1->start()].x() < m_vertexs[line2->start()].x())
        return true;

    if (m_vertexs[line1->start()].x() == m_vertexs[line2->start()].x()
        && m_vertexs[line1->start()].y() < m_vertexs[line2->start()].y())
        return true;

    if (m_vertexs[line1->start()].x() == m_vertexs[line2->start()].x()
        && m_vertexs[line1->start()].y() == m_vertexs[line2->start()].y()
        && m_vertexs[line1->end()].x() < m_vertexs[line2->end()].x())
        return true;

    if (m_vertexs[line1->start()].x() == m_vertexs[line2->start()].x()
        && m_vertexs[line1->start()].y() == m_vertexs[line2->start()].y()
        && m_vertexs[line1->end()].x() == m_vertexs[line2->end()].x()
        && m_vertexs[line1->end()].y() < m_vertexs[line2->end()].y())
        return true;

    return false;
}

void SMST::lineSort(int startIndex, int endIndex) {
    if (startIndex < endIndex) {
        int sIndex = startIndex;
        int eIndex = endIndex;
        Line sample = m_lines[(sIndex + eIndex) / 2];
        while (sIndex <= eIndex) {
            while (priorLineCompare(&m_lines[sIndex], &sample)) sIndex++;
            while (priorLineCompare(&sample, &m_lines[eIndex])) eIndex--;
            if (sIndex <= eIndex) {
                if (sIndex != eIndex) {
                    Line tmpLine = m_lines[sIndex];
                    m_lines[sIndex] = m_lines[eIndex];
                    m_lines[eIndex] = tmpLine;
                }
                sIndex++;
                eIndex--;
            }
        }
        if (startIndex < eIndex) lineSort(startIndex, eIndex);
        if (sIndex < endIndex) lineSort(sIndex, endIndex);
    }
}

void SMST::calculateMST() {
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

void SMST::setPointsFromRST(RST *rst) {
    m_vertexs.clear();
    m_vertexs = pointsGenerator.fromRST(rst);
}


