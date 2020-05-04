//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 23 10:50:07 2020 by ROOT version 6.18/04
// from TTree Data_F/CoMPASS filtered events TTree
// found on file: compassCf.root
//////////////////////////////////////////////////////////

#ifndef FissionAnalysis_h
#define FissionAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Header file for the classes stored in the TTree if any.

class FissionAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // output file
   TFile* expFile = 0;
   TDirectory* fileTreeDir = 0;

   // Declaration of leaf types
   UShort_t        Board;
   UShort_t        Channel;
   ULong64_t       Timestamp;
   UShort_t        Energy;
   UShort_t        EnergyShort;
   UInt_t          Flags;

   // List of branches
   TBranch        *b_Board;   //!
   TBranch        *b_Channel;   //!
   TBranch        *b_Timestamp;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_EnergyShort;   //!
   TBranch        *b_Flags;   //!

   FissionAnalysis(TString filename, int fileNum, TFile* expFileWrite, TTree *tree=0);
   virtual ~FissionAnalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual int   CreateFissionTree(int fileNum, Long64_t entriesToProc = -1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef FissionAnalysis_cxx
FissionAnalysis::FissionAnalysis(TString filename, int fileNum, TFile* expFileWrite, TTree *tree) : fChain(0)
{

   // set the output stream

   expFile = expFileWrite;

   TString treeNumT = TString(to_string(fileNum));
   fileTreeDir = expFile->mkdir(treeNumT);

   // set the input tree

   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
      if (!f || !f->IsOpen()) {
         f = new TFile(filename);
      }
      f->GetObject("Data_F",tree);

      //cout << "tree found at " << tree << endl;
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
      Notify();
   }
   return centry;
}

void FissionAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Board", &Board, &b_Board);
   fChain->SetBranchAddress("Channel", &Channel, &b_Channel);
   fChain->SetBranchAddress("Timestamp", &Timestamp, &b_Timestamp);
   fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
   fChain->SetBranchAddress("EnergyShort", &EnergyShort, &b_EnergyShort);
   fChain->SetBranchAddress("Flags", &Flags, &b_Flags);
}

Bool_t FissionAnalysis::Notify()
{
   return kTRUE;
}

void FissionAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t FissionAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef FissionAnalysis_cxx
