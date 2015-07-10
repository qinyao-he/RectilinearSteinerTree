//
//  PointsGenerator.cpp
//  RectilinearSteinerTree
//
//
//  TODO(Liang_Xihao)
//

#include <ctime>
#include <cstdlib>
#include <fstream>
#include "PointsGenerator.h"
#include "rst.h"


bool BinaryTree::insertNode(const Point *_point, Node *&_root) {
    if (_root == NULL) {
        _root = new Node(_point);
        return true;
    }
    else if (*_root > _point) return insertNode(_point, _root->leftChild);
    else if (*_root < _point) return insertNode(_point, _root->rightChild);
    else return false; //which means the point is repeated
}

void BinaryTree::exportToVector(std::vector<Point> *pointList, Node *_root) {
    if (_root != NULL) {
        exportToVector(pointList, _root->leftChild);
        pointList->push_back(_root->point);
        exportToVector(pointList, _root->rightChild);
    }
}

void BinaryTree::clear(Node *_root) {
    if (_root != NULL) {
        clear(_root->leftChild);
        clear(_root->rightChild);
        delete _root;
    }
}

BinaryTree::~BinaryTree() {
    clear(Root);
}

std::vector<Point> PointsGenerator::byRandom(int num, int maxRange) {
    srand(clock());
    int pointsCount = 0;
    BinaryTree binaryTree;
    while (pointsCount < num) {
        int x = rand() % maxRange;
        int y = rand() % maxRange;
        Point *tmpPoint = new Point(x, y);
        if (binaryTree.insertNode(tmpPoint, binaryTree.Root))
            pointsCount++;
        delete tmpPoint;
    }

    std::vector<Point> pointList;
    pointList.clear();
    binaryTree.exportToVector(&pointList, binaryTree.Root);

    return pointList;
}

void PointsGenerator::printPointsToFile(const char *filename, int num, int maxRange) {
    using std::vector;
    vector<Point> points = byRandom(num, maxRange);
    std::ofstream fout(filename);
    for (vector<Point>::iterator i = points.begin(); i != points.end(); ++i) {
        i->print(fout);
    }
    fout.close();
}

std::vector<Point> PointsGenerator::fromFile(const char *filename) {
    BinaryTree binaryTree;

    std::ifstream ifile(filename);
    while (ifile) {
        int x, y;
        ifile >> x;
        if (ifile >> y) {
            Point *tmpPoint = new Point(x, y);
            binaryTree.insertNode(tmpPoint, binaryTree.Root);
            delete tmpPoint;
        }
    }
    ifile.close();

    std::vector<Point> pointList;
    pointList.clear();
    binaryTree.exportToVector(&pointList, binaryTree.Root);

    return pointList;
}

std::vector<Point> PointsGenerator::fromRST(RST *rst) {
    BinaryTree binaryTree;

    for (int i = 0; i < rst->v_op.size(); i++) {
        int x, y;
        x = rst->v_op[i].x, y = rst->v_op[i].y;

        Point *tmpPoint = new Point(x, y);
        binaryTree.insertNode(tmpPoint, binaryTree.Root);
        delete tmpPoint;
    }
    std::vector<Point> pointList;
    pointList.clear();
    binaryTree.exportToVector(&pointList, binaryTree.Root);

    return pointList;
}


