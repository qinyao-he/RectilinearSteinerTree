/// Project name: New Algorithms for the Rectilinear Steiner Tree Problem
/// Sub-project: LMST
//  LMST.h

#ifndef __RectilinearSteinerTree__LMST__
#define __RectilinearSteinerTree__LMST__

#include "Line.h"
#include "MST.h"
#include "PointsGenerator.h"
#include <vector>
#include <map>


class LMST {
public:
    LMST();

    ////////////////////////////////////////////////////////////////////////////
    // Interfaces.
    ////////////////////////////////////////////////////////////////////////////
    // Initializes Data. In fact it calculates the psi_l, psi_u, and the LMST.
    void solve();

    // Returns the result.
    int get_result();

    // Returns the vector of m_vertexs.
    std::vector<Point> &points() {
        return m_points;
    }

    // Returns the vector of m_lines.
    std::vector<Line> &lines() {
        return m_lines;
    }

    // Returns the vector of m_result.
    std::vector<Line_L> &result() {
        return m_result;
    }

    // A function object for comparing two Points. Since someone thinks that
    // adding such comparison in the definition of Points is useless, I might
    // as well use this way.
    class IsLessThanForPoint {
    public:
        bool operator()(const Point &a, const Point &b) const {
            return (a.x < b.x || (a.x == b.x && a.y < b.y));
        }
    };

private:
    ////////////////////////////////////////////////////////////////////////////
    // Basic Utilities Section: Some basic functions and data.
    ////////////////////////////////////////////////////////////////////////////


    // Measures the Manhattan Distance btwn. Point x and Point y.
    int dist(Point x, Point y) {
        return abs(x.x - y.x) + abs(x.y - y.y);
    }

    // The function used for calculating absolute values.
    int abs(int x) {
        return x > 0 ? x : -x;
    }

    // the class created for calculating the MST.
    MST mst;
    // The points and lines of the MST given.
    std::vector<Point> m_points;
    std::vector<Line> m_lines;


    ////////////////////////////////////////////////////////////////////////////
    // Algorithm Control Section: Controls the flow of the core algorithm.
    ////////////////////////////////////////////////////////////////////////////

    // Find the Psi of all the edges.
    void FindPsi();

    // psi_l and psi_u are denoted just as the paper describes.
    // We use findPsiL and findPsiU to find the psi_l and psi_u,
    // and save the choices(L-shape or U-shape?) in result_for_l_.
    std::vector<int> psi_l;
    std::vector<int> psi_u;
    int psi_result;
    // choice_l and choice_u denotes the best method of l or u, in binary
    // notation. 1 means true and 0 means false
    std::vector<int> choice_l;
    std::vector<int> choice_u;

    // Note that if we already have calculated the edge, we don't need to
    // calculate it again. Instead we put them into psi_l and psi_u.
    int findPsiL(int /*label*/);

    int findPsiU(int /*label*/);

    ////////////////////////////////////////////////////////////////////////////
    // Tree Producing Section:
    // Before we do the dp, we should first produce a tree for our dp, so that
    // we can avoid useless calculations.
    ////////////////////////////////////////////////////////////////////////////

    // The root of the tree.
    int root;

    // Find the root of the tree, any leaf is ok.
    int findRoot();

    // A temporary vector used to set the tree.
    std::vector<bool> has_set;
    // The fathers of the points, the parent of root is denoted as -1.
    std::vector<int> parent;
    // The adjacent table which stores the tree.
    std::vector<std::vector<int> > tree;

    // Organize the tree and save it as an adjacent table.
    void organizeTree(int /*parent*/);

    ////////////////////////////////////////////////////////////////////////////
    // Disperse Section:
    // Since the points are only 100, we can easily change the former graph into
    // a graph with at most 10000 points. Also it is safe when we input
    // arbitrarily large integers for coordinates.
    ////////////////////////////////////////////////////////////////////////////

    // The coordinates of the despersed points.
    std::vector<int> x_coord;
    std::vector<int> y_coord;

    // Desperse the coordinates.
    void desperseData();

    // The x and y member disp_points_ and disp_lines_ represents the xth or
    // yth member of x_coord and y_coord
    std::vector<Point> disp_points;

    ////////////////////////////////////////////////////////////////////////////
    // Painting Section: Used to enumerate the conditions and calculate the
    // number of overlaps.
    ////////////////////////////////////////////////////////////////////////////

    // Sets the lines for painting.
    std::map<Point, int, LMST::IsLessThanForPoint> hori_line;
    std::map<Point, int, LMST::IsLessThanForPoint> verti_line;

    // draw is the function that implements the dynamic programming process of
    // finding psi_l and psi_u by enumerating the kids of point label.
    void draw(int parent, std::vector<int> & /*kids*/, size_t /*num*/,
              int & /*value*/, int & /*result*/, int /*choice*/, int & /*decision*/);

    // "paint" the L-shaped line from point start to point finish through
    // point middle, on the hash map board_
    void paint(Point /*start*/, Point /*middle*/, bool /*direction*/,
               int color, int &/*value*/);

    // "Paints" from (u, y) to (v, y). u must not be greater than v.
    void paintHori(int /*u*/, int /*v*/, int /*y*/, int /*color*/, int &/*value*/);

    // "Paints" from (x, u) to (x, v). u must not be greater than v.
    void paintVerti(int /*u*/, int /*v*/, int /*x*/, int /*color*/, int &/*value*/);

    ////////////////////////////////////////////////////////////////////////////
    // Output Section: Output the results into vectors and files which is easier
    // to read.
    ////////////////////////////////////////////////////////////////////////////

    // The Line_L vector which stores the result of the LMST.
    std::vector<Line_L> m_result;

    // Output the results into the vector m_result
    void outputResultToVector();

    // A recursive function used by outputResultToVector
    void outputResultToVectorOfLabel(int /*label*/, bool /*choice*/);

public:
    void setPointsFromRST(RST *rst);

    void getResult(RST *rst);
};



#endif /* defined(__RectilinearSteinerTree__L_MST__) */
