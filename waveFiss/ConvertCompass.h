//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 21 18:02:39 2022 by ROOT version 6.22/08
// from TTree Data_F/CoMPASS filtered events TTree
// found on file: compassF_CfOvernight_1_0.root
//////////////////////////////////////////////////////////

#ifndef ConvertCompass_h
#define ConvertCompass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class ConvertCompass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

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

   ConvertCompass(TTree *tree=0);
   virtual ~ConvertCompass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ConvertCompass_cxx
ConvertCompass::ConvertCompass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("compassF_CfOvernight_1_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("compassF_CfOvernight_1_0.root");
      }
      f->GetObject("Data_F",tree);

   }
   Init(tree);
}

ConvertCompass::~ConvertCompass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ConvertCompass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ConvertCompass::LoadTree(Long64_t entry)
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

void ConvertCompass::Init(TTree *tree)
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
   Notify();
}

Bool_t ConvertCompass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ConvertCompass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ConvertCompass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ConvertCompass_cxx
