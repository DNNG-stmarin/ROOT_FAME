// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystemTest_h
#define InfoSystemTest_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

class InfoSystemTest
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

  InfoSystemTest(int type)
  {
    if(type == 0) // FS3
    {
      // experiment specific information
      NUM_BEAMS = 0;
      NUM_DETS = 6;
      NUM_CHAMBERS = 3;

      // location of fission chambers and detectors
      //BEAM[NUM_BEAMS] = {};
      FISSION_CHAMBERS = new int[NUM_CHAMBERS];
      int tempfiss[] = {0, 16, 32};
      //memcpy(FISSION_CHAMBERS, tempfiss, sizeof(tempfiss));
      for(int i=0; i<NUM_CHAMBERS; i++) {
        FISSION_CHAMBERS[i] = tempfiss[i];
      }

      DETECTORS = new int[NUM_DETS];
      int tempDet[] = {2, 10, 18, 26, 34, 42};
      //memcpy(DETECTORS, tempDet, sizeof(tempDet));
      for(int i=0; i<NUM_DETS; i++) {
        DETECTORS[i] = tempDet[i];
      }
    }

    if(type == 1) // ChiNu
    {
      // experiment specific information
      NUM_BEAMS = 0;
      NUM_CHAMBERS = 1;
      NUM_DETS = 54;

      // location of fission chambers and detectors
      FISSION_CHAMBERS = new int[NUM_CHAMBERS];
      int tempfiss[] = {7};
      //memcpy(FISSION_CHAMBERS, tempfiss, sizeof(tempfiss));
      for(int i=0; i<NUM_CHAMBERS; i++) {
        FISSION_CHAMBERS[i] = tempfiss[i];
      }

      DETECTORS = new int[NUM_DETS];
      int tempDet[] = {9, 10, 11, 12, 13, 14, 15, 16, 25,
                      26, 27, 28, 29, 30, 31, 32, 41, 42,
      	            	43, 44, 45, 46, 47, 48, 49, 50, 51,
      	            	52, 53, 54, 55, 56, 57, 58, 59, 60,
      	            	61, 62, 63, 64, 65, 66, 67, 68, 69,
      	            	70, 71, 72, 73, 74, 75, 76, 77, 78};

      //memcpy(DETECTORS, tempDet, sizeof(tempDet));
      for(int i=0; i<NUM_DETS; i++) {
        DETECTORS[i] = tempDet[i];
      }
    }
  }

  ~InfoSystemTest()
  {
    delete BEAM;
    delete FISSION_CHAMBERS;
    delete DETECTORS;
  }
};

#endif
