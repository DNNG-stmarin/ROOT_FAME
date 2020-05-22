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

#include <TH1.h>
#include <TH2.h>
#include <THStack.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "DetectorClass.h"
#include "TriggerClass.h"

#include "PhysicalConstants.h"
#include "ProcessingConstants.h"
//#include "InfoSystem.h"
#include "InfoSystemTest.h"

using namespace std;

class DetectorSystemClass
{
public:

	/*
	___   _____
 |_ _| / / _ \
	| | / / (_) |
 |___/_/ \___/

	*/

	// inputs given by the fission experiment class file, made up of the data collected and
	// where to write
	TChain* tree;
	TFile*  detFile;

	TTree* fissionTree;
	TFile* fissionFile;

	TDirectory *cdMult, *cdCoinc, *cdFigCoinc, *cdBicorr, *cdRef;
	Int_t   fCurrent;

	TString nameExp = "Exp";
	TString nameFission = "Fiss";


	/*
	___      _          _
 |   \ ___| |_ ___ __| |_ ___ _ _ ___
 | |) / -_)  _/ -_) _|  _/ _ \ '_(_-<
 |___/\___|\__\___\__|\__\___/_| /__/
	*/

	// characterize the detector system
	int numTriggers;
	int numDetectors;

	// arrays of detectors to modify
	TriggerClass* triggers;
	DetectorClass* detectors;

	//declare arrays as dynamic and allot space once info object is passed
	TH2I* hMult; // multiplicity histograms
	//TH2F* hBicorr[NUM_DETS][NUM_DETS]; // bicorrelation histograms
	TH2F*** hBicorr;

	TH1I* hSingles; // singles measurements
	TH2I* hDoubles; // doubles measurements

	// // create array of cross-correlations histograms
	// TH1F* nnMult[NUM_DETS][NUM_DETS];
	// TH1F* ggMult[NUM_DETS][NUM_DETS];
	// TH1F* gnMult[NUM_DETS][NUM_DETS];
	// TH1F* ngMult[NUM_DETS][NUM_DETS];
	// THStack* allCoinc[NUM_DETS][NUM_DETS];
	//
	// TH2F* reflections[NUM_DETS][NUM_DETS]; // reflections in detectors

	TH1F*** nnMult;
	TH1F*** ggMult;
	TH1F*** gnMult;
	TH1F*** ngMult;
	THStack*** allCoinc;

	TH2F*** reflections;

/*
	 _____
	|_   _| _ ___ ___
	  | || '_/ -_) -_)
	  |_||_| \___\___|

*/

	// set up the reader for the coincidence tree
	// Declaration of leaf types
	int           tMult;
	double        tTime;
	double        tDep;
	// double        totToF[MAX_MULTIPLICITY];   //[tMult]
	// double        totPSP[MAX_MULTIPLICITY];   //[tMult]
	// double        totDep[MAX_MULTIPLICITY];   //[tMult]
	// double        totTail[MAX_MULTIPLICITY];   //[tMult]
	// int           totChan[MAX_MULTIPLICITY];   //[tMult]

	double*        totToF;   //[tMult]
	double*        totPSP;   //[tMult]
	double*        totDep;   //[tMult]
	double*        totTail;   //[tMult]
	int*           totChan;   //[tMult]

	// List of branches
	TBranch        *b_tMult;   //!
	TBranch        *b_fissionTime;   //!
	TBranch        *b_fissionErg;   //!
	TBranch        *b_totToF;   //!
	TBranch        *b_totPSP;   //!
	TBranch        *b_totDep;   //!
	TBranch        *b_totTail;   //!
	TBranch        *b_totChan;   //!




	// properties of the output tree
	double    f_fisTime;
	double    f_fisErg;
	int       f_neutronMult;
	int       f_gammaMult;

/*
___             _   _
| __|  _ _ _  __| |_(_)___ _ _  ___
| _| || | ' \/ _|  _| / _ \ ' \(_-<
|_| \_,_|_||_\__|\__|_\___/_||_/__/
*/

	// store all the histograms
	DetectorSystemClass(TChain* treeIn, TFile* writeFile, InfoSystemTest* info);
	virtual ~DetectorSystemClass();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TChain *treeIn);
	virtual void     InitFiss();
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
	virtual void 		 InitializeDetectorHistograms(InfoSystemTest* info);

	// functions to perfom the detection analysis
	virtual int      DetectionAnalysis(InfoSystemTest* info);
	virtual void     SystemAnalysis(InfoSystemTest* info);
	virtual void 		 FissionAnalysis(InfoSystemTest* info);
};
#endif



#ifdef DetectionAnalysis_cxx

// constructor of the detector system class
DetectorSystemClass::DetectorSystemClass(TChain* treeIn, TFile* writeFile, InfoSystemTest* info)
{
	// set the number of detectors and triggers
	numTriggers = info->NUM_CHAMBERS;
	numDetectors = info->NUM_DETS;

	// create the dynamically allocated array of detectors and triggers
	triggers = new TriggerClass[info->NUM_CHAMBERS];
	detectors = new DetectorClass[info->NUM_DETS];

	cout << "Detectors and triggers have been created" << endl;

	// initialize the tree and the file to write to
	detFile = writeFile;
	Init(treeIn);
	//cout << "DetectorSystemClass treeIn: " << treeIn << endl;

	// create the directories to store the results
	cdMult =  detFile->mkdir("Multiplicity");
	cdCoinc =  detFile->mkdir("Coincidences");
	cdFigCoinc =  detFile->mkdir("CoincFigs");
	cdBicorr =  detFile->mkdir("Bicorr");
	cdRef =  detFile->mkdir("Reflections");

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

   cout << "Tree being read in correctly @ " << tree << endl;

}

void DetectorSystemClass::InitFiss()
{
	 // initialize the fission tree
   fissionTree->Branch("fisTime", &f_fisTime, "fisTime/D");
   fissionTree->Branch("fisErg", &f_fisErg, "fisErg/D");
   fissionTree->Branch("neutronMult", &f_neutronMult, "neutronMult/I");
   fissionTree->Branch("gammaMult", &f_gammaMult, "neutronMult/I");

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

#endif // #ifdef SystemAnalysis_cxx
