//
//  PointsGenerator.h
//  RectilinearSteinerTree
//
//
//  TODO(Liang_Xihao)
//

#ifndef __RectilinearSteinerTree__PointsGenerator__
#define __RectilinearSteinerTree__PointsGenerator__

#include <vector>
#include <string>
#include "DataFormat.h"


class BinaryTree {
public:
    BinaryTree() : Root(NULL) { }

    ~BinaryTree();

    struct Node {
        Point point;
        Node *leftChild;
        Node *rightChild;

        Node(const Point *_point) :
                point(*_point), leftChild(NULL), rightChild(NULL) { }

        bool operator>(const Point *_point) {
            return ((point.x > _point->x) ||
                (point.x == _point->x && point.y > _point->y));
        }

        bool operator<(const Point *_point) {
            return ((point.x < _point->x) ||
                (point.x == _point->x && point.y < _point->y));
        }
    } *Root;

    void clear(Node *_root);

    bool insertNode(const Point *_point, Node *&_root);

    //Insert _point into BinaryTree.
    //Return true when _point is successfully inserted
    //and return false when _point is found repeated in the BinaryTree.
    void exportToVector(std::vector<Point> *pointList, Node *_root);
    //Export the list of point in the BinaryTree into vector<Point>.
};
//BinaryTree Sorting is used to avoid repected point during Inserting nodes into the tree.

class PointsGenerator {
public:
    PointsGenerator() { }

    virtual ~PointsGenerator() { }

    void printPointsToFile(const char *filename, int num, int maxRange);

    std::vector<Point> byRandom(int num = 100, int maxRange = 1000);

    //Generate Point by random.
    std::vector<Point> fromFile(const std::string& filename);

    //Generate Point from a given file.
    std::vector<Point> fromRST(RST *rst);
};

#endif /* defined(__RectilinearSteinerTree__PointsGenerator__) */
