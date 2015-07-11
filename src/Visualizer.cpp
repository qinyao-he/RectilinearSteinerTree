//
// Created by ZhuangTianYi on 15/7/11.
//

#include "Visualizer.h"
#include "RST.h"
#include "common.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void Visualizer::show(const RST* rst) {
    Mat picture(SCREEN_SIZE, SCREEN_SIZE, CV_8UC3, cv::Scalar(255, 255, 255));

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
}

Visualizer* Visualizer::visualizer_ = nullptr;

Visualizer *Visualizer::instance() {
    if (visualizer_ == nullptr)
        visualizer_ = new Visualizer();
    return visualizer_;
}
