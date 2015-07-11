//
// Created by ZhuangTianYi on 15/7/10.
//

#include "ZRST.h"
#include "RST.h"
#include "Overlap.h"

#include <ctime>
#include <cassert>

using std::abs;
using std::copy;
using std::lower_bound;
using std::max;
using std::min;
using std::sort;
using std::unique;
using std::vector;


void ZRST::dfs(int root, int father, int stat, layout &lay, vector<size_t>& stack) {
    using Overlap::overlap;
    if (head[root] == head[root + 1]) {
        lay.sub_ans = 0;
        return;
    }
    int id = stat - head[root], son = m_lines[stat].end();
    assert(id < 6);
    if (stat == head[root + 1]) {
        int ans = 0;
        vector<Line_Z> overLap;
        for (int i = 0; i < id; i++) {
            ans += layouts[son = m_lines[head[root] + i].end()][stack[i]].sub_ans;
            overLap.push_back(Line_Z(root, son, layouts[son][stack[i]].mid_point));
        }
        overLap.push_back(Line_Z(father, root, lay.mid_point));
        ans += overlap(points(), overLap) - point(root).distance(point(father));
        if (ans < lay.sub_ans) {
            lay.sub_ans = ans;
            copy(stack.begin(), stack.begin() + id, lay.best_lay);
        }
    } else {
        for (size_t i = 0; (stack[id] = i) < layouts[son].size(); i++) {
            dfs(root, father, stat + 1, lay, stack);
        }
    }
}

void ZRST::discretize_data() {
    x_coord.resize(0);
    y_coord.resize(0);
    std::map<int, int> x_coord_map, y_coord_map;
    for (const auto& point : m_points) {
        int x = point.x, y = point.y;
        x_coord_map[x] = 0;
        y_coord_map[y] = 0;
    }
    for (const auto& x : x_coord_map) {
        x_coord_map[x.first] = x_coord.size();
        x_coord.push_back(x.first);
    }
    for (const auto& y : y_coord_map) {
        y_coord_map[y.first] = y_coord.size();
        y_coord.push_back(y.first);
    }
    for (const auto& point : m_points) {
        int x = point.x, y = point.y;
        int disp_x = x_coord_map[x];
        int disp_y = y_coord_map[y];
        discr_points.push_back(Point(disp_x, disp_y));
    }
    for (size_t i = 0; i < x_coord_map.size(); i++) {
        for (size_t j = 0; j < y_coord_map.size(); j++) {
            hori_lines.insert(std::pair<Point, int>(Point(i, j), 0));
            verti_lines.insert(std::pair<Point, int>(Point(i, j), 0));
        }
    }
}

void ZRST::solve() {
    mst.mst();
    m_points = mst.points();
    for (const auto& line : mst.lines()) {
        m_lines.push_back(Line_Z(line.start(), line.end(), Point()));
    }
    sort(m_lines.begin(), m_lines.end(), [](const Line_Z &a, const Line_Z &b) {
        return a.start() < b.start();
    });
    head.resize(points().size() + 1);
    for (size_t i = 0, j = 0; i <= points().size() && j <= lines().size();) {
        if (j == lines().size() || line(j).start() != i - 1)
            head[i++] = j;
        else
            j++;
    }

    vector<int> parent(points().size());
    for (size_t i = 0; i < m_lines.size(); i++) {
        parent[m_lines[i].end()] = m_lines[i].start();
    }
    parent[0] = -1;

    discretize_data();

    layouts.resize(points().size());

    for (const auto& line : m_lines) {
        int min_x = min(point(line.start()).x, point(line.end()).x),
                max_x = max(point(line.start()).x, point(line.end()).x),
                min_y = min(point(line.start()).y, point(line.end()).y),
                max_y = max(point(line.start()).y, point(line.end()).y);
        int min_x_id = lower_bound(x_coord.begin(), x_coord.end(), min_x) - x_coord.begin(),
                max_x_id = lower_bound(x_coord.begin(), x_coord.end(), max_x) - x_coord.begin(),
                min_y_id = lower_bound(y_coord.begin(), y_coord.end(), min_y) - y_coord.begin(),
                max_y_id = lower_bound(y_coord.begin(), y_coord.end(), max_y) - y_coord.begin();
        for (int i = min_x_id; i <= max_x_id; i++) {
            layouts[line.end()].push_back(Point(x_coord[i], point(line.start()).y));
        }
        for (int i = min_y_id; i <= max_y_id; i++) {
            layouts[line.end()].push_back(Point(point(line.start()).x, y_coord[i]));
        }
    }

    vector<size_t> stack(6); // at most 6 child
    for (auto it = mst.lines().rbegin(); it != mst.lines().rend(); ++it) {
        for (auto lit = layouts[it->end()].begin(); lit != layouts[it->end()].end(); ++lit) {
            dfs(it->end(), parent[it->end()], head[it->end()], *lit, stack);
        }
    }
    layouts[0].push_back(point(0));
    dfs(0, 0, head[0], layouts[0][0], stack);
    get_ans(0, layouts[0][0]);
}

void ZRST::get_ans(int root, const layout &lay) {
    for (int i = head[root]; i < head[root + 1]; i++) {
        m_lines[i].mid_point =
                layouts[line(i).end()][lay.best_lay[i - head[root]]].mid_point;
        get_ans(line(i).end(), layouts[line(i).end()][lay.best_lay[i - head[root]]]);
    }
}

void ZRST::setPointsFromRST(RST *rst) {
    m_lines.clear();
    mst.set_rst(rst);
}

void ZRST::getResult(RST *rst) {
    rst->v_seg.clear();
    for (size_t i = 0; i < m_lines.size(); i++) {
        Point A(mst.points()[m_lines[i].start()].x, mst.points()[m_lines[i].start()].y);
        Point B(mst.points()[m_lines[i].end()].x, mst.points()[m_lines[i].end()].y);
        Point C(m_lines[i].mid_point.x, m_lines[i].mid_point.y);
        Point D;
        if (std::abs(A.y - C.y) < 0.1)
            D = Point(C.x, B.y);
        else
            D = Point(B.x, C.y);
        rst->v_seg.push_back(Segment(A, C));
        rst->v_seg.push_back(Segment(C, D));
        rst->v_seg.push_back(Segment(D, B));
    }
}