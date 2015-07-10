/// Project name: New Algorithms for the Rectilinear Steiner Tree Problem
/// Sub-project: LMST  
//  main.cpp
//  
//
//  Created by Tony Soong on 2013/6/9
//  Modified by Tony Soong on 2013/6/12
//  Copyright (c) Tony Soong(Jiaming Song) All rights reserved.
//
//	Main function control entrance.
//  
#include "../SMST/SMST.h"
#include "LMST.h"
#include <string>
#include <cassert>
// Command Line Control Added: 
// [program name] [input file name] [output file name]
int main(int argc, char *argv[]){
	assert(argc <= 3);
	std::string Input = (argc >= 2)? argv[1]: "input.txt";
	std::string Output = (argc >= 3)? argv[2]: "output.txt";
	PointsGenerator pg;
	LMST lmst(Input.c_str());
	lmst.Output(Output.c_str());
	return 0;
}
