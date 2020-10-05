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
#include <TGraph.h>
#include <TCanvas.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "DetectorClass.h"
#include "TriggerClass.h"

#include "ProcessingConstants.h"
#include "InfoSystem.h"

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

	//InfoSystem info;
	double DETECTOR_THRESHOLD;
	double COINC_WINDOW;
	double MIN_TIME_P;
	double MAX_TIME_P;
	double MIN_TIME_N;
	double MAX_TIME_N;
	double DELTA_BACK_SIG;
	double BACKGROUND_SHIFT;
	int DEBUG;

	// inputs given by the fission experiment class file, made up of the data collected and where to write
	TChain* tree;
	TFile*  detFile;

	// input variables
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
	int NUM_CHAMBERS;
	int NUM_DETS;
	int NUM_BROKEN;

	// list of channels for triggers and detectors
	int* FISSION_CHAMBERS;
	int* DETECTORS;
	int* BROKEN_DETECTORS;

	// arrays of detectors to modify
	TriggerClass* triggers;
	DetectorClass* detectors;

	// calibrations of the detectors
	TGraph* detCalibration;

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


	double    f_fisTime;
	double    f_fisErg;
	int       f_neutronMult;
	int       f_gammaMult;
	int       f_neutronBackMult;
	int       f_gammaBackMult;

	double neutronDetTimes[MAX_MULTIPLICITY] = {0};
	double neutronLightOut[MAX_MULTIPLICITY] = {0};
	double neutronPSD[MAX_MULTIPLICITY] = {0};
	double neutronToFErg[MAX_MULTIPLICITY] = {0};
	double neutronDet[MAX_MULTIPLICITY] = {0};
	double neutronVx[MAX_MULTIPLICITY] = {0};
	double neutronVy[MAX_MULTIPLICITY] = {0};
	double neutronVz[MAX_MULTIPLICITY] = {0};

	double photonDetTimes[MAX_MULTIPLICITY] = {0};
	double photonLightOut[MAX_MULTIPLICITY] = {0};
	double photonPSD[MAX_MULTIPLICITY] = {0};
	double photonDet[MAX_MULTIPLICITY] = {0};
	double photonVx[MAX_MULTIPLICITY] = {0};
	double photonVy[MAX_MULTIPLICITY] = {0};
	double photonVz[MAX_MULTIPLICITY] = {0};

	double backNeutronDetTimes[MAX_MULTIPLICITY] = {0};
	double backNeutronLightOut[MAX_MULTIPLICITY] = {0};
	double backNeutronPSD[MAX_MULTIPLICITY] = {0};
	double backNeutronToFErg[MAX_MULTIPLICITY] = {0};
	double backNeutronDet[MAX_MULTIPLICITY] = {0};
	double backNeutronVx[MAX_MULTIPLICITY] = {0};
	double backNeutronVy[MAX_MULTIPLICITY] = {0};
	double backNeutronVz[MAX_MULTIPLICITY] = {0};

	double backPhotonDetTimes[MAX_MULTIPLICITY] = {0};
	double backPhotonLightOut[MAX_MULTIPLICITY] = {0};
	double backPhotonPSD[MAX_MULTIPLICITY] = {0};
	double backPhotonDet[MAX_MULTIPLICITY] = {0};
	double backPhotonVx[MAX_MULTIPLICITY] = {0};
	double backPhotonVy[MAX_MULTIPLICITY] = {0};
	double backPhotonVz[MAX_MULTIPLICITY] = {0};


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
	DetectorSystemClass(TChain* treeIn, TFile* writeFile, InfoSystem* info);
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
	//virtual void     TriggerAnalysis();
	virtual int      DetectionAnalysis();
	virtual void     SystemAnalysis();
	virtual void 		 FissionAnalysis();
	virtual void     FissionAnalysisLoop();

	// mapping functions
	int isDetector(int detectorNumber);
	int isChamber(int detectorNumber);

};
#endif
