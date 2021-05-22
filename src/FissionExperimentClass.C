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
#include <TSystem.h>

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
        cout << "creating new coincidences." << endl;
		expFile = new TFile(treeFileT + rootEnding, "RECREATE");
	}
	else {
        cout << "reading old coincidences." << endl;
		expFile = new TFile(treeFileT + rootEnding, "READ");
	}

	detFile = new TFile(detFileT, "RECREATE");
  beamFile = new TFile(beamFileT, "RECREATE");

  // create the chain with all the entries to analyze for the raw coincidence mode
	rawTreeChain = new TChain();
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

	// attach the data file to the chain
	if(REUSE_DATA == 0)
	{
		cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

		// iterator for adding data to the fchain
		TFile *fileRaw;
		TTree *treeRaw;
		TString s_fileRaw;

		// set the input tree
		if(DATA_TYPE == 0)
		{
			 inputTreeName = compassName;
		}
		else if(DATA_TYPE == 1)
		{
			 inputTreeName = midasName;
		}

		for(int fileNum = MIN_FILE; fileNum < MIN_FILE + NUM_FILES; fileNum++)
		{
			 cout << "reading file number " << fileNum << endl;
			 // find the file
			 s_fileRaw =  filename + TString(to_string(fileNum)) + extExpFile;
       fileRaw = (TFile*)gROOT->GetListOfFiles()->FindObject(s_fileRaw);
       if (!fileRaw || !fileRaw->IsOpen()) {
          fileRaw = new TFile(s_fileRaw);
       }
       fileRaw->GetObject(inputTreeName, treeRaw);

			 if(treeRaw != 0)
			 {
	 			 rawTreeChain->Add(s_fileRaw + "/" + inputTreeName);
				 cout << "tree found at " << treeRaw << " added to chain" << endl;
			 }

			 //fileRaw->Close();
			 delete fileRaw;
		}

		CoincidenceAnalysis* inputData = new CoincidenceAnalysis(expFileWrite, rawTreeChain, beamFile, info); //replace digType w info->DATA_TYPE
		inputData->CreateCoincidenceTree(numEntries);
	}

	gROOT->cd();

	cout << "Reading coincidence tree from " << treeFileT + rootEnding + "/" + nameCoincTree << endl;

	coincTreeChain->Add(treeFileT + rootEnding + "/" + nameCoincTree);
	bool fileFound = true;
	int fileNum = 1;

	while(gSystem->AccessPathName(treeFileT + "_" + to_string(fileNum) + rootEnding) == false)
	{
		coincTreeChain->Add(treeFileT + "_" + to_string(fileNum) + rootEnding + "/" + nameCoincTree);
		cout << "And file " << treeFileT + "_" + to_string(fileNum) + rootEnding + "/" + nameCoincTree << endl;
		fileNum++;
	}
	cout << "Completed reading from " << fileNum << endl;
	return 1;
}

int FissionExperimentClass::CreateDetectionAnalysis()
{

	cout << "Analyzing " << coincTreeChain->GetEntries() << " events." << endl;
	detectorData = new DetectorSystemClass(coincTreeChain, detFile, info);

	cout << "Creating the functions to analyze the data. " << endl;
	detectorData->InitializePSDFunctions();

	cout << "Creating the histograms to store the data. " << endl;
	detectorData->InitializeDetectorHistograms();

	cout << "Entering detector analysis mode" << endl;
	detectorData->DetectionAnalysis();

	// System analysis needs to be fixed before running it
	// cout << "Entering system analysis mode" << endl;
	// detectorData->SystemAnalysis();

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
  // Close more things?
}

TString FissionExperimentClass::getExpName()
{
	return nameOfExp;
}
