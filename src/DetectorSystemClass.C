/*
Detector System Class Methods,
Author: Stefano Marin
Purpose: Methods associated with detector system class
Date: May 24th, 2020
*/

#include "DetectorSystemClass.h"

#define DetectorSystemClass_cxx

// constructor of the detector system class
DetectorSystemClass::DetectorSystemClass(TChain* treeIn, TFile* writeFile, InfoSystem* info)
{
	DETECTOR_THRESHOLD = info->DETECTOR_THRESHOLD;
	COINC_WINDOW = info->COINC_WINDOW;
	MIN_TIME_P = info->MIN_TIME_P;
	MAX_TIME_P = info->MAX_TIME_P;
	MIN_TIME_N = info->MIN_TIME_N;
	MAX_TIME_N = info->MAX_TIME_N;
	DELTA_BACK_SIG = info->DELTA_BACK_SIG;
	BACKGROUND_SHIFT = MAX_TIME_N - MIN_TIME_P + DELTA_BACK_SIG;
	DEBUG = info->DEBUG;

	NUM_TRIGGERS = info->NUM_TRIGGERS;
	NUM_DETS = info->NUM_DETS;
	NUM_EXCLUDED = info->NUM_EXCLUDED;
	FISSION_TRIGGERS = info->FISSION_TRIGGERS;
	DETECTORS = info->DETECTORS;
	EXCLUDE_DETECTORS = info->EXCLUDE_DETECTORS;

	// create the dynamically allocated array of detectors and triggers
	triggers = new TriggerClass[NUM_TRIGGERS];
	detectors = new DetectorClass[NUM_DETS];
	cout << "Detectors and triggers have been created" << endl;

	string line;
	string x, y, z;
	ifstream in (info->detectorPath);
	for(int i=0; i<NUM_DETS; i++) {
		getline(in, line);
		istringstream iss(line);
		iss >> x >> y >> z;
		detectors[i].X = stod(x)*100;
		detectors[i].Y = stod(y)*100;
		detectors[i].Z = stod(z)*100;
		detectors[i].distance = sqrt(pow(stod(x),2)+pow(stod(y),2)+pow(stod(z),2))*100;
	}

	//calibration for only chinu system
	detCalibration = new TGraph(*(info->calibrationDet));
	for(int i=0; i<NUM_DETS; i++) {
		detectors[i].calibration = detCalibration->Eval(i)/CSCOMPTEDGE;
	}
	cout << "Detector calibration complete\n" << endl;

	// initialize the tree and the file to write to
	detFile = writeFile;
	cout << "Tree passed at " << treeIn << endl;
	Init(treeIn);

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

	 delete triggers;
	 delete detectors;

	 delete DETECTORS;
	 delete FISSION_TRIGGERS;
	 delete EXCLUDE_DETECTORS;
	 delete detCalibration;
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

void DetectorSystemClass::Init(TChain* treeIn)
{
   // Set branch addresses and branch pointers for the coincidence tree
   if (!treeIn) {
		 cout << "No tree!" << endl;
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

		 cout << "Tree being read in correctly " << tree << endl;;
	 }

}

void DetectorSystemClass::InitFiss()
{
	 // initialize the fission tree
	 fissionTree->Branch("fisTime", &f_fisTime, "fisTime/D");
	 fissionTree->Branch("fisErg", &f_fisErg, "fisErg/D");
	 fissionTree->Branch("neutronMult", &f_neutronMult, "neutronMult/I");
	 fissionTree->Branch("gammaMult", &f_gammaMult, "gammaMult/I");
	 fissionTree->Branch("neutronBackMult", &f_neutronBackMult, "neutronBackMult/I");
	 fissionTree->Branch("gammaBackMult", &f_gammaBackMult, "gammaBackMult/I");

	 // pass the array without dereferencing.
	 fissionTree->Branch("neutronDetTimes", neutronDetTimes, "neutronDetTimes[neutronMult]/D");
	 fissionTree->Branch("neutronLightOut", neutronLightOut, "neutronLightOut[neutronMult]/D");
	 fissionTree->Branch("neutronPSD", neutronPSD, "neutronPSD[neutronMult]/D");
	 fissionTree->Branch("neutronToFErg", neutronToFErg, "neutronToFErg[neutronMult]/D");
	 fissionTree->Branch("neutronDet", neutronDet, "neutronDet[neutronMult]/D");
	 fissionTree->Branch("neutronVx", neutronVx, "neutronVx[neutronMult]/D");
	 fissionTree->Branch("neutronVy", neutronVy, "neutronVy[neutronMult]/D");
	 fissionTree->Branch("neutronVz", neutronVz, "neutronVz[neutronMult]/D");
	 fissionTree->Branch("photonDetTimes", photonDetTimes, "photonDetTimes[gammaMult]/D");
	 fissionTree->Branch("photonLightOut", photonLightOut, "photonLightOut[gammaMult]/D");
	 fissionTree->Branch("photonPSD", photonPSD, "photonPSD[gammaMult]/D");
	 fissionTree->Branch("photonDet", photonDet, "photonDet[gammaMult]/D");
	 fissionTree->Branch("photonVx", photonVx, "photonVx[gammaMult]/D");
	 fissionTree->Branch("photonVy", photonVy, "photonVy[gammaMult]/D");
	 fissionTree->Branch("photonVz", photonVz, "photonVz[gammaMult]/D");

	 fissionTree->Branch("backNeutronDetTimes", backNeutronDetTimes, "backNeutronDetTimes[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronLightOut", backNeutronLightOut, "backNeutronLightOut[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronPSD", backNeutronPSD, "backNeutronPSD[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronToFErg", backNeutronToFErg, "backNeutronToFErg[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronDet", backNeutronDet, "backNeutronDet[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronVx", backNeutronVx, "backNeutronVx[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronVy", backNeutronVy, "backNeutronVy[neutronBackMult]/D");
	 fissionTree->Branch("backNeutronVz", backNeutronVz, "backNeutronVz[neutronBackMult]/D");
	 fissionTree->Branch("backPhotonDetTimes", backPhotonDetTimes, "backPhotonDetTimes[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonLightOut", backPhotonLightOut, "backPhotonLightOut[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonPSD", backPhotonPSD, "backPhotonPSD[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonDet", backPhotonDet, "backPhotonDet[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonVx", backPhotonVx, "backPhotonVx[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonVy", backPhotonVy, "backPhotonVy[gammaBackMult]/D");
	 fissionTree->Branch("backPhotonVz", backPhotonVz, "backPhotonVz[gammaBackMult]/D");

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


// returns the index of the detector or -1 if not a detector
int DetectorSystemClass::isDetector(int detectorNumber)
{
	int detIndex = -1;

	for(int index = 0; index < NUM_DETS; index++)
	{
		//cout << "In loop!!\n";
		if(detectorNumber == DETECTORS[index])
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
	for(int index = 0; index < NUM_TRIGGERS; index++)
	{
		if(detectorNumber == FISSION_TRIGGERS[index])
		{
			chamberIndex = index;
			break;
		}
	}
	return chamberIndex;
}
