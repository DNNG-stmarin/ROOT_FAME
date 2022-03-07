//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug 24 10:55:31 2021 by ROOT version 6.22/08
// from TTree t/CAEN digital output tree
// found on file: run12520.root
//////////////////////////////////////////////////////////

#ifndef visualizeRun_h
#define visualizeRun_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class visualizeRun {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UChar_t         bnum;
   UChar_t         chnum;
   UInt_t          ts;
   Short_t         wf[1280];

   // List of branches
   TBranch        *b_bnum;   //!
   TBranch        *b_chnum;   //!
   TBranch        *b_ts;   //!
   TBranch        *b_wf;   //!

   visualizeRun(TTree *tree=0);
   virtual ~visualizeRun();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     PlotWavePlots();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef visualizeRun_cxx
visualizeRun::visualizeRun(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("mock12520.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("mock12520.root");
      }
      f->GetObject("t",tree);

   }
   Init(tree);
}

visualizeRun::~visualizeRun()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t visualizeRun::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t visualizeRun::LoadTree(Long64_t entry)
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

void visualizeRun::Init(TTree *tree)
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

   fChain->SetBranchAddress("bnum", &bnum, &b_bnum);
   fChain->SetBranchAddress("chnum", &chnum, &b_chnum);
   fChain->SetBranchAddress("ts", &ts, &b_ts);
   fChain->SetBranchAddress("wf[1280]", wf, &b_wf);
   Notify();
}

Bool_t visualizeRun::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void visualizeRun::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t visualizeRun::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef visualizeRun_cxx
