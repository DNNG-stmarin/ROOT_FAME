/*
Detector System Class Methods,
Author: Stefano Marin
Purpose: Methods associated with detector system class
Date: May 24th, 2020
*/

#include "DetectorSystemClass.h"

#define DetectorSystemClass_cxx

// constructor of the detector system class //
DetectorSystemClass::DetectorSystemClass(TChain* treeIn, TFile* writeFile, InfoSystem* info)
{
	// set the number of detectors and triggers
	numTriggers = info->NUM_CHAMBERS;
	numDetectors = info->NUM_DETS;

	listTriggersChan = info->FISSION_CHAMBERS;
	listDetectorsChan = info->DETECTORS;

	// create the dynamically allocated array of detectors and triggers
	triggers = new TriggerClass[info->NUM_CHAMBERS];
	detectors = new DetectorClass[info->NUM_DETS];

	cout << "Detectors and triggers have been created" << endl;

	//calibration for only chinu system
	//create fake file of calibrations for fs3/stilbene
	detCalibration = new TGraph(*(info->calibrationDet));

	for(int i=0; i<numDetectors; i++) {
		detectors[i].calibration = detCalibration->Eval(i)/CSCOMPTEDGE;
	}

	// initialize the tree and the file to write to
	detFile = writeFile;
	cout << "tree passed at " << treeIn << endl;
	Init(treeIn);
	//cout << "DetectorSystemClass treeIn: " << treeIn << endl;

	// create the directories to store the results

	//create folders and write things to correct folder
	cdPsd = detFile->mkdir("PSD");
	cdToF = detFile->mkdir("TOF");
	cdKin = detFile->mkdir("Kinematics");
	cdMult =  detFile->mkdir("Multiplicity");
	cdCoinc =  detFile->mkdir("Coincidences");
	cdFigCoinc = detFile->mkdir("CoincFigs");
	cdBicorr = detFile->mkdir("Bicorr");
	cdRef = detFile->mkdir("Reflections");

	// create the folder for psd slices
	cdPsdSlices = cdPsd->mkdir("PSD_slices");
	cdPsdIndividual = cdPsd->mkdir("PSD_individual");
	cdPsdErg = cdPsd->mkdir("PSDErg_discrimination");
	cdTofSlices = cdToF->mkdir("TOF_slices");
	cdTofIndividual = cdToF->mkdir("TOF_individual");
	cdTOFPSD = cdPsd->mkdir("TOF_PSD");
	cdTofErg = cdToF->mkdir("TOFErg_discrimination");
	cdTOFCorr = cdToF->mkdir("TOF_Corrected");

	fissionFile = new TFile(nameFission + ".root", "RECREATE");
	fissionTree = new TTree(nameFission, nameFission);

	InitFiss();
}

DetectorSystemClass::~DetectorSystemClass()
{
   delete tree;
}

Int_t DetectorSystemClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!tree) return 0;
   return tree->GetEntry(entry);
}
Long64_t DetectorSystemClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!tree) return -5;
   Long64_t centry = tree->LoadTree(entry);
   if (centry < 0) return centry;
   if (tree->GetTreeNumber() != fCurrent) {
      fCurrent = tree->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DetectorSystemClass::Init(TChain *treeIn)
{
   // Set branch addresses and branch pointers for the coincidence tree
   if (!tree) {
		 cout << "no tree!" << endl;
		 return;
	 }

	 else
	 {
		 tree = treeIn;
		 fCurrent = -1;
		 tree->SetMakeClass(1);

		 tree->SetBranchAddress("tMult", &tMult, &b_tMult);
		 tree->SetBranchAddress("tTime", &tTime, &b_fissionTime);
		 tree->SetBranchAddress("tDep", &tDep, &b_fissionErg);
		 tree->SetBranchAddress("totToF", totToF, &b_totToF);
		 tree->SetBranchAddress("totPSP", totPSP, &b_totPSP);
		 tree->SetBranchAddress("totDep", totDep, &b_totDep);
		 tree->SetBranchAddress("totTail", totTail, &b_totTail);
		 tree->SetBranchAddress("totChan", totChan, &b_totChan);
		 Notify();

		 cout << "Tree being read in correctly " << tree << endl;
	 }

}

void DetectorSystemClass::InitFiss()
{
	 // initialize the fission tree
	 // ISABEL //
   fissionTree->Branch("fisTime", &f_fisTime, "fisTime/D");
   fissionTree->Branch("fisErg", &f_fisErg, "fisErg/D");
   fissionTree->Branch("neutronMult", &f_neutronMult, "neutronMult/I");
   fissionTree->Branch("gammaMult", &f_gammaMult, "neutronMult/I");
   fissionTree->Branch("neutronBackMult", &f_neutronBackMult, "neutronBackMult/I");
   fissionTree->Branch("gammaBackMult", &f_gammaBackMult, "gammaBackMult/I");

   cout << "Fission tree has been created." << endl;
}

Bool_t DetectorSystemClass::Notify()
{
   return kTRUE;
}

void DetectorSystemClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!tree) return;
   tree->Show(entry);
}

Int_t DetectorSystemClass::Cut(Long64_t entry)
{
   return 1;
}
// mapping functions


// returns the index of the detector or -1 if not a detector
int DetectorSystemClass::isDetector(int detectorNumber)
{
	int detIndex = -1;

	for(int index = 0; index < numDetectors; index++)
	{
		//cout << "In loop!!\n";
		if(detectorNumber == listDetectorsChan[index])
		{
			detIndex = index;
			break;
		}
	}
	return detIndex;
}

int DetectorSystemClass::isChamber(int detectorNumber)
{
	int chamberIndex = -1;
	for(int index = 0; index < numTriggers; index++)
	{
		if(detectorNumber == listTriggersChan[index])
		{
			chamberIndex = index;
			break;
		}
	}
	return chamberIndex;
}
