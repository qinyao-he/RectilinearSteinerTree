//
// Created by ZhuangTianYi on 15/7/11.
//

#ifndef RECTILINEARSTEINERTREE_VISUALIZER_H
#define RECTILINEARSTEINERTREE_VISUALIZER_H

#include "RST.h"

class Visualizer {
private:
    static Visualizer* visualizer_;
    Visualizer() { }
    Visualizer(const Visualizer&) { }
    void operator = (const Visualizer &) { }
public:
    static Visualizer* instance();
    void show(const RST* rst);
};


#endif //RECTILINEARSTEINERTREE_VISUALIZER_H
