//
//  SMST.cpp
//  RectilinearSteinerTree
//
//
//  TODO(Liang_Xihao)
//

#include "SMST.h"
#include "../Headers/data_format.h"
#include "../../rst.h"
#include <iostream>
#include <fstream>
#include <cmath>

namespace imp_a {

LineStatus::LineStatus(const Point *point1, const Point *point2) {
    dist_ = pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2);
    distX_ = -std::abs(point1->y() - point2->y());
    distY_ = -((point1->x() >= point2->x()) ? point1->x() : point2->x());
}

LineStatus::~LineStatus() { };

bool LineStatus::operator<(const LineStatus &_LS) {
    if (this->dist_ < _LS.dist_) return true;
    if ((this->dist_ == _LS.dist_) && (this->distX_ < _LS.distX_)) return true;
    if ((this->dist_ == _LS.dist_) && (this->distX_ == _LS.distX_) && (this->distY_ < _LS.distY_)) return true;
    return false;
}

SMST::SMST() { }

SMST::~SMST() { }

void SMST::setPointsByRandom(int num, int maxRange) {
    points_.clear();
    points_ = pointsGenerator.byRandom(num, maxRange);
}

void SMST::setPointsFromFile(const char *filename) {
    points_.clear();
    points_ = pointsGenerator.fromFile(filename);
}

void SMST::printMSTToFile(const char *filename) const {
    std::ofstream ofile(filename);
    ofile << points_.size() << ' ' << lines_.size() << std::endl;
    for (int i = 0; i < points_.size(); i++) {
        points_[i].print(ofile);
    }
    for (int i = 0; i < lines_.size(); i++) {
        lines_[i].print(ofile);
    }
    ofile.close();
}


bool SMST::priorLineCompare(Line *line1, Line *line2) {
    if (points_[line1->start()].x() < points_[line2->start()].x())
        return true;

    if (points_[line1->start()].x() == points_[line2->start()].x()
        && points_[line1->start()].y() < points_[line2->start()].y())
        return true;

    if (points_[line1->start()].x() == points_[line2->start()].x()
        && points_[line1->start()].y() == points_[line2->start()].y()
        && points_[line1->end()].x() < points_[line2->end()].x())
        return true;

    if (points_[line1->start()].x() == points_[line2->start()].x()
        && points_[line1->start()].y() == points_[line2->start()].y()
        && points_[line1->end()].x() == points_[line2->end()].x()
        && points_[line1->end()].y() < points_[line2->end()].y())
        return true;

    return false;
}

void SMST::lineSort(int startIndex, int endIndex) {
    if (startIndex < endIndex) {
        int sIndex = startIndex;
        int eIndex = endIndex;
        Line sample = lines_[(sIndex + eIndex) / 2];
        while (sIndex <= eIndex) {
            while (priorLineCompare(&lines_[sIndex], &sample)) sIndex++;
            while (priorLineCompare(&sample, &lines_[eIndex])) eIndex--;
            if (sIndex <= eIndex) {
                if (sIndex != eIndex) {
                    Line tmpLine = lines_[sIndex];
                    lines_[sIndex] = lines_[eIndex];
                    lines_[eIndex] = tmpLine;
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
    lines_.clear();

    LineStatus *const minDist = new LineStatus[points_.size()];
    int *const parentIndex = new int[points_.size()];

    for (int i = 1; i < points_.size(); i++) {
        minDist[i] = LineStatus(&points_[0], &points_[i]);
        parentIndex[i] = 0;
    }

    for (int i = 1; i < points_.size(); i++) {
        LineStatus bestDist;
        int bestChild = -1;

        for (int j = 1; j < points_.size(); j++) {
            if ((minDist[j].dist() != 0) && ((bestChild == -1) || (minDist[j] < bestDist))) {
                bestDist = minDist[j];
                bestChild = j;
            }
        }

        lines_.push_back(Line(parentIndex[bestChild], bestChild));

        for (int j = 1; j < points_.size(); j++) {
            if (minDist[j].dist() != 0) {
                LineStatus tmpStatus(&points_[bestChild], &points_[j]);
                if (tmpStatus < minDist[j]) {
                    minDist[j] = tmpStatus;
                    parentIndex[j] = bestChild;
                }
            }
        }
    }

    //lineSort(0, lines_.size()-1);

    delete[] minDist;
    delete[] parentIndex;
}

void SMST::setPointsFromRST(RST *rst) {
    points_.clear();
    points_ = pointsGenerator.fromRST(rst);
}

}
