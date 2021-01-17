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

#include <queue>

#include "CoincidenceEvent.h"
#include "ParticleEvent.h"
#include "BeamEvent.h"
#include "TriggerEvent.h"

#include "InfoSystem.h"
#include "ProcessingConstants.h"

using namespace std;

class CoincidenceAnalysis {
public :
   TChain         *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   //create TTree object to contain the output coincidence tree
   TTree* coincTree;;

   // output file
   TFile* expFile = 0;
   // TDirectory* fileTreeDir = 0;

   // declare the digitizer classes
   int DATA_TYPE;
   // 0: CoMPASS
   // 1: MIDAS

   COMPASS_DIG* cp = new COMPASS_DIG();
   MIDAS_DIG* md = new MIDAS_DIG();

   //infosystem attributes (not info)
   int NUM_TRIGGERS = 0;
   int NUM_DETS = 0;
   int NUM_BEAMS = 0;

   double TRIGGER_THRESHOLD = 0;
   double TRIGGER_CLIP = 0;
   double TRIGGER_MIN_PSP = 0.0;
   double TRIGGER_MAX_PSP = 1.0;

   bool TRIGGER_SPLIT = 0;
   double MAX_TRIGGER_DRIFT = 0;
   double COINC_WINDOW = 0;
   double BEAM_WINDOW = 0;

   int* FISSION_TRIGGERS;
   int* DETECTORS;
   int* BEAM;

   double MICRO_SEP;
   double MACRO_SEP;
   int MICRO_NUM;

   //  _                          ____  _     _           _
   // | |                        / __ \| |   (_)         | |
   // | |     ___   ___  _ __   | |  | | |__  _  ___  ___| |_ ___
   // | |    / _ \ / _ \| '_ \  | |  | | '_ \| |/ _ \/ __| __/ __|
   // | |___| (_) | (_) | |_) | | |__| | |_) | |  __/ (__| |_\__ \
   // |______\___/ \___/| .__/   \____/|_.__/| |\___|\___|\__|___/
   //                   | |                 _/ |
   //                   |_|                |__/

  // use an array of fifo to store particles and chambers
  queue<TriggerEvent>* TriggerBuffer;
  queue<ParticleEvent>* DetectorBuffer;
  queue<BeamEvent>* BeamBuffer;

  // queue containing valid fission events

  queue<CoincidenceEvent> ValidTriggerBuffer;
  queue<BeamEvent> ValidBeamBuffer;
	queue<CoincidenceEvent> FissionBuffer;


   CoincidenceAnalysis(TFile* expFileWrite, TChain* tree, InfoSystem* info = 0);
   virtual ~CoincidenceAnalysis();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TChain *tree);
   virtual int   CreateCoincidenceTree(Long64_t entriesToProc = -1);
};

#endif

#ifdef CoincidenceAnalysis_cxx

CoincidenceAnalysis::CoincidenceAnalysis(TFile* expFileWrite, TChain* tree, InfoSystem* info)
{
    DATA_TYPE = info->DATA_TYPE;
    TRIGGER_THRESHOLD = info->TRIGGER_THRESHOLD;
    TRIGGER_CLIP = info->TRIGGER_CLIP;
    TRIGGER_SPLIT = info->TRIGGER_SPLIT;
    TRIGGER_MIN_PSP = info->TRIGGER_MIN_PSP;
    TRIGGER_MAX_PSP = info->TRIGGER_MAX_PSP;

    MAX_TRIGGER_DRIFT = info->MAX_TRIGGER_DRIFT;
    COINC_WINDOW = info->COINC_WINDOW;
    BEAM_WINDOW = info->BEAM_WINDOW;

    NUM_TRIGGERS = info->NUM_TRIGGERS;
    NUM_DETS = info->NUM_DETS;
    NUM_BEAMS = info->NUM_BEAMS;
    FISSION_TRIGGERS = info->FISSION_TRIGGERS;
    DETECTORS = info->DETECTORS;
    BEAM = info->BEAM;

    MICRO_SEP = info->MICRO_SEP;
    MACRO_SEP = info->MACRO_SEP;
    MICRO_NUM = info->MICRO_NUM;

   // set the output stream
   expFile = expFileWrite;

   Init(tree);

   // use an array of fifo to store particles and chambers
   TriggerBuffer = new queue<TriggerEvent> [NUM_TRIGGERS];
   DetectorBuffer = new queue<ParticleEvent> [NUM_DETS];
   BeamBuffer = new queue<BeamEvent> [NUM_BEAMS];

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

void CoincidenceAnalysis::Init(TChain *tree)
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
