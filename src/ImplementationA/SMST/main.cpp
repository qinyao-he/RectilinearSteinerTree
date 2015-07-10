#include "SMST.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){
    imp_a::PointsGenerator pg;
    imp_a::SMST smst;
	smst.setPointsFromFile("input.txt");
	smst.calculateMST();
	smst.printMSTToFile("output.txt");

	return 0;
}

