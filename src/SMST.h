//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_SMST_H
#define RECTILINEARSTEINERTREE_SMST_H

#include "data_format.h"
#include "PointsGenerator.h"

#include <vector>
#include <string>


class SMST {
public:
    PointsGenerator pointsGenerator;

    SMST();

    virtual ~SMST();

    const std::vector<Point> &points() const { return m_vertexs; }

    const std::vector<Line> &lines() const { return m_lines; }

    void setPointsByRandom(int num = 100, int maxRange = 1000);

    //using PointsGenerator to set points by random
    void set_points(const std::string &filename);

    //using PointsGenerator to set points from file
    void calculate();

    //calculate the minimum spanning tree and store the result into lines.
    void save(const std::string &filename) const;
    //print the points and lines of the MST into the file of "filename"

    void set_rst(RST *rst);

private:
    bool priorLineCompare(Line *line1, Line *line2);

    //Compare line1 and line2 in nondecreasing lexicographic order
    //and return true if line1 is prior to line2
    void sort(int startIndex, int endIndex);

    //Sort lines  in nondecreasing lexicographic order
    std::vector<Point> m_vertexs;
    //store the point set
    std::vector<Line> m_lines;
};


#endif //RECTILINEARSTEINERTREE_SMST_H
