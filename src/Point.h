//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_POINT_H
#define RECTILINEARSTEINERTREE_POINT_H

#include <iostream>


struct Point {
    Point(const int x = 0, const int y = 0) : m_x(x), m_y(y) { }

    void print(std::ostream &out) const {
        out << m_x << " " << m_y << std::endl;
    }

    friend bool operator<(Point a, Point b) {
        return (a.m_x < b.m_x) || (a.m_x == b.m_x && a.m_y < b.m_y);
    }

    int m_x;
    int m_y;
};


#endif //RECTILINEARSTEINERTREE_POINT_H
