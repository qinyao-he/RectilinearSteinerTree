//
// Created by ZhuangTianYi on 15/7/10.
//

#include "LineStatus.h"

LineStatus::LineStatus(const Point *point1, const Point *point2) {
    m_dist = pow(point1->x - point2->x, 2) + pow(point1->y - point2->y, 2);
    m_dist_x = -std::abs(point1->y - point2->y);
    m_dist_y = -((point1->x >= point2->x) ? point1->x : point2->x);
}

LineStatus::~LineStatus() { };

bool LineStatus::operator < (const LineStatus &op) {
    return m_dist < op.m_dist || (m_dist == op.m_dist && m_dist_x < op.m_dist_x)
            || (m_dist == op.m_dist && m_dist_x == op.m_dist_x && m_dist_y < op.m_dist_y);
}
