//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_SMST_H
#define RECTILINEARSTEINERTREE_SMST_H

#include "DataFormat.h"
#include "PointsGenerator.h"

#include <vector>
#include <string>


class MST {
public:
    PointsGenerator pointsGenerator;

    class Dist {
    public:
        Dist() { };

        Dist(const Point *point1, const Point *point2);

        ~Dist();

        double dist() const { return m_dist; }

        bool operator < (const Dist &_LS);

        //Compare "this" and _LS in nondecreasing lexicographic order
        //and return true if this is prior to _LS
        friend std::ostream &operator << (std::ostream &out, const Dist &l); //Output the information of LS

    private:
        double m_dist;
        int m_dist_x, m_dist_y;
    };

    MST();

    virtual ~MST();

    const std::vector<Point> &points() const { return m_vertexs; }

    const std::vector<Line> &lines() const { return m_lines; }

    //using PointsGenerator to set points from file
    void mst();

    //calculate the minimum spanning tree and store the result into lines.
    void save(const std::string &filename) const;
    //print the points and lines of the MST into the file of "filename"

    void set_rst(RST *rst);

private:
    bool priorLineCompare(Line *line1, Line *line2);

    //Sort lines  in nondecreasing lexicographic order
    std::vector<Point> m_vertexs;
    //store the point set
    std::vector<Line> m_lines;
};


#endif //RECTILINEARSTEINERTREE_SMST_H
