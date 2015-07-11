#ifndef RSTSTRATEGY_H
#define RSTSTRATEGY_H

/* rst strategy */

class RSTStrategy {
public:
    RSTStrategy();
    virtual ~RSTStrategy() {}

    virtual int solveRST(RST *rst) = 0;
};

class LRSTStrategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

class ZRSTStrategy : public RSTStrategy {
public:
    int solveRST(RST *rst);
};

#endif // RSTSTRATEGY_H
