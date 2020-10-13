/*
Authors: Stefano Marin, Isabel Hernandez
Purpose: Forms the coincidence tree
Date: Ann Arbor, May 6th, 2020
*/

#ifndef CoincidenceAnalysis_h
#define CoincidenceAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "InfoSystem.h"
#include "DigitizerBranchClass.h"

using namespace std;

class CoincidenceAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   //create TTree object to contain the output coincidence tree
   TTree* coincTree;;

   // output file
   TFile* expFile = 0;
   TDirectory* fileTreeDir = 0;

   // declare the digitizer classes
   int DATA_TYPE = 1;
   // 0: CoMPASS
   // 1: MIDAS

   COMPASS_DIG* cp = new COMPASS_DIG();
   MIDAS_DIG* md = new MIDAS_DIG();

   TString compassName = "Data_F";
   TString midasName = "midas_data";

   TString inputTreeName;

   //infosystem attributes (not info)
   int NUM_TRIGGERS = 0;
   int NUM_DETS = 0;

   double TRIGGER_THRESHOLD = 0;
   double TRIGGER_CLIP = 0;
   double MAX_TRIGGER_DRIFT = 0;
   double COINC_WINDOW = 0;

   int* FISSION_TRIGGERS;
   int* DETECTORS;


   CoincidenceAnalysis(TString filename, int fileNum, TFile* expFileWrite, TTree* tree = 0, InfoSystem* info = 0);
   virtual ~CoincidenceAnalysis();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual int   CreateCoincidenceTree(int fileNum, Long64_t entriesToProc = -1);
};

#endif

#ifdef CoincidenceAnalysis_cxx

CoincidenceAnalysis::CoincidenceAnalysis(TString filename, int fileNum, TFile* expFileWrite, TTree* tree, InfoSystem* info) : fChain(0)
{
  DATA_TYPE = info->DATA_TYPE;
  TRIGGER_THRESHOLD = info->TRIGGER_THRESHOLD;
  TRIGGER_CLIP = info->TRIGGER_CLIP;
  MAX_TRIGGER_DRIFT = info->MAX_TRIGGER_DRIFT;
  COINC_WINDOW = info->COINC_WINDOW;

  NUM_TRIGGERS = info->NUM_TRIGGERS;
  NUM_DETS = info->NUM_DETS;
  FISSION_TRIGGERS = info->FISSION_TRIGGERS;
  DETECTORS = info->DETECTORS;

   // set the output stream
   expFile = expFileWrite;

   TString treeNumT = TString(to_string(fileNum));
   fileTreeDir = expFile->mkdir(treeNumT);

   // set the input tree
   if(DATA_TYPE == 0)
   {
      inputTreeName = compassName;
   }
   else if(DATA_TYPE == 1)
   {
      inputTreeName = midasName;
   }
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
      if (!f || !f->IsOpen()) {
         f = new TFile(filename);
      }
      f->GetObject(inputTreeName,tree);

      cout << "tree found at " << tree << endl;
   }
   Init(tree);

}

CoincidenceAnalysis::~CoincidenceAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   delete coincTree;

   delete FISSION_TRIGGERS;
   delete DETECTORS;
}

Int_t CoincidenceAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t CoincidenceAnalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
   }
   return centry;
}

void CoincidenceAnalysis::Init(TTree *tree)
{

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   if(DATA_TYPE == 0)
   {
      fChain->SetBranchAddress("Board", &(cp->Board), &(cp->b_Board) );
      fChain->SetBranchAddress("Channel", &(cp->Channel), &(cp->b_Channel) );
      fChain->SetBranchAddress("Timestamp", &(cp->Timestamp), &(cp->b_Timestamp) );
      fChain->SetBranchAddress("Energy", &(cp->Energy), &(cp->b_Energy) );
      fChain->SetBranchAddress("EnergyShort", &(cp->EnergyShort) , &(cp->b_EnergyShort) );
      fChain->SetBranchAddress("Flags", &(cp->Flags), &(cp->b_Flags) );
   }

   cout << "Branches of input tree have been initialized" << endl;

   if(DATA_TYPE == 1)
   {
      fChain->SetBranchAddress("interpolation", &(md->interpolation), &(md->b_interpolation) );
      fChain->SetBranchAddress("time", &(md->time), &(md->b_time) );
      fChain->SetBranchAddress("detector", &(md->detector), &(md->b_detector) );
      fChain->SetBranchAddress("baseline", &(md->baseline), &(md->b_baseline) ) ;
      fChain->SetBranchAddress("total", &(md->total), &(md->b_total) );
      fChain->SetBranchAddress("head", &(md->head) , &(md->b_head) );
      fChain->SetBranchAddress("tail", &(md->tail), &(md->b_tail) );
   }

}

#endif // #ifdef CoincidenceAnalysis_cxx
