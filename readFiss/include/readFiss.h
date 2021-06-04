//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 17 09:51:08 2021 by ROOT version 5.34/38
// from TTree Fiss/Fiss
// found on file: Fiss.root
//////////////////////////////////////////////////////////

#ifndef readFiss_h
#define readFiss_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TMatrixD.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphErrors.h>

#include <iostream>
#include <fstream>
// Header file for the classes stored in the TTree if any.

#include "Constants.h"

class readFiss {
public :

/*
  _____                _      _    _
 |_   _| _ ___ ___    /_\  __| |__| |_ _ ___ ______ ___ ___
   | || '_/ -_) -_)  / _ \/ _` / _` | '_/ -_|_-<_-</ -_|_-<
   |_||_| \___\___| /_/ \_\__,_\__,_|_| \___/__/__/\___/__/
                                                           */
   TChain*           expTree;   //!pointer to the analyzed TTree or TChain
   TChain*           simTree;
   // TTree*           beamTree;
   TString          nameExp; // String name of where to find experiment
   TString          nameSim;
   TString          nameBeam;

   TFile*           expFile;
   TFile*           simFile;
   TFile*           beamFile;

   int              numExpFiles;
   int              numSimFiles;

   Int_t            fCurrent; //!current Tree number in a TChain

   TFile*           writeFile;
   TDirectory*      cd_basics;
   TDirectory*      cd_individual;
   TDirectory*      cd_simComparison;
   TDirectory*      cd_FAME;
   TDirectory*      cd_correlated;
   TDirectory*      cd_beam;

   int NUM_TRIGGERS; // number of trigger channels
   int* TRIGGERS;


  /*
   ___                       _
  | _ \_ _ ___  __ ___ _____(_)_ _  __ _
  |  _/ '_/ _ \/ _/ -_|_-<_-< | ' \/ _` |
  |_| |_| \___/\__\___/__/__/_|_||_\__, |
                                   |___/
  */
  // objects specific to this run of the analysis
  int mode;                     // what mode this run is using
  bool CovEM_in;                // whether or not this run uses CovEM

  int BN;                       // CovEM setting
  int BP;                       // CovEM setting
  double MAX_TIME_N;
  double THRESHOLD;
  double THRESHOLD_DEP;
  double BACKGROUND_DELAY;

  double BEAM_ERG_MIN, BEAM_ERG_MAX; // range of the beam energies to be employed
  double BEAM_ERG_BINNUM;            // number of energy bins in range

  double MIN_N_ERG, MAX_N_ERG;  // CovEM setting
  double MIN_P_ERG, MAX_P_ERG;  // CovEM setting

  TString rootEnding = ".root";
  TString nameExpTree = "Fiss";
  TString nameSimTree = "fissionTree";


  // non-user information
  long int expEntries;
  long int simEntries;

  double intWindowAlpha = 1e5; // ns integration window
  double intWindowFiss;


 /*
  _  _ _    _
 | || (_)__| |_ ___  __ _ _ _ __ _ _ __  ___
 | __ | (_-<  _/ _ \/ _` | '_/ _` | '  \(_-<
 |_||_|_/__/\__\___/\__, |_| \__,_|_|_|_/__/
                    |___/                    */

   // matrix variables
   TH2D* arrayCorr;
   TH2D* arraySpec;

   // n LO
   TH1D* neutronLightOutputSim;
   TH1D* neutronLightOutputExp;
   TH1D* neutronLightOutputBack;
   // p LO
   TH1D* photonLightOutputSim;
   TH1D* photonLightOutputExp;
   TH1D* photonLightOutputBack;

   // n ToF
   TH1D* neutronTofSim;
   TH1D* neutronTofExp;
   TH1D* neutronTofBack;

   // p ToF
   TH1D* photonTofSim;
   TH1D* photonTofExp;
   TH1D* photonTofBack;

   // n Erg
   TH1D* neutronEnergySim;
   TH1D* neutronEnergyExp;
   TH1D* neutronEnergyBack;

   // n Mult
   TH1I* neutronMultExp;
   TH1I* neutronMultSim;
   TH1I* neutronMultBack;

   // p Mult
   TH1I* photonMultExp;
   TH1I* photonMultSim;
   TH1I* photonMultBack;

   // n PSD
   TH1D* neutronPSDExp;
   TH1D* neutronPSDSim;
   TH1D* neutronPSDBack;

   // p PSD
   TH1D* photonPSDExp;
   TH1D* photonPSDSim;
   TH1D* photonPSDBack;

   // n det
   TH1D* neutronSinglesExp;
   TH1D* neutronSinglesSim;
   TH1D* neutronSinglesBack;

   // p det
   TH1D* photonSinglesExp;
   TH1D* photonSinglesSim;
   TH1D* photonSinglesBack;

   // correlated histograms - add Exp to the end of these
   TH2I* neutronGammaMultExp;
   TH2D* neutronMultPhotonLOExp;
   TH2D* neutronEnergyLOExp;
   TH2D* neutronLightOutPSDExp;
   TH2D* photonLightOutPSDExp;

   TH2D* neutronDoublesMat;
   TH2D* neutronSinglesMat;
   TH1D* neutronAngleCorr;

   // alphaFile histograms
   TH1D** h_alphaDep;
   TH1I* h_macroPop;

   // beam histograms
   TH1D** h_fisDep;
   TH1D** h_fisSubtract;
   TH2D** h2_fisDepErg;
   TH1D** h_beamTime;

   TH2D** h2_neutronMultDep;
   TH2D** h2_gammaMultDep;
   TH2D** h2_backNeutronMultDep;
   TH2D** h2_backGammaMultDep;

   TH2D** h2_neutronMultErg;
   TH2D** h2_gammaMultErg;
   TH2D** h2_backNeutronMultErg;
   TH2D** h2_backGammaMultErg;

   // visual settings
   Float_t legend_x1;
   Float_t legend_x2;
   Float_t legend_y1;
   Float_t legend_y2;

   Float_t x_labelSize;
   Float_t y_labelSize;

   Float_t x_tickSize;
   Float_t y_tickSize;


/*
   ___          __ _ _
  | _ \_ _ ___ / _(_) |___ ___
  |  _/ '_/ _ \  _| | / -_|_-<
  |_| |_| \___/_| |_|_\___/__/

*/

  TProfile** p_neutronMultDep;				                   //Profile neutronMult vs fisDep
  TProfile** p_gammaMultDep;				                     //Profile gammaMult vs fisDep
  TProfile** p_backNeutronMultDep;                       //Profile backNeutronMult vs fisDep
	TProfile** p_backGammaMultDep;                         //Profile backGammaMult vs fisDep

  TGraph** g_fisRatioThreshold;                          // fis ratio for different dep threshold
  TGraph** g_fisRatioSelect;                             // fis ratio for different dep threshold

  TGraph** g_neutronMultRatioDep;
  TGraph** g_gammaMultRatioDep;


  TProfile** p_neutronMultErg;				                   //Profile neutronMult vs beamEnergy
  TProfile** p_gammaMultErg;				                     //Profile gammaMult vs beamEnergy
  TProfile** p_backNeutronMultErg;                       //Profile backNeutronMult vs beamEnergy
	TProfile** p_backGammaMultErg;                         //Profile backGammaMult vs beamEnergy

  TGraph** g_fisRatioErg;                                // fission triggers / total vs. beamEnergy
  TGraph** g_nMultErg;                                   // Background-subtracted nMult vs. beamEnergy
  TGraph** g_gMultErg;                                   // Background-subtracted gMult vs. beamEnergy
  TGraph** g_nMultBackErg;                               // Background-subtracted nBackMult vs. beamEnergy
  TGraph** g_gMultBackErg;                               // Background-subtracted gBackMult vs. beamEnergy
  TGraph** g_gMultnMult;                                 // Background-subtracted gBackMult vs. beamEnergy

  TGraphErrors* g_gnRatio;                               // ave gamma-neutron slope ratio due to PPAC
  TGraphErrors* g_nSlope;                                // ave neutron slope due to PPAC
  TGraphErrors* g_gSlope;                                // ave gamma slope due to PPAC


/*
   ___                   ___
  | _ ) ___ __ _ _ __   | _ \__ _ _ _  __ _ ___ ___
  | _ \/ -_) _` | '  \  |   / _` | ' \/ _` / -_|_-<
  |___/\___\__,_|_|_|_| |_|_\__,_|_||_\__, \___/__/
                                      |___/
*/

  double DEP_MIN = 0;       // Min energy for alpha fit
  double DEP_MAX = 0.05;    // Max energy for alpha fit

  double FIS_MIN = 0.005;   // Min energy for fission fit
  double FIS_MAX = 0.05;    // Max energy for fission fit

/*
 ___             _   _
| __|  _ _ _  __| |_(_)___ _ _  ___
| _| || | ' \/ _|  _| / _ \ ' \(_-<
|_| \_,_|_||_\__|\__|_\___/_||_/__/

*/

   TF1* f_TimeFromErg = new TF1("f_TimeFromErg", "[0] / [1] * ([2] + x) / sqrt(x * (2 * [2] + x))", 0, 20);

   // Fit Functions
   TF1** f_alpha;// = new TF1("f_alpha", "expo", DEP_MIN, DEP_MAX);  // Define expo fit for alpha background to start at middle bin with most counts
   TF1** f_expo;// = new TF1("f_expo", "expo");
   TF1** f_fisProducts;// = new TF1("f_fisProducts", "gaus", FIS_MIN, FIS_MAX);  // Define gaus fit function for after binning errors in low values of fisDep
   TF1** f_gauss;// = new TF1("f_gauss", "gaus");
   TF1** f_aveGmult; // = new TF1((TString)"f_aveGmult" + (TString)to_string(indexChannel), "pol1");
   TF1** f_aveNmult; // = new TF1((TString)"f_aveNmult" + (TString)to_string(indexChannel), "pol1");



/*
              _____              ___                  _
  _____ ___ _|_   _| _ ___ ___  | _ )_ _ __ _ _ _  __| |_  ___ ___
 / -_) \ / '_ \| || '_/ -_) -_) | _ \ '_/ _` | ' \/ _| ' \/ -_|_-<
 \___/_\_\ .__/|_||_| \___\___| |___/_| \__,_|_||_\__|_||_\___/__/
         |_|                                                       */
   // Declaration of leaf types
   Double_t        fisTime;
   Double_t        fisDep;
   Int_t           fisChan;
   Double_t        fisPSP;
   Int_t           neutronMult;
   Int_t           gammaMult;
   Int_t           neutronBackMult;
   Int_t           gammaBackMult;
   Double_t        neutronDetTimes[MAX_MULT];   //[neutronMult]
   Double_t        neutronLightOut[MAX_MULT];   //[neutronMult]
   Double_t        neutronPSD[MAX_MULT];   //[neutronMult]
   Double_t        neutronToFErg[MAX_MULT];   //[neutronMult]
   Double_t        neutronDet[MAX_MULT];   //[neutronMult]
   Double_t        neutronVx[MAX_MULT];   //[neutronMult]
   Double_t        neutronVy[MAX_MULT];   //[neutronMult]
   Double_t        neutronVz[MAX_MULT];   //[neutronMult]
   Double_t        photonDetTimes[MAX_MULT];   //[gammaMult]
   Double_t        photonLightOut[MAX_MULT];   //[gammaMult]
   Double_t        photonPSD[MAX_MULT];   //[gammaMult]
   Double_t        photonDet[MAX_MULT];   //[gammaMult]
   Double_t        photonVx[MAX_MULT];   //[gammaMult]
   Double_t        photonVy[MAX_MULT];   //[gammaMult]
   Double_t        photonVz[MAX_MULT];   //[gammaMult]
   Double_t        backNeutronDetTimes[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronLightOut[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronPSD[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronToFErg[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronDet[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronVx[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronVy[MAX_MULT];   //[neutronBackMult]
   Double_t        backNeutronVz[MAX_MULT];   //[neutronBackMult]
   Double_t        backPhotonDetTimes[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonLightOut[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonPSD[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonDet[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonVx[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonVy[MAX_MULT];   //[gammaBackMult]
   Double_t        backPhotonVz[MAX_MULT];   //[gammaBackMult]

   // beam specific branches
   Double_t        beamTime;
   Double_t        beamEnergy;
   Double_t        beamDep;
   Double_t        beamPSP;
   Int_t           beamChan;
   Int_t           beamIndex;

   // List of branches
   TBranch        *b_fisTime;   //!
   TBranch        *b_fisDep;   //!
   TBranch        *b_fisChan;   //!
   TBranch        *b_fisPSP;   //!
   TBranch        *b_neutronMult;   //!
   TBranch        *b_gammaMult;   //!
   TBranch        *b_neutronBackMult;   //!
   TBranch        *b_gammaBackMult;   //!
   TBranch        *b_neutronDetTimes;   //!
   TBranch        *b_neutronLightOut;   //!
   TBranch        *b_neutronPSD;   //!
   TBranch        *b_neutronToFErg;   //!
   TBranch        *b_neutronDet;   //!
   TBranch        *b_neutronVx;   //!
   TBranch        *b_neutronVy;   //!
   TBranch        *b_neutronVz;   //!
   TBranch        *b_photonDetTimes;   //!
   TBranch        *b_photonLightOut;   //!
   TBranch        *b_photonPSD;   //!
   TBranch        *b_photonDet;   //!
   TBranch        *b_photonVx;   //!
   TBranch        *b_photonVy;   //!
   TBranch        *b_photonVz;   //!
   TBranch        *b_backNeutronDetTimes;   //!
   TBranch        *b_backNeutronLightOut;   //!
   TBranch        *b_backNeutronPSD;   //!
   TBranch        *b_backNeutronToFErg;   //!
   TBranch        *b_backNeutronDet;   //!
   TBranch        *b_backNeutronVx;   //!
   TBranch        *b_backNeutronVy;   //!
   TBranch        *b_backNeutronVz;   //!
   TBranch        *b_backPhotonDetTimes;   //!
   TBranch        *b_backPhotonLightOut;   //!
   TBranch        *b_backPhotonPSD;   //!
   TBranch        *b_backPhotonDet;   //!
   TBranch        *b_backPhotonVx;   //!
   TBranch        *b_backPhotonVy;   //!
   TBranch        *b_backPhotonVz;   //!

   // beam specific branches
   TBranch        *b_beamTime;   //!
   TBranch        *b_beamEnergy;   //!
   TBranch        *b_beamDep;   //!
   TBranch        *b_beamPSP;   //!
   TBranch        *b_beamChan;   //!
   TBranch        *b_beamIndex;   //!


   /*
     _     _____              ___                  _
  __(_)_ _|_   _| _ ___ ___  | _ )_ _ __ _ _ _  __| |_  ___ ___
 (_-< | '  \| || '_/ -_) -_) | _ \ '_/ _` | ' \/ _| ' \/ -_|_-<
 /__/_|_|_|_|_||_| \___\___| |___/_| \__,_|_||_\__|_||_\___/__/ */
 // Declaration of leaf types
   Double_t        neutronHeight[MAX_MULT];   //[neutronMult] not defined
   Double_t        photonHeight[MAX_MULT];   //[gammaMult]
   Int_t           neutronFlag[MAX_MULT];   //[neutronMult] not defined
   Int_t           photonFlag[MAX_MULT];   //[gammaMult]   not defined

   Int_t           neutronChannel[MAX_MULT];   //[neutronMult], different name neutronDet
   Double_t        neutronEnergy[MAX_MULT];   //[neutronMult] different name neutronToFErg
   Double_t        neutronIntegral[MAX_MULT];   //[neutronMult] different name neutronLightOut
   Int_t           photonChannel[MAX_MULT];   //[gammaMult]
   Double_t        photonEnergy[MAX_MULT];   //[gammaMult]
   Double_t        photonIntegral[MAX_MULT];   //[gammaMult]



   // List of branches ADD EXP OR SIM AT THE END
   TBranch *b_neutronMultSim;   //!
   TBranch *b_gammaMultSim;   //!
   TBranch *b_neutronChannelSim;   //!
   TBranch *b_neutronDetTimesSim;   //!
   TBranch *b_neutronEnergySim;   //!
   TBranch *b_neutronIntegralSim;   //!
   TBranch *b_neutronHeightSim;   //!
   TBranch *b_neutronPSDSim;   //!
   TBranch *b_photonChannelSim;   //!
   TBranch *b_photonDetTimesSim;   //!
   TBranch *b_photonEnergySim;   //!
   TBranch *b_photonIntegralSim;   //!
   TBranch *b_photonHeightSim;   //!
   TBranch *b_photonPSDSim;   //!
   TBranch *b_neutronFlagSim;   //!
   TBranch *b_photonFlagSim;   //!

/*
  __  __     _   _            _
 |  \/  |___| |_| |_  ___  __| |___
 | |\/| / -_)  _| ' \/ _ \/ _` (_-<
 |_|  |_\___|\__|_||_\___/\__,_/__/*/

  // constructors
   readFiss(int &argc, char** &argv);
   virtual ~readFiss();

   // create a menu
   virtual void GetInfo(std::istream &inputStream);
   virtual void BadInputMessage();

   // set thresholds and time limit
   virtual void SetRunThresholds(double threshold, double max_time_n);
   virtual void SetEnergyLimits(double min_n_erg, double max_n_erg, double min_p_erg, double max_p_erg);

   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetExpEntry(Long64_t entry);
   virtual Int_t    GetSimEntry(Long64_t entry);
   virtual Long64_t LoadExpTree(Long64_t entry);
   virtual Long64_t LoadSimTree(Long64_t entry);

   virtual void     InitExp(TChain *tree);
   virtual void     InitSim(TChain *tree);

   virtual Bool_t   Notify();
   virtual void     ShowExp(Long64_t entry = -1);
   virtual void     ShowSim(Long64_t entry = -1);

   virtual void     ReadBeamInfo();

   // loop through data
   virtual void     LoopExp();
   virtual void     LoopSim();
   virtual void     LoopBeam();

   // perform FAME analysis
   virtual void     SetBNBP(int BN, int BP); // set the number of bins for the covariance analysis
   virtual void     CovEM(); // number of energy bins in neutron and photon energies
   virtual void     WriteCovEM();
   virtual void     analyseCovEM(); // number of

   // perform beam analysis
   virtual void     BeamDepAnalysis();
   virtual void     BeamErgAnalysis();
   virtual void     FitMult();


   // initialization functions
   virtual void     InitializeHistograms();
   virtual void     InitializeFunctions();
   virtual int      isTrigger(int triggerNumber);

   // callable functions
   virtual void     PlotAll();
   virtual void     CompareAll();

   // plot the experiment branches
   virtual void     PlotTof();
   virtual void     PlotLightOut();
   virtual void     PlotErg();
   virtual void     PlotPSD();
   virtual void     PlotMult();
   virtual void     PlotSingles();
   // correlated
   virtual void     PlotMultCorExp();
   virtual void     PlotMultLOExp();
   virtual void     PlotEnergyLOExp();
   virtual void     PlotN_LOPSD_Exp();
   virtual void     PlotP_LOPSD_Exp();

   // plot the beam parameters
   virtual void     PlotDepSubtraction();
   virtual void     PlotRatioMult();
   virtual void     PlotMultErg();

   // plot the experiment vs simulated branches
   virtual void     CompareTof();
   virtual void     CompareLightOut();
   virtual void     CompareErg();
   virtual void     ComparePSD();
   virtual void     CompareMult();
   virtual void     CompareSingles();
};

#endif
