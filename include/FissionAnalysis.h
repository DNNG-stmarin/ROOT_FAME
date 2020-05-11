/*
Authors: Stefano Marin, Isabel Hernandez
Purpose: Forms the coincidence tree
Date: Ann Arbor, May 6th, 2020
*/

#ifndef FissionAnalysis_h
#define FissionAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "DigitizerBranchClass.h"

using namespace std;

class FissionAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // output file
   TFile* expFile = 0;
   TDirectory* fileTreeDir = 0;

   // declare the digitizer classes
   int digType = 1;
   // 0: CoMPASS
   // 1: MIDAS

   COMPASS_DIG* cp = new COMPASS_DIG();
   MIDAS_DIG* md = new MIDAS_DIG();

   TString compassName = "Data_F";
   TString midasName = "midas_data";

   TString inputTreeName;


   FissionAnalysis(TString filename, int fileNum, TFile* expFileWrite, int digTypeIn, TTree *tree=0);
   virtual ~FissionAnalysis();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual int   CreateFissionTree(int fileNum, Long64_t entriesToProc = -1);
};

#endif

#ifdef FissionAnalysis_cxx
FissionAnalysis::FissionAnalysis(TString filename, int fileNum, TFile* expFileWrite, int digTypeIn, TTree *tree) : fChain(0)
{

   // set the output stream
   expFile = expFileWrite;

   TString treeNumT = TString(to_string(fileNum));
   fileTreeDir = expFile->mkdir(treeNumT);

   // set the input tree
   digType = digTypeIn;
   if(digType == 0)
   {
      inputTreeName = compassName;
   }
   else if(digType == 1)
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

FissionAnalysis::~FissionAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t FissionAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t FissionAnalysis::LoadTree(Long64_t entry)
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

void FissionAnalysis::Init(TTree *tree)
{

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   if(digType == 0)
   {
      fChain->SetBranchAddress("Board", &(cp->Board), &(cp->b_Board) );
      fChain->SetBranchAddress("Channel", &(cp->Channel), &(cp->b_Channel) );
      fChain->SetBranchAddress("Timestamp", &(cp->Timestamp), &(cp->b_Timestamp) );
      fChain->SetBranchAddress("Energy", &(cp->Energy), &(cp->b_Energy) );
      fChain->SetBranchAddress("EnergyShort", &(cp->EnergyShort) , &(cp->b_EnergyShort) );
      fChain->SetBranchAddress("Flags", &(cp->Flags), &(cp->b_Flags) );
   }

   cout << "Branches of input tree have been initialized" << endl;

   if(digType == 1)
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

#endif // #ifdef FissionAnalysis_cxx
