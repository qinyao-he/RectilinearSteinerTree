#include <random>

#include "RST.h"
#include "RSTStrategy.h"
#include "common.h"
#include "Visualizer.h"

int main(int argc, char const *argv[]) {
    const int n = NUM;
    RST* rst = new RST;
    rst->changeStrategy(2);
    PointsGenerator* p = new PointsGenerator;
    std::string filename = "../test/data_0.txt";
    p -> print_points_to_file(filename, NUM, MAX_RANGE);
    std::vector<Point> points = p->generate_points_from_file(filename);
    rst->loadPoints(points);
//    std::mt19937 generator;
//    std::uniform_int_distribution<int> uniform(0, SCREEN_SIZE);
//    for (int i = 0; i < n; i++) {
//        int x, y;
//        x = uniform(generator);
//        y = uniform(generator);
//        rst->addPoint(x, y);
//    }
    rst->solve();

    Visualizer* visualizer = Visualizer::instance();
    visualizer -> show(rst);

    delete rst;
    delete visualizer;
    return 0;
}
