//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_SMST_H
#define RECTILINEARSTEINERTREE_SMST_H

#include <vector>
#include "data_format.h"
#include "PointsGenerator.h"


class LineStatus {
public:
    LineStatus() { };

    LineStatus(const Point *point1, const Point *point2);

    ~LineStatus();

    double dist() const { return m_dist; }

    bool operator<(const LineStatus &_LS);

    //Compare "this" and _LS in nondecreasing lexicographic order
    //and return true if this is prior to _LS
    friend std::ostream &operator<<(std::ostream& out, const LineStatus& l); //Output the information of LS

private:
    double m_dist;
    int m_dist_x, m_dist_y;
};

class SMST {
public:
    PointsGenerator pointsGenerator;

    SMST();

    virtual ~SMST();

    const std::vector<Point>& points() const { return m_vertexs; }

    const std::vector<Line>& lines() const { return m_lines; }

    void setPointsByRandom(int num = 100, int maxRange = 1000);

    //using PointsGenerator to set points by random
    void setPointsFromFile(const char *filename);

    //using PointsGenerator to set points from file
    void calculateMST();

    //calculate the minimum spanning tree and store the result into lines.
    void printMSTToFile(const char *filename) const;
    //print the points and lines of the MST into the file of "filename"

    void setPointsFromRST(RST *rst);

private:
    bool priorLineCompare(Line *line1, Line *line2);

    //Compare line1 and line2 in nondecreasing lexicographic order
    //and return true if line1 is prior to line2
    void lineSort(int startIndex, int endIndex);

    //Sort lines  in nondecreasing lexicographic order
    std::vector<Point> m_vertexs;
    //store the point set
    std::vector<Line> m_lines;
};


#endif //RECTILINEARSTEINERTREE_SMST_H
