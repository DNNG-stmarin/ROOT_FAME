/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Describe the detector system and calibrates each detector individually

Date: Ann Arbor, MI, May 3rd, 2020
*/

#ifndef DetectorSystemClass_h
#define DetectorSystemClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "FissionAnalysis.h"
#include "SystemAnalysis.h"

using namespace std;


class FissionExperimentClass
{
public:
	int numTriggers;
	int numDetectors;

	TriggerClass* Triggers;
	DetectorClass* Detectors;

};

#endif