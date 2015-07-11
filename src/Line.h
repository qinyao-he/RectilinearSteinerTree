//
//  data_format.h
//  RectilinearSteinerTree
//
//  Created by Yue Shichao on 1/6/13.
//  Copyright (c) 2013 Yue Shichao. All rights reserved.
//

#ifndef RectilinearSteinerTree_data_format_h
#define RectilinearSteinerTree_data_format_h

#include <iostream>
#include <vector>

#include "Point.h"

//Point's data format

class RST;

class Line {
public:
    Line(const size_t start = -1, const size_t end = -1) : start_(start), end_(end) { }

    //initalize points vector
    virtual void print(std::ostream &out) const {
        out << "E " << start() << ' ' << end() << std::endl;
    }

    int start() const { return start_; }

    int end() const { return end_; }

private:
    size_t start_;
    size_t end_;
};

class Line_L : public Line {
public:
    Line_L(const int start = -1, const int end = -1, const bool dir = 0) :
            Line(start, end), direction_(dir) { }

    void print(std::ostream &out) const {
        out << "L " << start() << ' ' << end() << ' ' << direction() << std::endl;
    }

    bool direction() const { return direction_; }

private:
    bool direction_;
};

//Line's data format for Z-MST
class Line_Z : public Line {
public:
    Line_Z(const int start = -1, const int end = -1, const Point mid = Point()) :
            Line(start, end), mid_point(mid) { }

    void print(std::ostream &out) const {
        out << "Z " << start() << ' ' << end() << ' ' <<
        mid_point.x << ' ' << mid_point.y << std::endl;
    }

    Point mid_point;
};

#endif
