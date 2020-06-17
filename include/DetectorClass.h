/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Class with all of the properties of the detection system

Date: Ann Arbor, MI, May 3rd, 2020
*/

#ifndef DetectorClass_h
#define DetectorClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TF1.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


class DetectorClass
{
private:

public:

	TString name = "det";

	double energyThreshold; // threshold in keVee

	double timeDelay; // store the time delay with respect to the trigger

	double energyResolution; // store the energy resolution of this detector
	double timeResolution; // store the time resolution from the fitted gamma peak

	TF1* discPSD; // psd discrimination line

	double energyCalibration; // store the energy calibration for each detector

	double efficiency; // efficiency of this detector to

	double X, Y, Z; // position of the center of the detector

};

#endif
