#include <random>

#include "RST.h"
#include "RSTStrategy.h"
#include "common.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char const *argv[]) {
    const int n = NUM;
    RST* rst = new RST;
    rst->changeStrategy(2);
    std::mt19937 generator;
    std::uniform_int_distribution<int> uniform(0, 500);
    for (int i = 0; i < n; i++) {
        int x, y;
        x = uniform(generator);
        y = uniform(generator);
        rst->addPoint(x, y);
    }
    rst->solve();

    Mat picture(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));

    for (const auto& seg : rst->v_seg) {
        line(picture,
             cv::Point(seg.u.x, seg.u.y),
             cv::Point(seg.v.x, seg.v.y),
             cv::Scalar(0, 0, 0));
    }

    for (const auto& p : rst->v_op) {
        circle(picture,
             cv::Point(p.x, p.y),
             3, cv::Scalar(255, 0, 0), -1);
    }

    namedWindow("RST");
    imshow("RST", picture);
    waitKey(0);
    return 0;
}
