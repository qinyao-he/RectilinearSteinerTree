//
// Created by ZhuangTianYi on 15/7/10.
//

#ifndef RECTILINEARSTEINERTREE_SMST_H
#define RECTILINEARSTEINERTREE_SMST_H

#include "DataFormat.h"
#include "PointsGenerator.h"

#include <vector>
#include <string>


class SMST {
public:
    PointsGenerator pointsGenerator;

    const std::vector<Point> &points() const { return m_vertexs; }

    const std::vector<Line> &lines() const { return m_lines; }

    void mst();

    void set_rst(RST *rst);

private:
    std::vector<Point> m_vertexs;
    std::vector<Line> m_lines;
};


#endif //RECTILINEARSTEINERTREE_SMST_H
