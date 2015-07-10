//
// Created by ZhuangTianYi on 15/7/10.
//

#include "LineStatus.h"

LineStatus::LineStatus(const Point *point1, const Point *point2) {
    m_dist = pow(point1->m_x - point2->m_x, 2) + pow(point1->m_y - point2->m_y, 2);
    m_dist_x = -std::abs(point1->m_y - point2->m_y);
    m_dist_y = -((point1->m_x >= point2->m_x) ? point1->m_x : point2->m_x);
}

LineStatus::~LineStatus() { };

bool LineStatus::operator<(const LineStatus &_LS) {
    if (this->m_dist < _LS.m_dist) return true;
    if ((this->m_dist == _LS.m_dist) && (this->m_dist_x < _LS.m_dist_x)
        || (this->m_dist == _LS.m_dist) && (this->m_dist_x == _LS.m_dist_x) && (this->m_dist_y < _LS.m_dist_y))
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& out, const LineStatus& l) {
    out << l.m_dist << ' ' << l.m_dist_y << ' ' << l.m_dist_x << std::endl;
    return out;
}