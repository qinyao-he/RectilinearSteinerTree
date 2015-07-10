#ifndef RST_H
#define RST_H

#include <vector>

#include "DataFormat.h"
#include "Overlap.h"
#include "MST.h"
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

    void addPoint(int x, int y);

    std::vector<Point> v_op; // original points
    std::vector<Segment> v_seg; // all segments

    int changeStrategy(int s_idx);

    int solve();

private:
    int isSolved_, myStrategyIdx_, pointsCert_, isNew_;
    RSTStrategy *myStrategy_;
};


#endif // RST_H
