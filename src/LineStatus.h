//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_LINESTATUS_H
#define RECTILINEARSTEINERTREE_LINESTATUS_H

#include <iostream>
#include <cmath>
#include "PointsGenerator.h"

class LineStatus {
public:
    LineStatus() { };

    LineStatus(const Point *point1, const Point *point2);

    ~LineStatus();

    double dist() const { return m_dist; }

    bool operator < (const LineStatus &_LS);

    //Compare "this" and _LS in nondecreasing lexicographic order
    //and return true if this is prior to _LS
    friend std::ostream &operator << (std::ostream &out, const LineStatus &l); //Output the information of LS

private:
    double m_dist;
    int m_dist_x, m_dist_y;
};


#endif //RECTILINEARSTEINERTREE_LINESTATUS_H
