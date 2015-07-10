#ifndef RSTSTRATEGY_H
#define RSTSTRATEGY_H

/* rst strategy */

class RSTStrategy {
public:
    RSTStrategy();

    virtual int solveRST(RST *rst) = 0;
};

class LMSTStrategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

class ZMSTStrategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

#endif // RSTSTRATEGY_H
