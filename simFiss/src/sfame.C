//
//

#include "SFAME.h"
#include "TString.h"

sfame::sfame()
{
    // main output
    NameOutput = "CoincidenceTrees.root";

    // read from sim
    simFile = "sim27";

    // read from collision files
    // nameColFile = "cn";
    firstFile = 0;
    numFiles = 1;

    // read from colliosn tree
    nameCollTree = "collTree.root";
    REUSE_DATA = 1;
    if(REUSE_DATA == 1) numCollTreeFiles = 1;

    //
    NUM_DETECTORS = 54;
    TRIGGER_CHANNEL = 46;
    SOURCE_POSITION[0] = 0; SOURCE_POSITION[1] = 0; SOURCE_POSITION[2] = 0;

    ACCEPT_PSP = 0.1;
    ACCEPT_TRIG = 0.01; // V us
    ACCEPT_TYPE = SPONTANEOUS; // Spontaneous fission (?)


    THRESHOLD = 0.01; // MeVee
    COINCIDENCE_WINDOW = 200; // ns
    PULSE_GENERATION_WINDOW = 10; // ns

    // external lightoutput files
    DET_DIST_FILE = "../../rootSel/inputSfame/cartesianFS3.txt";        //Converted to Chi-Nu
    TIME_RESOLUTION_FILE = "../../rootSel/inputSfame/tResFile_FS3.csv"; //Converted to Chi-Nu
    CELL_NUMBERS_FILE = "../../rootSel/inputSfame/cellNumbersFS3.csv";       //Converted to Chi-Nu
    LIGHTOUTPUT_FILE = "../../rootSel/inputSfame/BirksLO.csv";          //Converted to Chi-Nu
    RESOLUTION_COEFFICIENTS[0] = 10.61; RESOLUTION_COEFFICIENTS[1] = 0.355; RESOLUTION_COEFFICIENTS[2] = 3.923;
    // * could be an energy resolution file
    MEAN_NEUTRON_PSD_FILE = "../../rootSel/inputSfame/meanNeutParamsFS3";//Converted to Chi-Nu
    SIGMA_NEUTRON_PSD_FILE = "../../rootSel/inputSfame/sigNeutParamsFS3";//Converted to Chi-Nu
    MEAN_PHOTON_PSD_FILE = "../../rootSel/inputSfame/meanPhotParamsFS3";//Converted to Chi-Nu
    SIGMA_PHOTON_PSD_FILE = "../../rootSel/inputSfame/sigPhotParamsFS3";//Converted to Chi-Nu

    // supplamental files
    dEdX_FILE = "../../rootSel/inputSfame/stilbene_dEdX.csv";
    DET_BIRKCOEFF_FILE = "../../rootSel/inputSfame/BirkCoeff.csv";

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
