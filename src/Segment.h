//
// Created by hqythu on 7/10/2015.
//

#ifndef RECTILINEARSTEINERTREE_SEGMENT_H
#define RECTILINEARSTEINERTREE_SEGMENT_H

#include "Point.h"


class Segment {
public:
    Segment(Point u, Point v) : u(u), v(v) { }
    Point u, v;
};


#endif //RECTILINEARSTEINERTREE_SEGMENT_H
