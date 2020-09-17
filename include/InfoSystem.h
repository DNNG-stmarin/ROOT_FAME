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

//import from file

  // store the channel multiplcities
  int NUM_BEAMS;
  int NUM_DETS;
  int NUM_CHAMBERS;

  // store the channels
  int *BEAM;
  int *FISSION_CHAMBERS;
  int *DETECTORS;

  //(optional) broken detectors
  int *BROKENDETECTORS;
  int NUM_BROKEN;

  //calibration attributes
  TGraph *calibrationDet;
  TString calibrationPath;
  TString detectorPath;

  //input file
  int MIN_FILE;
  int NUM_FILES;
  int DATA_TYPE;
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

  bool DEBUG;
  bool PSD_ERG;

  TString experimentName;

  InfoSystem() {
    detectorPath = "";
    calibrationPath = "";
    experimentName = "";

    calibrationDet = NULL;

    NUM_BEAMS = 0;
    NUM_DETS = 0;
    NUM_CHAMBERS = 0;

    BEAM = NULL;
    FISSION_CHAMBERS =  NULL;
    DETECTORS = NULL;
    BROKENDETECTORS = NULL;

    MIN_FILE = 0;
    NUM_FILES = 0;
    DATA_TYPE = 0;
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

  InfoSystem ReadInput(Tstring inputFile) {
    ifstream file (to_string(inputFile));
    if(!file.is_open()) {
      cout << "Failed to open input file\n";
      exit(0);
    }
    //add experiment name

    string line;
    string tag, value;
    while(getline(file, line)) {
      istringstream iss(line);
      iss >> tag;
      if(tag == "<MIN_FILE>:") {
        iss >> value:
        MINFILE = stoi(x); //stoi(MIN_FILE); //not sure if this will work
      }
      if(line == "<NUM_FILES>:") {
        iss >> value;
        NUM_FILES = stoi(value); //not sure if this will work
      }
      if(line == "<DATA_TYPE>:") {
        iss >> value;
        DATA_TYPE = stoi(value);
      }
      if(line == "<DETECTOR_PATH>:") {
        iss >> tag >> ...
        TString(detectorPath); //not sure if this will work
      }
      if(line == "<CALIBRATION_PATH>:") {
        istringstream iss(line);
        iss >> tag >> TString(calibrationPath); //not sure
      }
      if(line == "<NUM_DETS>:") {
        istringstream iss(line);
        iss >> tag >> stoi(NUM_DETS);
      }
      if(line == "<DETECTORS>:") {
        istringstream iss(line);
        iss >> tag;
        for(int i=0; i<NUM_DETS; i++) {
          //iss >>
        }
      }
    }
  }

  // InfoSystem(int type)
  // {
  //   if(type == 0) // FS3_6 detector array
  //   {
  //     detectorPath = "expParameters/someFileFS3.txt";
  //     calibrationPath = "expParameters/intCalibrationFS3.txt"; //main.cc
  //     TString pathT = (TString)calibrationPath;
  //     ifstream fin(calibrationPath);
  //     if(!fin.is_open()) {
  //       cout << "Failed to open calibration file\n";
  //       exit(0);
  //     }
  //     //store calibration values in TGraph
  //     calibrationDet = new TGraph(pathT);
  //
  //     // experiment specific information
  //     NUM_BEAMS = 0;
  //     NUM_DETS = 6;
  //     NUM_CHAMBERS = 3;
  //     NUM_BROKEN = 0;
  //
  //     // location of fission chambers and detectors
  //     FISSION_CHAMBERS = new int[NUM_CHAMBERS];
  //     int tempfiss[] = {0, 16, 32};
  //     for(int i=0; i<NUM_CHAMBERS; i++) {
  //       FISSION_CHAMBERS[i] = tempfiss[i];
  //     }
  //
  //     BROKENDETECTORS = new int[NUM_BROKEN];
  //     int tempBroke[] = {};
  //     for(int i=0; i<NUM_BROKEN; i++) {
  //       BROKENDETECTORS[i] = tempBroke[i];
  //     }
  //
  //     DETECTORS = new int[NUM_DETS];
  //     int tempDet[] = {2, 10, 18, 26, 34, 42};
  //     for(int i=0; i<NUM_DETS; i++) {
  //       DETECTORS[i] = tempDet[i];
  //     }
  //   }
  //
  //   else if(type == 1) // ChiNu 242Pu
  //   {
  //     detectorPath = "expParameters/cartesian.txt";
  //     calibrationPath = "expParameters/intCalibrationCs.txt"; //main.cc
  //     TString pathT = (TString)calibrationPath;
  //     ifstream fin(calibrationPath);
  //     if(!fin.is_open()) {
  //       cout << "Failed to open calibration file\n";
  //       exit(0);
  //     }
  //     //store calibration values in TGraph
  //     calibrationDet = new TGraph(pathT);
  //
  //     // experiment specific information
  //     NUM_BEAMS = 0;
  //     NUM_CHAMBERS = 1;
  //     NUM_DETS = 54;
  //     NUM_BROKEN = 2;
  //
  //     // location of fission chambers and detectors
  //     FISSION_CHAMBERS = new int[NUM_CHAMBERS];
  //     int tempfiss[] = {7};
  //     for(int i=0; i<NUM_CHAMBERS; i++) {
  //       FISSION_CHAMBERS[i] = tempfiss[i];
  //     }
  //
  //     //broken detectors 45&46
  //     BROKENDETECTORS = new int[NUM_BROKEN];
  //     int tempBroke[] = {45, 46};
  //     for(int i=0; i<NUM_BROKEN; i++) {
  //       BROKENDETECTORS[i] = tempBroke[i];
  //     }
  //
  //     DETECTORS = new int[NUM_DETS];
  //     int tempDet[] = {9, 10, 11, 12, 13, 14, 15, 16, 25,
  //                     26, 27, 28, 29, 30, 31, 32, 41, 42,
  //     	            	43, 44, 45, 46, 47, 48, 49, 50, 51,
  //     	            	52, 53, 54, 55, 56, 57, 58, 59, 60,
  //     	            	61, 62, 63, 64, 65, 66, 67, 68, 69,
  //     	            	70, 71, 72, 73, 74, 75, 76, 77, 78};
  //
  //     for(int i=0; i<NUM_DETS; i++) {
  //       DETECTORS[i] = tempDet[i];
  //     }
  //   }
  //
  //   else //if(type == 2) // Stilbene setup
  //   {
  //     detectorPath = "expParameters/someFileFS3.txt";
  //     calibrationPath = "expParameters/intCalibrationSTIL.txt"; //main.cc
  //     TString pathT = (TString)calibrationPath;
  //     ifstream fin(calibrationPath);
  //     if(!fin.is_open()) {
  //       cout << "Failed to open calibration file\n";
  //       exit(0);
  //     }
  //     //store calibration values in TGraph
  //     calibrationDet = new TGraph(pathT);
  //
  //     // experiment specific information
  //     NUM_BEAMS = 0;
  //     NUM_DETS = 1;
  //     NUM_CHAMBERS = 1;
  //     NUM_BROKEN = 0;
  //
  //     // location of fission chambers and detectors
  //     FISSION_CHAMBERS = new int[NUM_CHAMBERS];
  //     int tempfiss[] = {0};
  //     for(int i=0; i<NUM_CHAMBERS; i++) {
  //       FISSION_CHAMBERS[i] = tempfiss[i];
  //     }
  //
  //     BROKENDETECTORS = new int[NUM_BROKEN];
  //     int tempBroke[] = {};
  //     for(int i=0; i<NUM_BROKEN; i++) {
  //       BROKENDETECTORS[i] = tempBroke[i];
  //     }
  //
  //     DETECTORS = new int[NUM_DETS];
  //     int tempDet[] = {10};
  //     for(int i=0; i<NUM_DETS; i++) {
  //       DETECTORS[i] = tempDet[i];
  //     }
  //   }
  // }

  ~InfoSystem()
  {
    delete BEAM;
    delete FISSION_CHAMBERS;
    delete DETECTORS;
    delete BROKENDETECTORS;
    delete calibrationDet;
  }
};

#endif
