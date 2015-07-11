#include "RST.h"
#include "RSTStrategy.h"

RSTStrategy::RSTStrategy() { }

int LRSTStrategy::solveRST(RST *rst) {
    LRST lmst;
    lmst.setPointsFromRST(rst);
    lmst.solve();
    lmst.getResult(rst);
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