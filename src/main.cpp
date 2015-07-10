#include "rst.h"
#include "rststrategy.h"

int main(int argc, char const *argv[]) {
    RST* rst;
    // if (!rst->getStrategy())
    // {
    //     QMessageBox msgBox;
    //     msgBox.setText("Error: No Algorithm Selected. ");
    //     msgBox.exec();
    //     return 0;
    // }
    // if (!rst->hasData())
    // {
    //     QMessageBox msgBox;
    //     msgBox.setText("Error: No Input Data. ");
    //     msgBox.exec();
    //     return 0;
    // }

    rst->changeStrategy(1);
    rst->toSolveRST();
    // delete imp;
    return 0;
}
