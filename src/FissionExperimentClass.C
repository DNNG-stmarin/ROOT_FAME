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

	expFile = new TFile(treeFileT, "RECREATE");
	detFile = new TFile(detFileT, "RECREATE");
	sysFile = new TFile(sysFileT, "RECREATE");

    // create the chain with all the entries to analyze for the raw coincidence mode
    coincTreeChain = new TChain();
}

// Now go ahead and create all the fission tree
int FissionExperimentClass::CreateFissionTree(TString filename, TFile* expFileWrite, int numEntries)
{

	cout << "Writing coincidence trees to " << expFileWrite->GetName() << "." << endl;

	for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
	{
		cout << "reading file number " << fileNum << endl;
		FissionAnalysis* inputData = new FissionAnalysis(filename + TString(to_string(fileNum)) + extExpFile, fileNum, expFileWrite);
		inputData->CreateFissionTree(fileNum, numEntries);
	}
	expFile->Close();

	for(int fileNum = startFile; fileNum < startFile + numFiles; fileNum++)
	{
		coincTreeChain->Add(treeFileT + "/" + TString(to_string(fileNum)) + "/" + nameCoincTree);
	}

	cout << coincTreeChain->GetEntries() << endl;

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