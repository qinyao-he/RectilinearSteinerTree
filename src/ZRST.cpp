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


void ZRST::dfs(int root, int father, size_t stat, layout &lay, vector<size_t>& stack) {
    using Overlap::overlap;
    if (tree[root].size() == 0) {
        lay.sub_ans = 0;
        return;
    }
    int id = stat, son;
    assert(id < 6);
    if (stat == tree[root].size()) {
        int ans = 0;
        vector<Line_Z> overLap;
        for (int i = 0; i < id; i++) {
            ans += layouts[son = tree[root][i]][stack[i]].sub_ans;
            overLap.push_back(Line_Z(root, son, layouts[son][stack[i]].mid_point));
        }
        overLap.push_back(Line_Z(father, root, lay.mid_point));
        ans += overlap(points(), overLap) - point(root).distance(point(father));
        if (ans < lay.sub_ans) {
            lay.sub_ans = ans;
            copy(stack.begin(), stack.begin() + id, lay.best_lay);
        }
    } else {
        son = tree[root][stat];
        for (size_t i = 0; (stack[id] = i) < layouts[son].size(); i++) {
            dfs(root, father, stat + 1, lay, stack);
        }
    }
}

int ZRST::find_root() {
    std::vector<int> degree(m_points.size(), 0);
    for (const auto& line : m_lines) {
        degree[line.start()]++;
        degree[line.end()]++;
    }
    for (auto it = degree.begin(); it != degree.end(); ++it) {
        if ((*it) == 1) {
            return (it - degree.begin());
        }
    }
    assert(false);
    return -1;
}

void ZRST::build_tree(size_t father) {
    size_t child;
    intree[father] = true;
    for (const auto& line : m_lines) {
        if (line.start() != father && line.end() != father) {
            continue;
        }
        if (line.start() == father) {
            child = line.end();
        } else {
            child = line.start();
        }
        if (intree[child]) {
            continue;
        }
        tree[father].push_back(child);
        parent[child] = father;
        build_tree(child);
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

    intree.resize(m_points.size(), false);
    parent.resize(m_points.size(), -1);
    tree.resize(m_points.size(), vector<int>());
    root = find_root();
    build_tree(root);

    discretize_data();

    layouts.resize(points().size());
    stack.resize(6);

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

    find_layout(tree[root][0]);

    layouts[root].push_back(point(0));
    dfs(root, root, 0, layouts[root][0], stack);
    m_lines.clear();
    get_ans(root, layouts[root][0]);
}

void ZRST::get_ans(int root, const layout &lay) {
    int i = 0;
    for (const auto& child : tree[root]) {
        m_lines.push_back(Line_Z(root, child, layouts[child][lay.best_lay[i]].mid_point));
        get_ans(child, layouts[child][lay.best_lay[i]]);
        i++;
    }
}

void ZRST::set_points(RST *rst) {
    m_lines.clear();
    mst.set_rst(rst);
}

void ZRST::get_result(RST *rst) {
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

void ZRST::find_layout(int label) {
    for (const auto& child : tree[label]) {
        find_layout(child);
    }
    for (auto& layout : layouts[label]) {
        dfs(label, parent[label], 0, layout, stack);
    }
}

int ZRST::get_result() {
    return layouts[root][0].sub_ans;
}
