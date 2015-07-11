#include "RST.h"
#include "RSTStrategy.h"

RSTStrategy::RSTStrategy() { }

int LRSTStrategy::solveRST(RST *rst) {
    LRST lmst;
    lmst.set_points(rst);
    lmst.solve();
    lmst.get_result(rst);
    return 1;
}

int ZRSTStrategy::solveRST(RST *rst) {
    ZRST zmst;
    zmst.setPointsFromRST(rst);
    zmst.solve();
    zmst.getResult(rst);
    return 1;
}
//
//int ImpASMST::solveRST(RST *rst) {
//
//}