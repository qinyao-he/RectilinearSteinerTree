#ifndef RST_H
#define RST_H

typedef int DTYPE;

#include <vector>

#include "ImplementationA/Headers/data_format.h"
#include "ImplementationA/Headers/overlap.h"
#include "ImplementationA/SMST/SMST.h"
#include "ImplementationA/LMST/LMST.h"
#include "ImplementationA/ZMST/ZMST.h"

const double eps = 1e-6;
inline bool dEq(DTYPE x, DTYPE y);
inline bool dLe(DTYPE x, DTYPE y);

struct Point2D
{
    DTYPE x, y;
};

Point2D mkPoint(DTYPE x, DTYPE y);
bool operator ==(Point2D p, Point2D q);
bool operator <(Point2D p, Point2D q);

struct Segment2D
{
    Point2D u, v;
};

Segment2D mkSegment(DTYPE x1, DTYPE y1, DTYPE x2, DTYPE y2);
Segment2D mkSegment(Point2D u, Point2D v);
bool operator ==(Segment2D a, Segment2D b);
bool operator <(Segment2D a, Segment2D b);

class RST;
class RSTStrategy;

class RST
{
public:
    RST();
    ~RST();
    void loadPoints(const char* fileName);
    void loadPoints(std::vector< Point2D > &gen_p);

    void addPoint(DTYPE x, DTYPE y);
    void deletePoint(DTYPE x, DTYPE y);
    void deleteAndAdd(DTYPE ox, DTYPE oy, DTYPE nx, DTYPE ny);

    int isSolved();
    int isNew();
    void setNew(int x);
    void setSolved(int solved);
    int hasData();
    int getStrategy();

    DTYPE overall();

    std::vector< Point2D > v_op; // original points
    std::vector< Segment2D > v_seg; // all segments

    void savePoints(const char* fileName);

    // void pointsChanged(int cert);
    // void strategyChanged(int s_idx);
    // void solveFinished();

    int changeStrategy(int s_idx);
    int toSolveRST();

private:
    int isSolved_, myStrategyIdx_, pointsCert_, isNew_;
    DTYPE overall_;
    RSTStrategy *myStrategy_;
};


#endif // RST_H
