//
//  overlap.h
//  Overlap
//
//  Created by Yue Shichao on 6/6/13.
//  Copyright (c) 2013 Yue Shichao. All rights reserved.
//

#ifndef Overlap_overlap_h
#define Overlap_overlap_h

#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include "Line.h"


namespace Overlap {

using std::vector;

//two vectors store the start point and the end point of all lines
struct Points_Array {
    vector<Point> start;
    vector<Point> end;
};

//this struct is used for computing overlap by storing all the intervals
struct Record {
    enum record_kind {
        VERTICAL = 0,
        HORIZONTAL = 1
    };

    struct Interval {
        Interval(int a = 0, int b = 0) : start(a), end(b) { }

        int start;
        int end;
    };

    record_kind type;
    int coodinate;
    vector<Interval> intervals;
};

inline void add_interval(const int a, const int b, Record &record);

static void add_line(const Point &a, const Point &b, vector<Record> &records);

static int compute_sum(vector<Record::Interval> &intervals);

inline bool compare_Interval(const Record::Interval &a, const Record::Interval &b);

static Points_Array convert_format(const vector<Point> &points,
                                   const vector<Line_Z> &lines_z);

//This funtion's aim is to compute the sum in given Line_Z vector
static int overlap(const vector<Point> &points, const vector<Line_Z> &lines) {
    vector<Record> records;
    Points_Array data = convert_format(points, lines);
    for (int i = 0; i < (int) data.start.size(); ++i) {
        add_line(data.start[i], data.end[i], records);
    }
    int sum = 0;
    for (vector<Record>::iterator i = records.begin(); i != records.end(); ++i) {
        sum += compute_sum(i->intervals);
    }
    return sum;
}

static Points_Array convert_format(const vector<Point> &points,
                                   const vector<Line_Z> &lines_z) {
    Points_Array data;
    for (vector<Line_Z>::const_iterator i = lines_z.begin(); i != lines_z.end();
         ++i) {
        Point start, end, mid1, mid2;
        start = points[i->start()];
        end = points[i->end()];
        mid1 = i->mid_point;
        if (start.y == mid1.y) {
            mid2.x = mid1.x;
            mid2.y = end.y;
        } else if (start.x == mid1.x) {
            mid2.x = end.x;
            mid2.y = mid1.y;
        } else {
            start.print(std::cout);
            mid1.print(std::cout);
            mid2.print(std::cout);
            end.print(std::cout);
            std::cout << "There are something wrong in the data\n";
            abort();
        }
        data.start.push_back(start);
        data.end.push_back(mid1);
        data.start.push_back(mid1);
        data.end.push_back(mid2);
        data.start.push_back(mid2);
        data.end.push_back(end);
    }
    assert(data.start.size() == data.end.size());
    return data;
}

static int compute_sum(vector<Record::Interval> &intervals) {
    assert(intervals.size() > 0);
    std::sort(&intervals[0], &intervals[0] + intervals.size(), compare_Interval);
    int head = intervals[0].start;
    int tail = intervals[0].end;
    int sum = 0;
    for (vector<Record::Interval>::iterator i = intervals.begin();
         i != intervals.end(); ++i) {
        if (i->start <= tail) {
            if (i->end > tail) {
                tail = i->end;
            }
        } else {
            sum += (tail - head);
            head = i->start;
            tail = i->end;
        }
    }
    sum += tail - head;
    return sum;
}

//add a interval into records
static void add_line(const Point &a, const Point &b, vector<Record> &records) {
    if (a.y == b.y) {
        int flag = 0;
        for (vector<Record>::iterator j = records.begin();
             j != records.end(); ++j) {
            if (j->coodinate == a.y && j->type == Record::HORIZONTAL) {
                add_interval(a.x, b.x, *j);
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            Record record;
            record.type = Record::HORIZONTAL;
            record.coodinate = a.y;
            add_interval(a.x, b.x, record);
            records.push_back(record);
        }
    } else if (a.x == b.x) {
        int flag = 0;
        for (vector<Record>::iterator j = records.begin();
             j != records.end(); ++j) {
            if (j->coodinate == a.x && j->type == Record::VERTICAL) {
                add_interval(a.y, b.y, *j);
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            Record record;
            record.type = Record::VERTICAL;
            record.coodinate = a.x;
            add_interval(a.y, b.y, record);
            records.push_back(record);
        }
    }
}

inline void add_interval(const int a, const int b, Record &record) {
    Record::Interval interval(std::min(a, b), std::max(a, b));
    record.intervals.push_back(interval);
}

inline bool compare_Interval(const Record::Interval &a, const Record::Interval &b) {
    if (a.start < b.start) {
        return true;
    } else if (a.start == b.start) {
        if (a.end > b.end) {
            return true;
        }
    }
    return false;
}

}   //namespace Overlap

#undef MAX
#undef MIN
#endif
