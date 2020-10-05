// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystem_h
#define InfoSystem_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TGraph.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

class InfoSystem
{
private:

public:

  // store the channel multiplcities
  int NUM_BEAMS;
  int NUM_DETS;
  int NUM_CHAMBERS;

  // store the channels
  int *BEAM;
  int *DETECTORS;
  int *FISSION_CHAMBERS;

  //(optional) broken detectors
  int *BROKEN_DETECTORS;
  int NUM_BROKEN;

  //calibration attributes
  TGraph *calibrationDet;
  TString calibrationPath;
  TString detectorPath;
  TString nameOfExp;

  //input file
  int MIN_FILE;
  int NUM_FILES;
  int DATA_TYPE;
  int REUSE_DATA;
  double COINC_WINDOW;
  double DETECTOR_THRESHOLD;
  double CHAMBER_THRESHOLD;
  double CHAMBER_CLIP;
  double MAX_CHAMBER_DRIFT;
  double MIN_TIME_P;
  double MAX_TIME_P;
  double MIN_TIME_N;
  double MAX_TIME_N;
  double DELTA_BACK_SIG;

  int DEBUG;
  int PSD_ERG;

  InfoSystem() {
    detectorPath = "";
    calibrationPath = "";
    nameOfExp = "";

    calibrationDet = NULL;

    NUM_BEAMS = 0;
    NUM_DETS = 0;
    NUM_CHAMBERS = 0;

    BEAM = NULL;
    FISSION_CHAMBERS =  NULL;
    DETECTORS = NULL;
    BROKEN_DETECTORS = NULL;

    MIN_FILE = 0;
    NUM_FILES = 0;
    DATA_TYPE = 0;
    REUSE_DATA = 0;
    COINC_WINDOW = 0.0;
    DETECTOR_THRESHOLD = 0.0;
    CHAMBER_THRESHOLD = 0.0;
    CHAMBER_CLIP = 0.0;
    MAX_CHAMBER_DRIFT = 0.0;
    MIN_TIME_P = 0.0;
    MAX_TIME_P = 0.0;
    MIN_TIME_N = 0.0;
    MAX_TIME_N = 0.0;
    DELTA_BACK_SIG = 0.0;
  }

  void ReadInput(TString inputFile) {
    string inputFileString = string(inputFile);
    ifstream file (inputFileString);

    if(!file.is_open()) {
      cout << "Failed to open input file\n";
      exit(0);
    }

    string line;
    string tag, value;
    while(file >> tag) {
      if(tag == "<NAME>:") {
        file >> value;
        nameOfExp = TString(value);
      }
      if(tag == "<MIN_FILE>:") {
        file >> value;
        MIN_FILE = stoi(value);
      }
      else if(tag == "<NUM_FILES>:") {
        file >> value;
        NUM_FILES = stoi(value);
      }
      else if(tag == "<DATA_TYPE>:") {
        file >> value;
        DATA_TYPE = stoi(value);
      }
      else if(tag == "<REUSE_DATA>:") {
        file >> value;
        REUSE_DATA = stoi(value);
      }
      else if(tag == "<DETECTOR_PATH>:") {
        file >> value;
        detectorPath = TString(value);
      }
      else if(tag == "<CALIBRATION_PATH>:") {
        file >> value;
        calibrationPath = TString(value);
      }
      else if(tag == "<NUM_BEAMS>:") {
        file >> value;
        NUM_BEAMS = stoi(value);
      }
      else if(tag == "<BEAMS>:") {
        BEAM = new int[NUM_BEAMS];
        for(int i=0; i<NUM_BEAMS; i++) {
          file >> value;
          BEAM[i] = stoi(value);
        }
      }
      else if(tag == "<NUM_DETS>:") {
        file >> value;
        NUM_DETS = stoi(value);
      }
      else if(tag == "<DETECTORS>:") {
        DETECTORS = new int[NUM_DETS];
        for(int i=0; i<NUM_DETS; i++) {
          file >> value;
          DETECTORS[i] = stoi(value);
        }
      }
      else if(tag == "<NUM_CHAMBERS>:") {
        file >> value;
        NUM_CHAMBERS = stoi(value);
      }
      else if(tag == "<CHAMBERS>:") {
        FISSION_CHAMBERS = new int[NUM_CHAMBERS];
        for(int i=0; i<NUM_CHAMBERS; i++) {
          file >> value;
          FISSION_CHAMBERS[i] = stoi(value);
        }
      }
      else if(tag == "<NUM_BROKEN>:") {
        file >> value;
        NUM_BROKEN = stoi(value);
      }
      else if(tag == "<BROKEN_DETECTORS>:") {
        BROKEN_DETECTORS = new int[NUM_BROKEN];
        for(int i=0; i<NUM_BROKEN; i++) {
          file >> value;
          BROKEN_DETECTORS[i] = stoi(value);
        }
      }
      else if(tag == "<COINC_WINDOW>:") {
        file >> value;
        COINC_WINDOW = stod(value);
      }
      else if(tag == "<DETECTOR_THRESHOLD>:") {
        file >> value;
        DETECTOR_THRESHOLD = stod(value);
      }
      else if(tag == "<CHAMBER_THRESHOLD>:") {
        file >> value;
        CHAMBER_THRESHOLD = stod(value);
      }
      else if(tag == "<CHAMBER_CLIP>:") {
        file >> value;
        CHAMBER_CLIP = stod(value);
      }
      else if(tag == "<MAX_CHAMBER_DRIFT>:") {
        file >> value;
        MAX_CHAMBER_DRIFT = stod(value);
      }
      else if(tag == "<MIN_TIME_P>:") {
        file >> value;
        MIN_TIME_P = stod(value);
      }
      else if(tag == "<MAX_TIME_P>:") {
        file >> value;
        MAX_TIME_P = stod(value);
      }
      else if(tag == "<MIN_TIME_N>:") {
        file >> value;
        MIN_TIME_N = stod(value);
      }
      else if(tag == "<MAX_TIME_N>:") {
        file >> value;
        MAX_TIME_N = stod(value);
      }
      else if(tag == "<DELTA_BACK_SIG>:") {
        file >> value;
        DELTA_BACK_SIG = stod(value);
      }
      else if(tag == "<DEBUG>:") {
        file >> value;
        DEBUG = stoi(value);
      }
      else if(tag == "<PSD_ERG>:") {
        file >> value;
        PSD_ERG = stoi(value);
      }
    }

    TString pathT = (TString)calibrationPath;
    ifstream fin(calibrationPath);
    if(!fin.is_open()){
      cout << "Failed to open calibration file\n";
    }
    calibrationDet = new TGraph(pathT);
  }

  ~InfoSystem()
  {
    delete BEAM;
    delete FISSION_CHAMBERS;
    delete DETECTORS;
    delete BROKEN_DETECTORS;
    delete calibrationDet;
  }
};

#endif
