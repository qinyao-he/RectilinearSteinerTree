//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_SMST_H
#define RECTILINEARSTEINERTREE_SMST_H

#include "Line.h"
#include "PointsGenerator.h"

#include <vector>
#include <string>


class MST {
public:
    PointsGenerator pointsGenerator;

    class Dist {
    public:
        Dist() {}

        Dist(int dist, int x, int y) : m_dist(dist), m_dist_x(x), m_dist_y(y) { }

        Dist(const Point& point1, const Point& point2);

        int dist() const { return m_dist; }

        bool operator < (const Dist &op);

    private:
        int m_dist;
        int m_dist_x, m_dist_y;
    };


    const std::vector<Point> &points() const { return m_vertexs; }

    const std::vector<Line> &lines() const { return m_lines; }

    void mst();

    void set_rst(RST *rst);

private:
    std::vector<Point> m_vertexs;
    std::vector<Line> m_lines;
};


#endif //RECTILINEARSTEINERTREE_SMST_H
