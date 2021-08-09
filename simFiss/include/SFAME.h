// this class takes in simulated data and creates trees to be analyzed using root fame
// inputs
// Stefano, Kate August 4th, 2021

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TMatrixD.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <THStack.h>

#include <iostream>
#include <fstream>
// Header file for the classes stored in the TTree if any.

#include <string.h>
#include "Constants.h"
#include "DetectorClass.h"

using namespace std;

#ifndef sfame_h
#define sfame_h

class sfame {
public:
    sfame();

    // readfromCol vars
    TString nameColFile;

    // read from sim vars
    TString simFile;
    int firstFile;
    int numFiles;

    //
    int NUM_DETECTORS;
    double SOURCE_POSITION[3];

    // external files
    TString DET_DIST_FILE; // detector distances
    TString TIME_RESOLUTION_FILE; // time resolution
    TString CELL_NUMBERS_FILE; // cell numbers
    TString LIGHTOUTPUT_FILE; // conversion between energyDep and lightoutput
    double RESOLUTION_COEFFICIENTS;
    // energy file

    // supplemental files
    TString dEdX_FILE; // birks stopping power
    TString DET_BIRKCOEFF_FILE; // birks coeffs
    //
    TGraph* chanToCellList;
    TGraph* cellToChanList;
    TGraph* posXList;
    TGraph* posYList;
    TGraph* posZList;
    TGraph* timeResList;

    TGraph** LightOutList;
    TGraph** KinLightList;

    // initialization functions
    void initializeDetectors();
    void initializeInputFiles();

    // functions
    void readFromCol();
    void readFromSimFile(TString nameSim, int firstFile, int numFiles);


    // lightoutput functions
    int cellToChannel(int cell);
    int chanToCell(int chan);
    void lightOutput();

    double birks(int channel, double energy, int typeP, int zaid);
    double findBroad(double erg);
    int cutLightOut(double height, int typeP);
    int cutTimePsd(double times, int typeP);

    // array of the detectors
    DetectorClass* detectors;

    ~sfame();
};

#endif
