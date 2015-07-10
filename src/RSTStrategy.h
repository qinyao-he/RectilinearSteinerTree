#ifndef RSTSTRATEGY_H
#define RSTSTRATEGY_H

/* rst strategy */

class RSTStrategy {
public:
    RSTStrategy();

    virtual int solveRST(RST *rst) = 0;
};

/* concrete strategies */

//class ImpASMST : public RSTStrategy {
//public:
//    int solveRST(RST *rst);
//};

class LMST_Strategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

class ZMST_strategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

#endif // RSTSTRATEGY_H
