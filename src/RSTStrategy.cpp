#include "RST.h"
#include "RSTStrategy.h"

RSTStrategy::RSTStrategy() { }

int LMSTStrategy::solveRST(RST *rst) {
    LMST lmst;
    lmst.setPointsFromRST(rst);
    lmst.init();
    lmst.getResult(rst);
    return 1;
}

int ZMSTStrategy::solveRST(RST *rst) {
    ZMST zmst;
    zmst.setPointsFromRST(rst);
    zmst.exec();
    zmst.getResult(rst);
    return 1;
}
//
//int ImpASMST::solveRST(RST *rst) {
//
//}