//
// Created by ZhuangTianYi on 15/7/11.
//

#ifndef RECTILINEARSTEINERTREE_LRST_H
#define RECTILINEARSTEINERTREE_LRST_H

#include "Line.h"
#include "MST.h"
#include "PointsGenerator.h"
#include <vector>
#include <map>
#include <cstdlib>


class LRST {
public:
    LRST();

    void solve();

    int get_result();

    std::vector<Point> &points() {
        return m_points;
    }

    std::vector<Line> &lines() {
        return m_lines;
    }

    std::vector<Line_L> &result() {
        return m_result;
    }

private:
    MST mst;

    std::vector<Point> m_points;
    std::vector<Line> m_lines;

    void find_layout();

    std::vector<int> layout_l;
    std::vector<int> layout_u;
    int layout_result;

    std::vector<int> choice_l;
    std::vector<int> choice_u;

    int find_layout_L(int label);

    int find_layout_U(int label);

    int root;

    int find_root();

    std::vector<bool> intree;
    std::vector<int> parent;
    std::vector<std::vector<int>> tree;

    void build_tree(size_t parent);

    std::vector<int> x_coord;
    std::vector<int> y_coord;

    void discretize_data();

    std::vector<Point> discr_points;

    std::map<Point, int> hori_lines;
    std::map<Point, int> verti_lines;

    void dfs(int parent, std::vector<int> & kids, size_t num,
             int & value, int & result, int choice, int & decision);

    void paint(Point start, Point middle, bool direction,
               int color, int &value);

    void paint_hori(int u, int v, int y, int color, int &value);

    void paint_verti(int u, int v, int x, int color, int &value);

    std::vector<Line_L> m_result;

    void to_vector();

    void outputResultToVectorOfLabel(int label, bool choice);

public:
    void set_points(RST *rst);

    void get_result(RST *rst);
};


#endif //RECTILINEARSTEINERTREE_LRST_H
