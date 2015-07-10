//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_POINT_H
#define RECTILINEARSTEINERTREE_POINT_H

#include <iostream>


struct Point {
    Point(const int x = 0, const int y = 0) : x(x), y(y) { }

    void print(std::ostream &out) const {
        out << x << " " << y << std::endl;
    }

    friend bool operator<(Point a, Point b) {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    }

    int x;
    int y;
};


#endif //RECTILINEARSTEINERTREE_POINT_H
