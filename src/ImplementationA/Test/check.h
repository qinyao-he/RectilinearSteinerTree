//
//  Check.h
//  RectilinearSteinerTree
//
//  Created by Yue Shichao on 25/6/13.
//
//

#ifndef RectilinearSteinerTree_Check_h
#define RectilinearSteinerTree_Check_h

#include <vector>
#include "../Headers/overlap.h"

using std::vector;

namespace Check {

//vector AB and vector CD's cross product
long long cross_product(const Point &A, const Point &B,
                        const Point &C, const Point &D) {
    int x1 = B.x() - A.x();
    int y1 = B.y() - A.y();
    int x2 = D.x() - C.x();
    int y2 = D.y() - C.y();
    return (x1 * y2 - x2 * y1);
}

//Check whether there are crossings
template<typename T>
bool crossing(const vector<Point> &points, const T &lines) {
    using namespace std;
    using Overlap::Points_Array;
    using Overlap::convert_format;
    Points_Array data = convert_format(points, lines);
    for (int i = 0; i < data.start.size(); ++i) {
        for (int j = i + 1; j < data.start.size(); ++j) {
            //for convenience, give points a shorter name;
            Point &A = data.start[i], B = data.end[i],
                    C = data.start[j], D = data.end[j];
            //check crossing
            if (((cross_product(A, C, A, B) * cross_product(A, D, A, B)) < 0) &&
                ((cross_product(C, A, C, D) * cross_product(C, B, C, D)) < 0)) {
                return false;
            }
        }
    }
    return true;
}

} //namespace Check

#endif
