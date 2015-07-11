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


void ZRST::dfs(int root, int father, int stat, layout &lay,
               const vector<Line_Z> &lines, vector<vector<layout>> &subProb,
               const vector<int>& head, vector<size_t>& stack) {
    using Overlap::overlap;
    if (head[root] == head[root + 1]) {
        lay.sub_ans = 0;
        return;
    }
    int id = stat - head[root], son = lines[stat].end();
    assert(id < 6);
    if (stat == head[root + 1]) {
        int ans = 0;
        vector<Line_Z> overLap;
        for (int i = 0; i < id; i++) {
            ans += subProb[son = lines[head[root] + i].end()][stack[i]].sub_ans;
            overLap.push_back(Line_Z(root, son, subProb[son][stack[i]].mid_point));
        }
        overLap.push_back(Line_Z(father, root, lay.mid_point));
        ans += overlap(points(), overLap) - point(root).distance(point(father));
        if (ans < lay.sub_ans) {
            lay.sub_ans = ans;
            copy(stack.begin(), stack.begin() + id, lay.best_lay);
        }
    } else {
        for (size_t i = 0; (stack[id] = i) < subProb[son].size(); i++) {
            dfs(root, father, stat + 1, lay, lines, subProb, head, stack);
        }
    }
}

void ZRST::solve() {
    mst.mst();
    for (const auto& line : mst.lines()) {
        m_lines.push_back(Line_Z(line.start(), line.end(), Point()));
    }
    sort(m_lines.begin(), m_lines.end(), [](const Line_Z &a, const Line_Z &b) {
        return a.start() < b.start();
    });
    vector<int> head(points().size() + 1);
    for (size_t i = 0, j = 0; i <= points().size() && j <= lines().size();) {
        if (j == lines().size() || line(j).start() != i - 1)
            head[i++] = j;
        else
            j++;
    }

    vector<int> parent(points().size());
    vector<int> x_grids(points().size()), y_grids(points().size());
    for (size_t i = 0; i < m_lines.size(); i++) {
        parent[m_lines[i].end()] = m_lines[i].start();
    }
    parent[0] = -1;
    for (size_t i = 0; i < points().size(); i++) {
        x_grids[i] = point(i).x;
        y_grids[i] = point(i).y;
    }
    sort(x_grids.begin(), x_grids.end());
    sort(y_grids.begin(), y_grids.end());
    auto x_f = unique(x_grids.begin(), x_grids.end());
    auto y_f = unique(y_grids.begin(), y_grids.end());
    x_grids.erase(x_f, x_grids.end());
    y_grids.erase(y_f, y_grids.end());
    vector<vector<layout> > subProb(vector<vector<layout> >(points().size(),
                                                            vector<layout>()));

    for (const auto& line : m_lines) {
        int min_x = min(point(line.start()).x, point(line.end()).x),
                max_x = max(point(line.start()).x, point(line.end()).x),
                min_y = min(point(line.start()).y, point(line.end()).y),
                max_y = max(point(line.start()).y, point(line.end()).y);
        int min_x_id = lower_bound(x_grids.begin(), x_grids.end(), min_x) - x_grids.begin(),
                max_x_id = lower_bound(x_grids.begin(), x_grids.end(), max_x) - x_grids.begin(),
                min_y_id = lower_bound(y_grids.begin(), y_grids.end(), min_y) - y_grids.begin(),
                max_y_id = lower_bound(y_grids.begin(), y_grids.end(), max_y) - y_grids.begin();
        for (int i = min_x_id; i <= max_x_id; i++) {
            subProb[line.end()].push_back(Point(x_grids[i], point(line.start()).y));
        }
        for (int i = min_y_id; i <= max_y_id; i++) {
            subProb[line.end()].push_back(Point(point(line.start()).x, y_grids[i]));
        }
    }

    vector<size_t> stack(6); // at most 6 child
    for (auto it = mst.lines().rbegin(); it != mst.lines().rend(); ++it) {
        for (auto lit = subProb[it->end()].begin(); lit != subProb[it->end()].end(); ++lit) {
            dfs(it->end(), parent[it->end()], head[it->end()], *lit, lines(),
                subProb, head, stack);
        }
    }
    subProb[0].push_back(point(0));
    dfs(0, 0, head[0], subProb[0][0], lines(), subProb, head, stack);
    get_ans(0, subProb[0][0], subProb, head);
}

void ZRST::get_ans(int root, const layout &lay,
                   const vector<vector<layout> > subProb,
                   const vector<int> &head) {
    for (int i = head[root]; i < head[root + 1]; i++) {
        m_lines[i].mid_point =
                subProb[line(i).end()][lay.best_lay[i - head[root]]].mid_point;
        get_ans(line(i).end(), subProb[line(i).end()][lay.best_lay[i - head[root]]],
                subProb, head);
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