#include "RST.h"
#include "RSTStrategy.h"

RSTStrategy::RSTStrategy() { }

int LRSTStrategy::solveRST(RST *rst) {
    LRST lmst;
    lmst.set_points(rst);
    lmst.solve();
    lmst.get_result(rst);
    return lmst.get_result();
}

int ZRSTStrategy::solveRST(RST *rst) {
    ZRST zmst;
    zmst.set_points(rst);
    zmst.solve();
    zmst.get_result(rst);
    return zmst.get_result();
}
