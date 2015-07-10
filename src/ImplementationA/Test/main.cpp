//
//  main.cpp
//  RectilinearSteinerTree
//
//  Created by Yue Shichao on 25/6/13.
//
//  Test the results given by our softwares

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include "../SMST/PointsGenerator.h"
#include "../SMST/SMST.h"
#include "../LMST/LMST.h"
#include "../ZMST/ZMST.h"
#include "../Headers/overlap.h"
#include "check.h"

using std::vector;
using std::string;
using std::endl;

int main(int argc, char *argv[]) {
    PointsGenerator point_gen;
    vector<double> MST_result;
    vector<int> LMST_result;
    vector<int> ZMST_result;
    string test_data = "test_data_x.txt";
    string test_MST = "test_MST_x.txt";
    string test_LMST = "test_LMST_x.txt";
    string test_ZMST = "test_ZMST_x.txt";
    string folder = "testx";
    SMST smst;
    ZMST zmst;
    int count = 5;
    if (argc >= 2) {
        count = atoi(argv[1]);
    }
    for (int i = 0; i < count; ++i) {
        test_data[10] = i + '0';
        test_MST[9] = i + '0';
        test_LMST[10] = i + '0';
        test_ZMST[10] = i + '0';
        folder[4] = i + '0';
        //creat a new folder
        string code = "mkdir " + folder;
        system(code.c_str());
        string data = folder + "/" + test_data;
        string MST = folder + "/" + test_MST;
        string LMST = folder + "/" + test_LMST;
        string ZMST = folder + "/" + test_ZMST;
        point_gen.printPointsToFile(data.c_str(), 20 * (i + 1), 1000);

        //check MST
        smst.setPointsFromFile(data.c_str());
        smst.calculateMST();
        smst.printMSTToFile(MST.c_str());
        assert(Check::crossing(smst.points(), smst.lines()));
        MST_result.push_back(Overlap::overlap(smst.points(), smst.lines()));

        //check LMST
        class LMST lmst(data.c_str());
        lmst.Output(LMST.c_str());
        assert(Check::crossing(lmst.points(), lmst.result()));
        LMST_result.push_back(Overlap::overlap(lmst.points(), lmst.result()));

        //check ZMST
        zmst.setPointsFromFile(data.c_str());
        zmst.exec();
        zmst.print(ZMST.c_str());
        assert(Check::crossing(zmst.points(), zmst.lines()));
        ZMST_result.push_back(Overlap::overlap(zmst.points(), zmst.lines()));
    }
    //output all the results into a single file
    std::ofstream fout("result.txt");
    for (int i = 0; i < count; ++i) {
        fout << "MST: " << MST_result[i] << endl;
        fout << "LMST: " << LMST_result[i] << endl;
        fout << "ZMST: " << ZMST_result[i] << endl;
        fout << "================================" << endl;
    }
    fout.close();
    return 0;
}
