/*
Author: Stefano Marin, Isabel Hernandez
Date: May 1st, 2020
Purpose: Methods of the Fission Experiment Class
*/

#define FissionExperimentClass_cxx

#include "FissionExperimentClass.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>
#include <TFolder.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <queue>

#include "ParticleEvent.h"
#include "TriggerEvent.h"
#include "InfoSystem.h"

using namespace std;


FissionExperimentClass::FissionExperimentClass(TString inputFileName)
{
	//call info System
	info = new InfoSystem;
	info->ReadInput(inputFileName);

	nameOfExp = info->nameOfExp;
	MIN_FILE = info->MIN_FILE;
	NUM_FILES = info->NUM_FILES;
	DATA_TYPE = info->DATA_TYPE;
	REUSE_DATA = info->REUSE_DATA;

	resultFold = new TFolder(nameOfExp, nameOfExp);

	if(REUSE_DATA == 0) {
		expFile = new TFile(treeFileT, "RECREATE");
	}
	else {
		expFile = new TFile(treeFileT, "READ");
	}
	detFile = new TFile(detFileT, "RECREATE");

  // create the chain with all the entries to analyze for the raw coincidence mode
  coincTreeChain = new TChain();
}

// destructor closes all the remaining loose ends
FissionExperimentClass::~FissionExperimentClass()
{
	delete info;
}

// Now go ahead and create all the fission tree
int FissionExperimentClass::CreateCoincidenceTree(TString filename, TFile* expFileWrite, int numEntries)
{
	// produce the data if not already present
	if(REUSE_DATA == 0)
	{
		cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

		for(int fileNum = MIN_FILE; fileNum < MIN_FILE + NUM_FILES; fileNum++)
		{
			cout << "reading file number " << fileNum << endl;
			CoincidenceAnalysis* inputData = new CoincidenceAnalysis(*info, filename + TString(to_string(fileNum)) + extExpFile, fileNum, expFileWrite, 0); //replace digType w info->DATA_TYPE
			inputData->CreateCoincidenceTree(fileNum, numEntries);
		}
	}
	// coincidence tree must be closed in order to read from them
	expFile->Close();

	gROOT->cd();

	// attach the coincidence tree to the chain
	for(int fileNum = MIN_FILE; fileNum < MIN_FILE + NUM_FILES; fileNum++)
	{
		coincTreeChain->Add(treeFileT + "/" + TString(to_string(fileNum)) + "/" + nameCoincTree);
	}

	return 1;
}

int FissionExperimentClass::CreateDetectionAnalysis(TFile* writeFile)
{

	cout << "Analyzing " << coincTreeChain->GetEntries() << " events." << endl;
	detectorData = new DetectorSystemClass(coincTreeChain, detFile, info);

	cout << "Creating the functions to analyze the data. " << endl;
	detectorData->InitializePSDFunctions();

	cout << "Creating the histograms to store the data. " << endl;
	detectorData->InitializeDetectorHistograms();

	cout << "Entering detector analysis mode" << endl;
	detectorData->DetectionAnalysis();

	cout << "Entering system analysis mode" << endl;
	detectorData->SystemAnalysis();

	cout << "Entering fission analysis mode" << endl;
	if(DEBUG==1)
		detectorData->FissionAnalysisLoop();
	else
		detectorData->FissionAnalysis();

	return 1;
}

// getSystemInfo
int FissionExperimentClass::getStartFile()
{
	return MIN_FILE;
}

int FissionExperimentClass::getEndFile()
{
	return MIN_FILE + NUM_FILES;
}

void FissionExperimentClass::saveAll()
{
	delete expFile;
	delete detFile;
}

TString FissionExperimentClass::getExpName()
{
	return nameOfExp;
}
