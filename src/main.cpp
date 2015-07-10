#include <random>

#include "rst.h"
#include "rststrategy.h"

int main(int argc, char const *argv[]) {
    const int n = 100;
    RST* rst = new RST;
    rst->changeStrategy(1);
    std::mt19937 generator;
    std::uniform_int_distribution<int> uniform(0, 500);
    for (int i = 0; i < n; i++) {
        int x, y;
        x = uniform(generator);
        y = uniform(generator);
        rst->addPoint(x, y);
    }
    rst->toSolveRST();
    return 0;
}
