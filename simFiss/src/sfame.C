//
//

#include "SFAME.h"
#include "TString.h"

sfame::sfame()
{
    // main output
    NameOutput = "SimFis.root";

    // read from sim
    nameColFile = "FS3_openV5_dist_270.d";
    simFile = "sim27";
    firstFile = 0;
    numFiles = 1;

    //
    NUM_DETECTORS = 40;
    TRIGGER_CHANNEL = 46;
    SOURCE_POSITION[0] = 0; SOURCE_POSITION[1] = 0; SOURCE_POSITION[2] = -2;

    ACCEPT_PSP = 0.1;
    ACCEPT_TRIG = 0.01; // V us

    THRESHOLD = 0.01; // MeVee
    COINCIDENCE_WINDOW = 200; // ns
    PULSE_GENERATION_WINDOW = 10; // ns

    // external lightoutput files
    DET_DIST_FILE = "inputSfame/cartesianFS3.txt";
    TIME_RESOLUTION_FILE = "inputSfame/tResFile_FS3.csv";
    CELL_NUMBERS_FILE = "inputSfame/cellNumbers.csv";
    LIGHTOUTPUT_FILE = "inputSfame/birksLO.csv";
    RESOLUTION_COEFFICIENTS[0] = 10.61; RESOLUTION_COEFFICIENTS[1] = 0.355; RESOLUTION_COEFFICIENTS[2] = 3.923;
    // * could be an energy resolution file
    MEAN_NEUTRON_PSD_FILE = "inputSfame/meanNeutParams";
    SIGMA_NEUTRON_PSD_FILE = "inputSfame/sigNeutParams";
    MEAN_PHOTON_PSD_FILE = "inputSfame/meanPhotParams";
    SIGMA_PHOTON_PSD_FILE = "inputSfame/sigPhotParams";

    // supplamental files
    dEdX_FILE = "inputSfame/stilbene_dEdX.csv";
    DET_BIRKCOEFF_FILE = "inputSfame/BirkCoeff.csv";

    initializeInputFiles();
    initializeDetectors();
    initializeCollisionTree();
    initializeCoincTree();
}

sfame::~sfame()
{
    cout << "Cleaning up " << endl;

    delete detectors;
    delete collisionTree->GetCurrentFile();
    delete coincTree->GetCurrentFile();
    delete SfameFiss;

    delete chanToCellList;
    delete cellToChanList;
    delete posXList;
    delete posYList;
    delete posZList;
    delete timeResList;

    delete LightOutList;
    delete KinLightList;

    delete meanNeutPSDArray;
    delete meanPhotPSDArray;
    delete sigNeutPSDArray;
    delete sigPhotPSDArray;
}
