//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 23 12:11:00 2022 by ROOT version 6.24/06
// from TTree et/event tree from waveform analysis
// found on file: event12520.root
//////////////////////////////////////////////////////////

#ifndef fragFiss_h
#define fragFiss_h

#include "InfoSystem.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class fragFiss {
public :
   TChain*         eventChain;
   TFile*          eventFile;
   Long64_t        nentries;

   int             numFiles;
   Long64_t*       nentriesTree;

   TTree*          fragTree;
   TFile*          fragFile;

   InfoSystem*     infoSystem;

   TGraph*         g_sawtooth;
   TGraph*         g_bindErg;
   TGraph*         g_phd;
   TGraph*         g_fpy;
   TGraph*         g_tke;

   Int_t           fCurrent; //!current Tree number in a TChain
   TString         inputTreeName = "et";
   TString         eventFilename = "default";
   TString         extEventfile = ".root";

   // angular analysis
   TGraph* g_Ang1;
   TGraph* g_Ang2;

   TF1* f_ang1;
   TF1* f_ang2;

   // attenuation analysis

   TF1* f_att1;
   TF1* f_att2;

   TF1* f_att1L;
   TF1* f_att1H;
   TF1* f_att2L;
   TF1* f_att2H;

   TF1* f_sepAtt1;
   TF1* f_sepAtt2;

   // calibration analysis
   TGraph* g_gainMatch;
   TGraph* g_calib1;
   TGraph* g_calib2;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        t0t;
   Double_t        ct;
   Double_t        dt;
   Double_t        cph;
   Double_t        tTrig[8];
   Double_t        aph[2];
   Double_t        gph[2];
   Double_t        sumph[2];
   Double_t        sumtTrig[2];
   Int_t           sumtPeak[2];
   Bool_t          ccoinc;
   Short_t         a0wf[1280];
   Short_t         a1wf[1280];
   Short_t         g0wf[1280];
   Short_t         g1wf[1280];
   Short_t         sum0wf[1280];
   Short_t         sum1wf[1280];
   Short_t         cwf[1280];

   // List of branches
   TBranch        *b_t0t;   //!
   TBranch        *b_ct;   //!
   TBranch        *b_dt;   //!
   TBranch        *b_cph;   //!
   TBranch        *b_tTrig;   //!
   TBranch        *b_aph;   //!
   TBranch        *b_gph;   //!
   TBranch        *b_sumph;   //!
   TBranch        *b_sumtTrig;   //!
   TBranch        *b_sumtPeak;   //!
   TBranch        *b_ccoinc;   //!
   TBranch        *b_a0wf;   //!
   TBranch        *b_a1wf;   //!
   TBranch        *b_g0wf;   //!
   TBranch        *b_g1wf;   //!
   TBranch        *b_sum0wf;   //!
   TBranch        *b_sum1wf;   //!
   TBranch        *b_cwf;   //!

   fragFiss(InfoSystem* infoIn, TString fileName);
   virtual ~fragFiss();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init();

   virtual void     PlotWaves();


   virtual void     AngleAnalysis();
   virtual void     ELossCorrection();
   virtual void     GainMatching();
   virtual void     FillFragTree();


   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif
