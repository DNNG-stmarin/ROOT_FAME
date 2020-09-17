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

//#include "InfoSystem.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"
#include "InfoSystem.h"



using namespace std;


// methods to acquire from the user the info about the system
FissionExperimentClass::FissionExperimentClass(/*inputFileName*/)
{
	//call info System

	//string inName;
	cout << "What is the name of your experiment: ";
	cin >> nameOfExp;
	//nameOfExp = TString(inName);

	cout << "What type is your experiment: " << endl;
	cout << "(0): FS-3" << endl;
	cout << "(1): ChiNu" << endl;
	cin >> expType;

	cout << "What is your initial start file: ";
	cin >> startFile;

	cout << "How many files are you analyzing: ";
	cin >> numFiles;

	cout << "What is the format of your data file: " << endl;
	cout << "(0): CoMPASS" << endl;
	cout << "(1): MIDAS (LANSCE)" << endl;
	cin >> digType;

	cout << "Do you want to use existing data: no (0), yes (1)" << endl;
	cin >> oldDat;

	cout << "Do you want to enter debug option: no (0), yes (1)" << endl;
	cin >> debug;

	resultFold = new TFolder(nameOfExp, nameOfExp);

	// read data already written
	if(oldDat == 0)
	{
		expFile = new TFile(treeFileT, "RECREATE");
	}
	else
	{
		expFile = new TFile(treeFileT, "READ");
	}

	detFile = new TFile(detFileT, "RECREATE");

	//info = new InfoSystem(expType);
	info = new InfoSystem();
	info->ReadInput(inputFileName);

  // create the chain with all the entries to analyze for the raw coincidence mode
  coincTreeChain = new TChain();
}

// destructor closes all the remaining loose ends
/*
FissionExperimentClass::~FissionExperimentClass()
{

}
*/

// Now go ahead and create all the fission tree
int FissionExperimentClass::CreateCoincidenceTree(TString filename, TFile* expFileWrite, int numEntries)
{
	// produce the data if not already present
	if(oldDat == 0)
	{
		cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

		//info->min_file... etc
		for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
		{
			cout << "reading file number " << fileNum << endl;
			CoincidenceAnalysis* inputData = new CoincidenceAnalysis(filename + TString(to_string(fileNum)) + extExpFile, fileNum, expFileWrite, digType); //replace digType w info->DATA_TYPE
			inputData->CreateCoincidenceTree(info, fileNum, numEntries);
		}
	}
	// coincidence tree must be closed in order to read from them
	expFile->Close();

	gROOT->cd();

	// attach the coincidence tree to the chain
	for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
	{
		coincTreeChain->Add(treeFileT + "/" + TString(to_string(fileNum)) + "/" + nameCoincTree);
	}
	//expFile->Close();

	//cout << "Analyzing " << coincTreeChain->GetEntries() << " events." << endl;

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
	if(debug)
		detectorData->FissionAnalysisLoop();
	else
		detectorData->FissionAnalysis();

	return 1;
}

// getSystemInfo
int FissionExperimentClass::getStartFile()
{
	return startFile;
}

int FissionExperimentClass::getEndFile()
{
	return startFile + numFiles;
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
