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

    void printPointsToFile(const std::string& filename, int num, int max_range);

//    std::vector<Point> byRandom(int num = 100, int maxRange = 1000);

    //Generate Point by random.
    std::vector<Point> fromFile(const std::string& filename);

    //Generate Point from a given file.
    std::vector<Point> fromRST(RST *rst);
};


#endif //RECTILINEARSTEINERTREE_POINTSGENERATOR_H
