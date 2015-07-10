#include "rst.h"
#include "rststrategy.h"

RSTStrategy::RSTStrategy()
{
    
}

int ImpALMST::solveRST(RST *rst)
{
    imp_a::LMST lmst;
    lmst.setPointsFromRST(rst);
    lmst.init();
    lmst.getResult(rst);
    return 1;
}

int ImpAZMST::solveRST(RST *rst)
{
    imp_a::ZMST zmst;
    zmst.setPointsFromRST(rst);
    zmst.exec();
    zmst.getResult(rst);
    return 1;
}

int ImpASMST::solveRST(RST* rst) 
{
    // imp_a::SMST smst;
    // smst.setPointsFromRST(rst);
    // smst.init();
    // smst.getResult(rst);
    // return 1;
}