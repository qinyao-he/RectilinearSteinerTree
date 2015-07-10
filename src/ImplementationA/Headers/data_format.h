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

//Point's data format

//RST fore declare
class RST;

namespace imp_a{
class Point {
 public:
  Point(const int x = 0, const int y = 0): x_(x), y_(y) {}
  void print(std::ostream& out) const {
    out << x_ << " " << y_ << std::endl;
  }
  friend bool operator < (Point a, Point b) {
    return (a.x() < b.x()) || (a.x() == b.x() && a.y() < b.y());
  }
  //Accessors
  int x() const { return x_; }
  int y() const { return y_; }
  //Setters
  int& x_set() { return x_; }
  int& y_set() { return y_; }
 private:
  int x_;
  int y_;
};

//Line's data format for SMST
class Line {
 public:
  Line(const int start = -1, const int end = -1) : start_(start), end_(end) {}
  //initalize points vector
  virtual void print(std::ostream& out) const {
    out << "E " << start() << ' ' << end() << std::endl;
  }
  //Accessors
  int start() const { return start_; }
  int end() const { return end_; }
  //Setters
  int& start_set() { return start_; }
  int& end_set() { return end_; }
 private:
  int start_;
  int end_;
};

//Line's data format for L-MST
class Line_L : public Line {
 public:
  Line_L(const int start = -1, const int end = -1, const bool dir = 0) :
      Line(start, end), direction_(dir) {}
  void print(std::ostream& out) const {
    //start_point_id end_point_id mid_point_x mid_point_y
    out << "L " << start() << ' ' <<  end() << ' ' <<  direction() << std::endl;
  }
  //Accessor
  bool direction() const { return direction_; }
  //Setter
  bool& direction_set() { return direction_; }
 private:
  //true if the line connected to the start point is horizontal
  bool direction_;
};

//Line's data format for Z-MST
class Line_Z : public Line {
 public:
  Line_Z(const int start = -1, const int end = -1, const Point mid = Point()) :
      Line(start, end), mid_point_(mid) {}
  void print(std::ostream& out) const {
    //start_point_id end_point_id
    //mid_point1_x mid_point1_y (connected to the start point)
    //mid_point2_x mid_point2_y (connected to the end point)
    out << "Z " << start() << ' ' <<  end() << ' ' <<
          mid_point().x() << ' ' << mid_point().y() << std::endl;
  }
  //Accessor
  Point mid_point() const { return mid_point_; }
  //Setter
  Point& mid_point_set() { return mid_point_; }
 private:
  Point mid_point_;
};
}
#endif
