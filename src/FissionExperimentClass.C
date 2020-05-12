/*
Author: Stefano Marin, Isabel Hernandez
Date: May 1st, 2020
Purpose: Methods of the Fission Experiment Class
*/

#define FissionExperimentClass_cxx
#include "FissionAnalysis.h"

#include "FissionExperimentClass.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

#include "InfoSystem.h"
#include "FissionEvent.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"

using namespace std;


// methods to acquire from the user the info about the system
FissionExperimentClass::FissionExperimentClass()
{
	cout << "What is your initial start file: ";
	cin >> startFile;

	cout << "How many files are you analyzing: ";
	cin >> numFiles;

	cout << "What is the format of your data file: " << endl;
	cout << "(0): CoMPASS" << endl;
	cout << "(1): MIDAS (LANSCE)" << endl;
	cin >> digType;

	cout << "Do you want to use existing data: ";
	cin >> oldDat;

	// read data already written
	if(oldDat == 0)
	{
		expFile = new TFile(treeFileT, "RECREATE");
	}
	else
	{
		expFile = new TFile(treeFileT, "READm");
	}

	detFile = new TFile(detFileT, "RECREATE");
	sysFile = new TFile(sysFileT, "RECREATE");

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
int FissionExperimentClass::CreateFissionTree(TString filename, TFile* expFileWrite, int numEntries)
{

	if(oldDat == 0)
	{
		cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

		for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
		{
			cout << "reading file number " << fileNum << endl;
			FissionAnalysis* inputData = new FissionAnalysis(filename + TString(to_string(fileNum)) + extExpFile, fileNum, expFileWrite, digType);
			inputData->CreateFissionTree(fileNum, numEntries);
		}
		expFile->Close();
	}

	else
	{
		for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
		{
			coincTreeChain->Add(treeFileT + "/" + TString(to_string(fileNum)) + "/" + nameCoincTree);
		}
		expFile->Close();
	}

	cout << "Analyzing " << coincTreeChain->GetEntries() << " events." << endl;

	return 1;
}

int FissionExperimentClass::CreateDetectionAnalysis(TChain* chainm, TFile* writeFile)
{
	DetectorSystemClass* detData = new DetectorSystemClass(coincTreeChain, detFile);
	detData->DetectionAnalysis();
	return 1;
}

int FissionExperimentClass::CreateSystemAnalysis(TChain* chainm, TFile* writeFile)
{
	SystemAnalysis* sysData = new SystemAnalysis(coincTreeChain, sysFile);
	sysData->Loop();
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

TString FissionExperimentClass::getExpName()
{
	return nameOfExp;
}
