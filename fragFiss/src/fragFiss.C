#include "fragFiss.h"

fragFiss::fragFiss(InfoSystem* infoIn, TString fileName) : eventChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  infoSystem = infoIn;
  cout << "Creating output file" << endl;
  fragFile = new TFile("frag.root", "RECREATE");
  eventChain = new TChain(inputTreeName, inputTreeName);

   for(int i = 0; i < infoSystem->NUM_FILES; i++)
   {
      int fileNum = (infoSystem->FILE_LIST)[i];

      cout << "Reading file number " << fileNum << endl;
      // find the file

      TString s_eventFile =  fileName + TString(to_string(fileNum)) + extEventfile;
      eventFile = (TFile*)gROOT->GetListOfFiles()->FindObject(s_eventFile);

      if (!eventFile || !eventFile ->IsOpen())
      {
        eventFile = new TFile(s_eventFile);
      }
      else
      {
        cout << "ERROR: Failed to open event file" << endl;
      }

      TTree* eventTree;
      eventFile->GetObject(inputTreeName, eventTree);

      if(eventTree != 0)
      {
        eventChain->Add(s_eventFile + "/" + inputTreeName);
      }
      //eventFile->Close();
      delete eventFile;
   }
   Init();

   nentries = eventChain->GetEntries();
   cout << "Analyzing " << nentries << " events" << endl;

   // Initialize sawtooth pointer
   g_sawtooth = new TGraph(infoIn->SAWTOOTH_FILENAME);

}

fragFiss::~fragFiss()
{
   if (!eventChain) return;
   delete eventChain->GetCurrentFile();
   // fragFile->Close();
   delete eventChain;
   delete eventFile;

   delete fragTree;
   delete fragFile;

   delete infoSystem;

   cout << "Finished deleting" << endl;

}

Int_t fragFiss::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!eventChain) return 0;
   return eventChain->GetEntry(entry);
}
Long64_t fragFiss::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!eventChain) return -5;
   Long64_t centry = eventChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (eventChain->GetTreeNumber() != fCurrent) {
      fCurrent = eventChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void fragFiss::Init()
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   cout << "Init has been called" << endl;
   // if (!tree) return;
   // eventChain = tree;
   fCurrent = -1;
   eventChain->SetMakeClass(1);

   eventChain->SetBranchAddress("t0t", &t0t, &b_t0t);
   eventChain->SetBranchAddress("ct", &ct, &b_ct);
   eventChain->SetBranchAddress("dt", &dt, &b_dt);
   eventChain->SetBranchAddress("cph", &cph, &b_cph);
   eventChain->SetBranchAddress("tTrig[8]", tTrig, &b_tTrig);
   eventChain->SetBranchAddress("aph[2]", aph, &b_aph);
   eventChain->SetBranchAddress("gph[2]", gph, &b_gph);
   eventChain->SetBranchAddress("sumph[2]", sumph, &b_sumph);
   eventChain->SetBranchAddress("sumtTrig[2]", sumtTrig, &b_sumtTrig);
   eventChain->SetBranchAddress("sumtPeak[2]", sumtPeak, &b_sumtPeak);
   eventChain->SetBranchAddress("ccoinc", &ccoinc, &b_ccoinc);
   eventChain->SetBranchAddress("a0wf[1280]", a0wf, &b_a0wf);
   eventChain->SetBranchAddress("a1wf[1280]", a1wf, &b_a1wf);
   eventChain->SetBranchAddress("g0wf[1280]", g0wf, &b_g0wf);
   eventChain->SetBranchAddress("g1wf[1280]", g1wf, &b_g1wf);
   eventChain->SetBranchAddress("sum0wf[1280]", sum0wf, &b_sum0wf);
   eventChain->SetBranchAddress("sum1wf[1280]", sum1wf, &b_sum1wf);
   eventChain->SetBranchAddress("cwf[1280]", cwf, &b_cwf);
   Notify();
}

Bool_t fragFiss::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void fragFiss::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!eventChain) return;
   eventChain->Show(entry);
}
Int_t fragFiss::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
