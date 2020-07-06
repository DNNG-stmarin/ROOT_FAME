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
#include <TH3.h>
#include <THStack.h>

#include <TF1.h>

#include <TCut.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "DetectorClass.h"
#include "TriggerClass.h"

//#include "PhysicalConstants.h"
#include "ProcessingConstants.h"
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

	// inpu variables
	TTree* fissionTree;
	TFile* fissionFile;

	// directories to create for the output
	TDirectory *cdPsd;
	TDirectory *cdToF;
	TDirectory *cdKin;
	TDirectory *cdMult;
	TDirectory *cdCoinc;
	TDirectory *cdFigCoinc;
	TDirectory *cdBicorr;
	TDirectory *cdRef;

	// subdirectory for slices
	TDirectory * cdPsdSlices;
	TDirectory * cdPsdIndividual;
	TDirectory * cdPsdErg;
	TDirectory * cdTofSlices;
	TDirectory * cdTofIndividual;
	TDirectory * cdTOFPSD;
	TDirectory * cdTofErg;
	TDirectory * cdTOFCorr;

	// current tree in chain
	Int_t   fCurrent;

	// names
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

	// list of channels for triggers and detectors
	int* listTriggersChan;
	int* listDetectorsChan;

	// arrays of detectors to modify
	TriggerClass* triggers;
	DetectorClass* detectors;

/*
_  _ _    _
| || (_)__| |_ ___  __ _ _ _ __ _ _ __  ___
| __ | (_-<  _/ _ \/ _` | '_/ _` | '  \(_-<
|_||_|_/__/\__\___/\__, |_| \__,_|_|_|_/__/
									|___/
*/

	// experiment Histograms
	TH1F** triggerHist;

	// experiment Histograms
	TH3F** expHists;

	// psd and energy histograms for discriminations
	TH1F** psdhists;
	TH1F** erghists;
	TH2F** psdErgHists;
	TH2F** tofPsdHists;
	TH2F** tofErgHists;

	// tof histograms
	TH1F** tofDelPhists;
	TH1F** tofNhists;
	TH1F** tofPhists;

	//corrected tof histograms
	TH1F** tofDelPhistsCorr;
	TH2F** tofPsdHistsCorr;
	TH2F** tofErgHistsCorr;

	// kinematic histograms
	TH2F** kinematicAll;
	TH2F** kinematicN;
	TH2F** kinematicP;

	TH2I* hMult; // multiplicity histograms

	TH2F*** hBicorr; // bicorrelation histograms

	TH1I* hSingles; // singles measurements
	TH2I* hDoubles; // doubles measurements

	// cross correlations histograms
	TH1F*** nnMult;
	TH1F*** ggMult;
	TH1F*** gnMult;
	TH1F*** ngMult;
	THStack*** allCoinc;

	TH2F*** reflections; // reflections histogram

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

	double        totToF[MAX_MULTIPLICITY];   //[tMult]
	double        totPSP[MAX_MULTIPLICITY];   //[tMult]
	double        totDep[MAX_MULTIPLICITY];   //[tMult]
	double        totTail[MAX_MULTIPLICITY];   //[tMult]
	int           totChan[MAX_MULTIPLICITY];   //[tMult]

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
	int       f_neutronBackMult;
	int       f_gammaBackMult;


	/*
	___  ___ ___
 | _ \/ __|   \
 |  _/\__ \ |) |
 |_|  |___/___/

	*/

	//bounds
	double minPSD_fit = 0.00;
	double divPSD_fit = 0.16;
	double maxPSD_fit = 0.40;

	double minErg_fit = 0.05; // MeVee
	double maxErg_fit = 4.00; // MeVee


	// initialize the fitting functions
	TF1* fitPSD_p;
	TF1* fitPSD_n;
	TF1* fitPSD;
	TF1* intersection;

	//TF1* fitTOF_p;
	//TF1* fitTOF_n;
	//TF1* TOFinteresection


/*
  ___     _
 / __|  _| |_ ___
| (_| || |  _(_-<
 \___\_,_|\__/__/
*/
TCut selectChan;



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

	// functions to initialize attributes
	virtual void 		 InitializeDetectorHistograms();
	virtual void 		 InitializePSDFunctions();

	// functions to perfom the detection analysis
	//virtual void      TriggerAnalysis();
	virtual int      DetectionAnalysis();
	virtual void     SystemAnalysis();
	virtual void 		 FissionAnalysis();
	virtual void     FissionAnalysisLoop();

	// mapping functions
	int isDetector(int detectorNumber);
	int isChamber(int detectorNumber);

};
#endif
