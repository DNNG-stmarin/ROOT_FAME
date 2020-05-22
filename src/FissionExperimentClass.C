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
#include "InfoSystemTest.h"



using namespace std;


// methods to acquire from the user the info about the system
FissionExperimentClass::FissionExperimentClass()
{
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

	//resultFold = new TFolder(nameOfExp, nameOfExp);

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
	//sysFile = new TFile(sysFileT, "RECREATE");

	//InfoSystemTest info = InfoSystemTest(expType);

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
	InfoSystemTest info = InfoSystemTest(expType);
	// produce the data if not already present
	if(oldDat == 0)
	{
		cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

		for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
		{
			cout << "reading file number " << fileNum << endl;
			CoincidenceAnalysis* inputData = new CoincidenceAnalysis(filename + TString(to_string(fileNum)) + extExpFile, fileNum, expFileWrite, digType);
			cout << "\nFinish CA constructor\n";
			inputData->CreateCoincidenceTree(info, fileNum, numEntries);
			cout << "\nFinish CCT\n";
		}
		expFile->Close();
	}

	// attach the coincidence tree to the chain
	for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
	{
		coincTreeChain->Add(treeFileT + "/" + TString(to_string(fileNum)) + "/" + nameCoincTree);
	}
	//expFile->Close();

	cout << "Analyzing " << coincTreeChain->GetEntries() << " events." << endl;

	cout << "get this far??\n";

	return 1;
}

int FissionExperimentClass::CreateDetectionAnalysis(TChain* chain, TFile* writeFile)
{
	InfoSystemTest info = InfoSystemTest(expType);

	detectorData = new DetectorSystemClass(coincTreeChain, detFile, info);

	cout << "Entering detector analysis mode" << endl;
	detectorData->DetectionAnalysis(info);

	cout << "Creating the histograms to store the data. " << endl;
	detectorData->InitializeDetectorHistograms(info);

	cout << "Entering system analysis mode" << endl;
	detectorData->SystemAnalysis(info);

	cout << "Entering fission analysis mode" << endl;
	detectorData->FissionAnalysis(info);

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
	//delete sysFile;
}

TString FissionExperimentClass::getExpName()
{
	return nameOfExp;
}
