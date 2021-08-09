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
    //
    NUM_DETECTORS = 40;
    SOURCE_POSITION[0] = 0; SOURCE_POSITION[1] = 0; SOURCE_POSITION[2] = -2;

    // external lightoutput files
    DET_DIST_FILE = "inputSfame/cartesianFS3.txt";
    TIME_RESOLUTION_FILE = "inputSfame/tResFile.csv";
    CELL_NUMBERS_FILE = "inputSfame/cellNumbers.csv";
    LIGHTOUTPUT_FILE = "inputSfame/birksLO.csv";
    //RESOLUTION_COEFFICIENTS = [10.61, 0.355, 3.923];
    // energy resolution file

    // supplamental files
    dEdX_FILE = "inputSfame/stilbene_dEdX.csv";
    DET_BIRKCOEFF_FILE = "inputSfame/BirkCoeff.csv";

    initializeInputFiles();
    initializeDetectors();

}

sfame::~sfame()
{
    delete detectors;
}
