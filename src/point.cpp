//
// Created by hqythu on 6/22/2015.
//

#include "point.h"

#include <cstdlib>


int Point::distance(const Point& op) const
{
    return std::abs(x - op.x) + std::abs(y - op.y);
}


bool Point::operator < (const Point &op) const {
    return (x < op.x) || (x == op.x && y < op.y);
}


bool Point::operator == (const Point &op) const {
    return x == op.x && y == op.y;
}
