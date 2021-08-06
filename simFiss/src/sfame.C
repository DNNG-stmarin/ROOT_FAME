//
//

#include "SFAME.h"
#include "TString.h"

sfame::sfame()
{
    nameColFile = "FS3_openV5_dist_270.d";
    simFile = "sim27";
    firstFile = 0;
    numFiles = 1;
    

    // external lightoutput files 
    DET_DIST_FILE = "FS3_closeDists.csv";
    TIME_RESOLUTION_FILE = "tResFile.csv";
    CELL_NUMBERS_FILE = "cellNumbers.csv";
    LIGHTOUTPUT_FILE = "birksLO.csv";
    RESOLUTION_COEFFICIENTS = [10.61, 0.355, 3.923];
    // energy resolution file 

    // supplamental files
    dEdX_FILE = "stilbene_dEdX.csv";
    DET_BIRKCOEFF_FILE = "BirkCoeff.csv";

}
