//
// Created by ZhuangTianYi on 15/7/11.
//

#ifndef RECTILINEARSTEINERTREE_POINTSGENERATOR_H
#define RECTILINEARSTEINERTREE_POINTSGENERATOR_H

#include <vector>
#include <string>
#include "Line.h"

class PointsGenerator {
public:
    PointsGenerator() { }

    virtual ~PointsGenerator() { }

    void print_points_to_file(const std::string &filename, size_t num, int max_range);

    std::vector<Point> generate_points_from_file(const std::string &filename);

    std::vector<Point> generate_points_from_RST(RST *rst);
};


#endif //RECTILINEARSTEINERTREE_POINTSGENERATOR_H
