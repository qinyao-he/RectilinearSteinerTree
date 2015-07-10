//
//  main.cpp
//  RectilinearSteinerTree
//
//  Z_MST
//  
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "ZMST.h"
/* Author : Wang_Tianyi
  * zmst main procedure. If no argument is given, then generate data randomly
  * for debug use. If one argument is given then read data from the file and
  * give result to "out.txt".
  * If two argument is given then read data from the first file and give the
  * result to the second one.
  */
int main(int argc, const char* argv[]) {
  ZMST zmst;
  if (argc >= 2)
    zmst.setPointsFromFile(argv[1]);
  else zmst.setPointsByRandom();
  zmst.exec();
  zmst.print(argc == 3 ? argv[3] : "out.txt");
  return 0;
}
