#ifndef RST_H
#define RST_H

typedef int DTYPE;

#include <vector>

#include "DataFormat.h"
#include "Overlap.h"
#include "SMST.h"
#include "LMST.h"
#include "ZMST.h"
#include "Point.h"
#include "Segment.h"
#include "RSTStrategy.h"

class RST {
public:
    RST();

    ~RST();

    void loadPoints(std::vector<Point> &gen_p);

    void addPoint(DTYPE x, DTYPE y);

    std::vector<Point> v_op; // original points
    std::vector<Segment> v_seg; // all segments

    int changeStrategy(int s_idx);

    int solve();

private:
    int isSolved_, myStrategyIdx_, pointsCert_, isNew_;
    DTYPE overall_;
    RSTStrategy *myStrategy_;
};


#endif // RST_H
