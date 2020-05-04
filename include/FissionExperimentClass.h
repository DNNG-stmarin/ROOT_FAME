/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Class that analyzes experiments in fission from raw data to observed correlations.

The object contains several classes each representing a stage of the analysis process.

Date: late April 2020, Ann Arbor, Michigan
*/

#ifndef FissionExperimentClass_h
#define FissionExperimentClass_h

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
private:
	TString nameOfExp;
	int startFile = 0;
	int numFiles = 1;

	TString nameExpFile = "fiss";
	TString extExpFile = ".root";

	TString nameCoincTree = "FissionTree";

	TString treeFileT = "CoincidenceTrees.root";
	TString detFileT = "Detectors.root";
	TString sysFileT = "System.root";

public:
	// file to write all results
	TFile* expFile = 0;
	TFile* detFile = 0;
	TFile* sysFile = 0;

	// folder to write the coincidence trees
	TDirectory* treeDir = 0;
	TDirectory* detDir = 0;
	TDirectory* sysDir = 0;

	// pointer attributes
	FissionAnalysis* inputData;
	SystemAnalysis* systemData;

	// chain of raw tree files
	TChain* coincTreeChain = 0;

	// class constructor
	FissionExperimentClass();

	// operations
	int CreateFissionTree(TString filename, TFile* expFile, int numEntries = - 1);
	int DetectionAnalysis(TChain* chain, TFile* writeFile);
	int CreateSystemAnalysis(TChain* chainm, TFile* writeFile);

	// get attributes
	int getStartFile();
	int getEndFile();
	TString getExpName();
};

#endif