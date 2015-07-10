//
// Created by ZhuangTianYi on 15/7/10.
//

#include "LineStatus.h"

LineStatus::LineStatus(const Point *point1, const Point *point2) {
    m_dist = std::abs(point1->x - point2->x) + std::abs(point1->y - point2->y);
    m_dist_x = -std::abs(point1->y - point2->y);
    m_dist_y = -std::max(point1->x, point2->y);
}

LineStatus::~LineStatus() { };

bool LineStatus::operator < (const LineStatus &op) {
    return m_dist < op.m_dist || (m_dist == op.m_dist && m_dist_x < op.m_dist_x)
            || (m_dist == op.m_dist && m_dist_x == op.m_dist_x && m_dist_y < op.m_dist_y);
}
