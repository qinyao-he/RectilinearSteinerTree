//
//  SMST.h
//  RectilinearSteinerTree
//
//
//  TODO(Liang_Xihao)
//

#ifndef __RectilinearSteinerTree__SMST__
#define __RectilinearSteinerTree__SMST__

#include <vector>
#include "data_format.h"
#include "PointsGenerator.h"

namespace imp_a {

class LineStatus {
public:
    LineStatus() { };

    LineStatus(const Point *point1, const Point *point2);

    ~LineStatus();

    double dist() const { return dist_; }

    bool operator<(const LineStatus &_LS);

    //Compare "this" and _LS in nondecreasing lexicographic order
    //and return true if this is prior to _LS
    friend std::ostream &operator<<(std::ostream &out, const LineStatus &LS) {
        out << LS.dist_ << ' ' << LS.distY_ << ' ' << LS.distX_ << '\n';
        return out;
    } //Output the information of LS

private:
    double dist_;
    int distY_;
    int distX_;
};

class SMST {
public:
    PointsGenerator pointsGenerator;

    SMST();

    virtual ~SMST();

    const std::vector<Point> &points() const { return points_; }

    const std::vector<Line> &lines() const { return lines_; }

    void setPointsByRandom(int num = 100, int maxRange = 1000);

    //using PointsGenerator to set points by random
    void setPointsFromFile(const char *filename);

    //using PointsGenerator to set points from file
    void calculateMST();

    //calculate the minimum spanning tree and store the result into lines.
    void printMSTToFile(const char *filename) const;
    //print the points and lines of the MST into the file of "filename"

    void setPointsFromRST(RST *rst);

    void getResult(RST *rst);

private:
    bool priorLineCompare(Line *line1, Line *line2);

    //Compare line1 and line2 in nondecreasing lexicographic order
    //and return true if line1 is prior to line2
    void lineSort(int startIndex, int endIndex);

    //Sort lines  in nondecreasing lexicographic order
    std::vector<Point> points_;
    //store the point set
    std::vector<Line> lines_;
    //store the line set of the minimum spanning tree of points
};

} //namespace imp A

#endif /* defined(__RectilinearSteinerTree__SMST__) */
