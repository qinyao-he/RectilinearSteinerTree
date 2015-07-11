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
    void init();

    // Returns the result.
    int Result();

    // Returns the vector of m_vertexs.
    std::vector<Point> &points() {
        return points_;
    }

    // Returns the vector of m_lines.
    std::vector<Line> &lines() {
        return lines_;
    }

    // Returns the vector of result_.
    std::vector<Line_L> &result() {
        return result_;
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
    MST smst_;
    // The points and lines of the MST given.
    std::vector<Point> points_;
    std::vector<Line> lines_;


    ////////////////////////////////////////////////////////////////////////////
    // Algorithm Control Section: Controls the flow of the core algorithm.
    ////////////////////////////////////////////////////////////////////////////

    // Find the Psi of all the edges.
    void FindPsi();

    // psi_l_ and psi_u_ are denoted just as the paper describes.
    // We use FindPsiL and FindPsiU to find the psi_l_ and psi_u_,
    // and save the choices(L-shape or U-shape?) in result_for_l_.
    std::vector<int> psi_l_;
    std::vector<int> psi_u_;
    int psi_result_;
    // choice_l_ and choice_u_ denotes the best method of l or u, in binary
    // notation. 1 means true and 0 means false
    std::vector<int> choice_l_;
    std::vector<int> choice_u_;

    // Note that if we already have calculated the edge, we don't need to
    // calculate it again. Instead we put them into psi_l_ and psi_u_.
    int FindPsiL(int /*label*/);

    int FindPsiU(int /*label*/);

    ////////////////////////////////////////////////////////////////////////////
    // Tree Producing Section:
    // Before we do the dp, we should first produce a tree for our dp, so that
    // we can avoid useless calculations.
    ////////////////////////////////////////////////////////////////////////////

    // The root of the tree.
    int root_;

    // Find the root of the tree, any leaf is ok.
    void FindRoot();

    // A temporary vector used to set the tree.
    std::vector<bool> has_set_;
    // The fathers of the points, the father of root_ is denoted as -1.
    std::vector<int> father_;
    // The adjacent table which stores the tree.
    std::vector<std::vector<int> > tree_;

    // Organize the tree and save it as an adjacent table.
    void OrganizeTree(int /*father*/);

    ////////////////////////////////////////////////////////////////////////////
    // Disperse Section:
    // Since the points are only 100, we can easily change the former graph into
    // a graph with at most 10000 points. Also it is safe when we input
    // arbitrarily large integers for coordinates.
    ////////////////////////////////////////////////////////////////////////////

    // The coordinates of the despersed points.
    std::vector<int> x_coord_;
    std::vector<int> y_coord_;

    // Desperse the coordinates.
    void DesperseData();

    // The x and y member disp_points_ and disp_lines_ represents the xth or
    // yth member of x_coord_ and y_coord
    std::vector<Point> disp_points_;

    ////////////////////////////////////////////////////////////////////////////
    // Painting Section: Used to enumerate the conditions and calculate the
    // number of overlaps.
    ////////////////////////////////////////////////////////////////////////////

    // Sets the lines for painting.
    std::map<Point, int, LMST::IsLessThanForPoint> hori_line_;
    std::map<Point, int, LMST::IsLessThanForPoint> verti_line_;

    // Draw is the function that implements the dynamic programming process of
    // finding psi_l_ and psi_u_ by enumerating the kids of point label.
    void Draw(int parent, std::vector<int> & /*kids*/, size_t /*num*/,
              int & /*value*/, int & /*result*/, int /*choice*/, int & /*decision*/);

    // "Paint" the L-shaped line from point start to point finish through
    // point middle, on the hash map board_
    void Paint(Point /*start*/, Point /*middle*/, bool /*direction*/,
               int color, int &/*value*/);

    // "Paints" from (u, y) to (v, y). u must not be greater than v.
    void PaintHori(int /*u*/, int /*v*/, int /*y*/, int /*color*/, int &/*value*/);

    // "Paints" from (x, u) to (x, v). u must not be greater than v.
    void PaintVerti(int /*u*/, int /*v*/, int /*x*/, int /*color*/, int &/*value*/);

    ////////////////////////////////////////////////////////////////////////////
    // Output Section: Output the results into vectors and files which is easier
    // to read.
    ////////////////////////////////////////////////////////////////////////////

    // The Line_L vector which stores the result of the LMST.
    std::vector<Line_L> result_;

    // Output the results into the vector result_
    void OutputResultToVector();

    // A recursive function used by OutputResultToVector
    void OutputResultToVectorOfLabel(int /*label*/, bool /*choice*/);

public:
    void setPointsFromRST(RST *rst);

    void getResult(RST *rst);
};



#endif /* defined(__RectilinearSteinerTree__L_MST__) */
