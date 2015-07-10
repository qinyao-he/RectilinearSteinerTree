//
//  ZMST.cpp
//  RectilinearSteinerTree
//
//  Author : Wang_Tianyi
//  Implementation of ZMST.
//

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "ZMST.h"
#include "../Headers/overlap.h"
#include "../../rst.h"
#include <QDebug>

namespace imp_a{

using std::abs;
using std::copy;
using std::lower_bound;
using std::max;
using std::min;
using std::sort;
using std::time;
using std::unique;
using std::vector;
//This function should have been provided in class Points, but there isn't one.
inline bool operator == (const Point& a, const Point& b) {
  return a.x() == b.x() && a.y() == b.y();
}
//Compare the line by the start node.
inline bool lineCmp (const Line_Z& a, const Line_Z& b) {
  return a.start() < b.start();
}
//The Manhattan distance of two points.
inline int dist (const Point& a, const Point& b) {
  return abs(a.x() - b.x()) + abs(a.y() - b.y());
}
//SubProcedure for exec(). It would examine all the layouts for the sons of a
//node and get the best one.layout::subAns and layout::bestLay should be
//filled after this procedure.
void ZMST::dfs(int root, int father, int stat, layout& lay,
               const vector<Line_Z>& lines, vector<vector<layout> >& subProb,
               const int* head, int* stack) {
  using Overlap::overlap;
  //current node is a leaf
  if (head[root] == head[root + 1]) {
    lay.subAns = 0;
    return;
  }
  int id = stat - head[root], son = lines[stat].end();
  //A node has no more than 6 childs
  assert(id < 6);
  if (stat == head[root + 1]) {
    int ans = 0;
    vector<Line_Z> overLap;
    for (int i = 0; i < id; i ++) {
      ans += subProb[son = lines[head[root] + i].end()][stack[i]].subAns;
      overLap.push_back(Line_Z(root, son, subProb[son][stack[i]].midPoint));
    }
    overLap.push_back(Line_Z(father, root, lay.midPoint));
    //current answer exclude the edge upon the node itself
    ans += overlap(points(), overLap) - dist(point(root), point(father));
    if (ans < lay.subAns) {
      lay.subAns = ans;
      copy(stack, stack + id, lay.bestLay);
    }
  } else {
    for (int i = 0; (stack[id] = i) < subProb[son].size(); i ++) {
      dfs(root, father, stat + 1, lay, lines, subProb, head, stack);
    }
  }
}

void ZMST::exec() {
  smst.calculateMST();
  //reserve before mass data pushing back
  lines_.reserve(smst.lines().size());
  for (int i = 0; i < smst.lines().size(); i ++)
    lines_.push_back(Line_Z(smst.lines()[i].start(), smst.lines()[i].end(),
                            Point()));
  //lines is a sorted line vector, while smst.lines_ is in father-child order
  sort(lines_.begin(), lines_.end(), lineCmp);
  //make the positive direction table
  int* head = new int[points().size() + 1];
  for (int i = 0, j = 0; i <= points().size() && j <= lines().size(); ) {
    if (j == lines().size() || line(j).start() != i - 1)
      head[i ++] = j;
    else
      j ++;
  }
  //Discretization
  int* father = new int[points().size()],
     * xgrids = new int[points().size()],
     * ygrids = new int[points().size()],
     xgridsize, ygridsize;
  for (int i = 0; i < lines_.size(); i ++) {
    father[lines_[i].end()] = lines_[i].start();
  }
  father[0] = -1;
  for (int i = 0; i < points().size(); i ++) {
    xgrids[i] = point(i).x();
    ygrids[i] = point(i).y();
  }
  sort (xgrids, xgrids + points().size());
  sort (ygrids, ygrids + points().size());
  xgridsize = unique(xgrids, xgrids + points().size()) - xgrids;
  ygridsize = unique(ygrids, ygrids + points().size()) - ygrids;
  vector<vector<layout> > subProb(vector<vector<layout> >(points().size(),
                                                          vector<layout>()));
  //Enumerate all layouts
  for (vector<Line_Z>::iterator it = lines_.begin();
       it != lines_.end(); ++ it) {
    int minx = min(point(it->start()).x(), point(it->end()).x()),
        maxx = max(point(it->start()).x(), point(it->end()).x()),
        miny = min(point(it->start()).y(), point(it->end()).y()),
        maxy = max(point(it->start()).y(), point(it->end()).y());
    int minxid = lower_bound(xgrids, xgrids + xgridsize, minx) - xgrids,
        maxxid = lower_bound(xgrids, xgrids + xgridsize, maxx) - xgrids,
        minyid = lower_bound(ygrids, ygrids + ygridsize, miny) - ygrids,
        maxyid = lower_bound(ygrids, ygrids + ygridsize, maxy) - ygrids;
    for (int i = minxid; i <= maxxid; i ++) {
      subProb[it->end()].push_back(Point(xgrids[i], point(it->start()).y()));
    }
    for (int i = minyid; i <= maxyid; i ++) {
      subProb[it->end()].push_back(Point(point(it->start()).x(), ygrids[i]));
    }
  }
  //Caculate from leaves to root
  int stack[6];
  for (vector<Line>::const_reverse_iterator it = smst.lines().rbegin();
       it != smst.lines().rend(); ++ it) {
    for (vector<layout>::iterator lit = subProb[it->end()].begin();
         lit != subProb[it->end()].end(); ++ lit) {
      dfs(it->end(), father[it->end()], head[it->end()], *lit, lines(),
          subProb, head, stack);
    }
  }
  subProb[0].push_back(point(0));
  dfs(0, 0, head[0], subProb[0][0], lines(), subProb, head, stack);
  getAns(0, subProb[0][0],subProb, head);
  delete []head;
  delete []xgrids;
  delete []ygrids;
  delete []father;
}
//SubProcedure for exec(). It would examine the subProb and look for
//best solutions for each node.
void ZMST::getAns(int root, const layout& lay,
                  const vector<vector<layout> > subProb,
                  const int* head) {
  for (int i = head[root]; i < head[root + 1]; i ++) {
    lines_[i].mid_point_set() =
      subProb[line(i).end()][lay.bestLay[i - head[root]]].midPoint;
    getAns(line(i).end(), subProb[line(i).end()][lay.bestLay[i - head[root]]],
           subProb, head);
  }
}
// print the result to a file
void ZMST::print(const char* fileName) const {
  FILE* out = fopen(fileName, "w");
  print(out);
  fclose(out);
}
// print the result to a FILE*
void ZMST::print(FILE* out) const{
  fprintf(out, "%lu %lu\n", points().size(), lines().size());
  for (vector<Point>::const_iterator it = points().begin();
       it != points().end(); ++ it) {
    fprintf(out, "%d %d\n", it->x(), it->y());
  }
  for (vector<Line_Z>::const_iterator it = lines().begin();
       it != lines().end(); ++ it) {
    fprintf(out, "Z %d %d %d %d\n", it->start(), it->end(),
            it->mid_point().x(), it->mid_point().y());
  }
}
//for debug use. generates data randomly, with n=100 by default
void ZMST::setPointsByRandom(int num, int maxRange) {
  srand(std::time(0));
  smst.setPointsByRandom();
}

//read data from a file
void ZMST::setPointsFromFile(const char* fileName) {
  lines_.clear();
  smst.setPointsFromFile(fileName);
}

void ZMST::setPointsFromRST(RST *rst)
{
    lines_.clear();
    //qDebug("get points for ZMST");
    smst.setPointsFromRST(rst);
}

void ZMST::getResult(RST *rst)
{
    //qDebug("ZMST end");
    rst->v_seg.clear();
    for (int i = 0; i < lines_.size(); i++)
    {
        Point2D A = mkPoint(smst.points()[lines_[i].start()].x(), smst.points()[lines_[i].start()].y());
        Point2D B = mkPoint(smst.points()[lines_[i].end()].x(), smst.points()[lines_[i].end()].y());
        Point2D C = mkPoint(lines_[i].mid_point().x(), lines_[i].mid_point().y());
        Point2D D; // A - C -(default:vertical when A==C) D - B
        if (std::abs(A.y - C.y) < 0.1)
            D = mkPoint(C.x, B.y);
        else
            D = mkPoint(B.x, C.y);
        rst->v_seg.push_back(mkSegment(A, C));
        rst->v_seg.push_back(mkSegment(C, D));
        rst->v_seg.push_back(mkSegment(D, B));
    }
    //qDebug("get result with %d segs", rst->v_seg.size());
}
}
