//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_POINT_H
#define RECTILINEARSTEINERTREE_POINT_H

#include <iostream>
#include <cstdlib>


struct Point {
    Point(const int x = 0, const int y = 0) : x(x), y(y) { }

    void print(std::ostream &out) const {
        out << x << " " << y << std::endl;
    }

    bool operator == (const Point& op) const {
        return (x == op.x) && (y == op.y);
    }

    bool operator < (const Point& op) const {
        return (x < op.x) || (x == op.x && y < op.y);
    }

    int distance(const Point& op) const {
        return std::abs(x - op.x) + std::abs(y - op.y);
    }

    int x;
    int y;
};


#endif //RECTILINEARSTEINERTREE_POINT_H
