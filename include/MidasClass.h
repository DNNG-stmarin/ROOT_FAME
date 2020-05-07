//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May  1 18:19:09 2020 by ROOT version 6.18/04
// from TTree midas_data/MIDAS data
// found on file: output_054941.root
//////////////////////////////////////////////////////////

#ifndef MidasClass_h
#define MidasClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MidasClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // output file
   TFile* expFile = 0;
   TDirectory* fileTreeDir = 0;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         interpolation;
   ULong64_t       time;
   Int_t           detector;
   Int_t           baseline;
   Int_t           total;
   Int_t           head;
   Int_t           tail;

   // List of branches
   TBranch        *b_interpolation;   //!
   TBranch        *b_time;   //!
   TBranch        *b_detector;   //!
   TBranch        *b_baseline;   //!
   TBranch        *b_total;   //!
   TBranch        *b_head;   //!
   TBranch        *b_tail;   //!

   MidasClass(TString filename, int fileNum, TFile* expFileWrite, TTree *tree=0);
   virtual ~MidasClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual int      CreateFissionTree(int fileNum, Long64_t entriesToProc = -1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MidasClass_cxx
MidasClass::MidasClass(TString filename, int fileNum, TFile* expFileWrite, TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output_054941.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("output_054941.root");
      }
      f->GetObject("midas_data",tree);

   }
   Init(tree);
}

MidasClass::~MidasClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MidasClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MidasClass::LoadTree(Long64_t entry)
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

void MidasClass::Init(TTree *tree)
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

   fChain->SetBranchAddress("interpolation", &interpolation, &b_interpolation);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("detector", &detector, &b_detector);
   fChain->SetBranchAddress("baseline", &baseline, &b_baseline);
   fChain->SetBranchAddress("total", &total, &b_total);
   fChain->SetBranchAddress("head", &head, &b_head);
   fChain->SetBranchAddress("tail", &tail, &b_tail);
   Notify();
}

Bool_t MidasClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MidasClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MidasClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MidasClass_cxx
