#include <random>

#include "RST.h"
#include "RSTStrategy.h"
#include "common.h"
#include "Visualizer.h"

int main(int argc, char const *argv[]) {
    const int n = NUM;

    RST* rst = new RST;
    PointsGenerator* p = new PointsGenerator;
    std::string filename = "../test/data_0.txt";

    rst->changeStrategy(ZRST);

    if (argc > 1) {
        if (!strcmp(argv[1], "ZRST"))
            rst->changeStrategy(ZRST);
        else if (!strcmp(argv[1], "LRST"))
            rst->changeStrategy(LRST);
        else if (!strcmp(argv[1], "test")){
            p->print_points_to_file(filename, NUM, MAX_RANGE);
        }
    } 

    std::vector<Point> points = p->generate_points_from_file(filename);
    rst->loadPoints(points);
    rst->solve();

    Visualizer* visualizer = Visualizer::instance();
    visualizer->show(rst);

    delete rst;
    delete visualizer;
    return 0;
}
