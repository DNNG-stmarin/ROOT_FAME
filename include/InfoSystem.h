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
  string calibrationPath;
  string detectorPath;

  InfoSystem(int type)
  {
    if(type == 0) // FS3_6 detector array
    {
      detectorPath = "expParameters/someFileFS3.txt";
      calibrationPath = "expParameters/intCalibrationFS3.txt"; //main.cc
      TString pathT = (TString)calibrationPath;
      ifstream fin(calibrationPath);
      if(!fin.is_open()) {
        cout << "Failed to open calibration file\n";
        exit(0);
      }
      //store calibration values in TGraph
      calibrationDet = new TGraph(pathT);

      // experiment specific information
      NUM_BEAMS = 0;
      NUM_DETS = 6;
      NUM_CHAMBERS = 3;
      NUM_BROKEN = 0;

      // location of fission chambers and detectors
      FISSION_CHAMBERS = new int[NUM_CHAMBERS];
      int tempfiss[] = {0, 16, 32};
      for(int i=0; i<NUM_CHAMBERS; i++) {
        FISSION_CHAMBERS[i] = tempfiss[i];
      }

      BROKENDETECTORS = new int[NUM_BROKEN];
      int tempBroke[] = {};
      for(int i=0; i<NUM_BROKEN; i++) {
        BROKENDETECTORS[i] = tempBroke[i];
      }

      DETECTORS = new int[NUM_DETS];
      int tempDet[] = {2, 10, 18, 26, 34, 42};
      for(int i=0; i<NUM_DETS; i++) {
        DETECTORS[i] = tempDet[i];
      }
    }

    else if(type == 1) // ChiNu 242Pu
    {
      detectorPath = "expParameters/cartesian.txt";
      calibrationPath = "expParameters/intCalibrationCs.txt"; //main.cc
      TString pathT = (TString)calibrationPath;
      ifstream fin(calibrationPath);
      if(!fin.is_open()) {
        cout << "Failed to open calibration file\n";
        exit(0);
      }
      //store calibration values in TGraph
      calibrationDet = new TGraph(pathT);

      // experiment specific information
      NUM_BEAMS = 0;
      NUM_CHAMBERS = 1;
      NUM_DETS = 54;
      NUM_BROKEN = 2;

      // location of fission chambers and detectors
      FISSION_CHAMBERS = new int[NUM_CHAMBERS];
      int tempfiss[] = {7};
      for(int i=0; i<NUM_CHAMBERS; i++) {
        FISSION_CHAMBERS[i] = tempfiss[i];
      }

      //broken detectors 45&46
      BROKENDETECTORS = new int[NUM_BROKEN];
      int tempBroke[] = {45, 46};
      for(int i=0; i<NUM_BROKEN; i++) {
        BROKENDETECTORS[i] = tempBroke[i];
      }

      DETECTORS = new int[NUM_DETS];
      int tempDet[] = {9, 10, 11, 12, 13, 14, 15, 16, 25,
                      26, 27, 28, 29, 30, 31, 32, 41, 42,
      	            	43, 44, 45, 46, 47, 48, 49, 50, 51,
      	            	52, 53, 54, 55, 56, 57, 58, 59, 60,
      	            	61, 62, 63, 64, 65, 66, 67, 68, 69,
      	            	70, 71, 72, 73, 74, 75, 76, 77, 78};

      for(int i=0; i<NUM_DETS; i++) {
        DETECTORS[i] = tempDet[i];
      }
    }

    else //if(type == 2) // Stilbene setup
    {
      detectorPath = "expParameters/someFileFS3.txt";
      calibrationPath = "expParameters/intCalibrationSTIL.txt"; //main.cc
      TString pathT = (TString)calibrationPath;
      ifstream fin(calibrationPath);
      if(!fin.is_open()) {
        cout << "Failed to open calibration file\n";
        exit(0);
      }
      //store calibration values in TGraph
      calibrationDet = new TGraph(pathT);

      // experiment specific information
      NUM_BEAMS = 0;
      NUM_DETS = 1;
      NUM_CHAMBERS = 1;
      NUM_BROKEN = 0;

      // location of fission chambers and detectors
      FISSION_CHAMBERS = new int[NUM_CHAMBERS];
      int tempfiss[] = {0};
      for(int i=0; i<NUM_CHAMBERS; i++) {
        FISSION_CHAMBERS[i] = tempfiss[i];
      }

      BROKENDETECTORS = new int[NUM_BROKEN];
      int tempBroke[] = {};
      for(int i=0; i<NUM_BROKEN; i++) {
        BROKENDETECTORS[i] = tempBroke[i];
      }

      DETECTORS = new int[NUM_DETS];
      int tempDet[] = {10};
      for(int i=0; i<NUM_DETS; i++) {
        DETECTORS[i] = tempDet[i];
      }
    }
  }

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
