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



//Line's data format for MST
class Line {
public:
    Line(const size_t start = -1, const size_t end = -1) : start_(start), end_(end) { }

    //initalize points vector
    virtual void print(std::ostream &out) const {
        out << "E " << start() << ' ' << end() << std::endl;
    }

    //Accessors
    int start() const { return start_; }

    int end() const { return end_; }

    //Setters
    size_t &start_set() { return start_; }

    size_t &end_set() { return end_; }

private:
    size_t start_;
    size_t end_;
};

//Line's data format for L-MST
class Line_L : public Line {
public:
    Line_L(const int start = -1, const int end = -1, const bool dir = 0) :
            Line(start, end), direction_(dir) { }

    void print(std::ostream &out) const {
        //start_point_id end_point_id mid_point_x mid_point_y
        out << "L " << start() << ' ' << end() << ' ' << direction() << std::endl;
    }

    //Accessor
    bool direction() const { return direction_; }

    //Setter
    bool &direction_set() { return direction_; }

private:
    //true if the line connected to the start point is horizontal
    bool direction_;
};

//Line's data format for Z-MST
class Line_Z : public Line {
public:
    Line_Z(const int start = -1, const int end = -1, const Point mid = Point()) :
            Line(start, end), mid_point_(mid) { }

    void print(std::ostream &out) const {
        out << "Z " << start() << ' ' << end() << ' ' <<
        mid_point().x << ' ' << mid_point().y << std::endl;
    }

    //Accessor
    Point mid_point() const { return mid_point_; }

    //Setter
    Point &mid_point_set() { return mid_point_; }

private:
    Point mid_point_;
};

#endif
