#include "rst.h"
#include "rststrategy.h"

RSTStrategy::RSTStrategy() { }

int LMST_Strategy::solveRST(RST *rst) {
    LMST lmst;
    lmst.setPointsFromRST(rst);
    lmst.init();
    lmst.getResult(rst);
    return 1;
}

int ZMST_strategy::solveRST(RST *rst) {
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