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

#include "DetectorClass.h"
#include "TriggerClass.h"

#include "InfoSystem.h"

using namespace std;


class DetectorSystemClass
{
public:
	// characterize the detector system
	int numTriggers;
	int numDetectors;

	// create arrays of detectors to modify
	TriggerClass* triggers;
	DetectorClass* detectors;

	// inputs given by the larger file, made up of the data collected and
	// where to write
	TChain* tree;
	TFile* expFile;

	// constructor
	DetectorSystemClass(TChain* treeIn, TFile* writeFile)
	{
		numTriggers = 0;
		numDetectors = 0;

		triggers = new (TriggerClass[NUM_CHAMBERS]);
		detectors = new (DetectorClass[NUM_DETS]);

		cout << "Detectors created at: " << detectors[0].name << endl;

		tree = treeIn;
		expFile = writeFile;
	}


	// functions to perfom the detection analysis
	int DetectionAnalysis();
};

#endif
