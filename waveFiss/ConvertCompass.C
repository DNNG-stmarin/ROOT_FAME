#define ConvertCompass_cxx
#include "ConvertCompass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ConvertCompass::Loop()
{

   // setup info
   const int RECORD_LENGTH = 1280;
   const int NUM_ELEC = 7;
   const int channelChamber[NUM_ELEC] = {1,2,3,4,5,21};

   // create tree
   TFile* danaFile = new TFile("danaFile.root", "RECREATE");
   danaFile->cd();
   TTree* danaTree = new TTree("t", "DanaCompatible TTree");
   danaTree->SetFileNumber(0);

   UChar_t tBoard;
   UChar_t tChan;
   UInt_t tTime;
   Short_t tWave[RECORD_LENGTH];

   danaTree->Branch("bnum", &tBoard, "bnum/b");
   danaTree->Branch("chnum", &tChan, "chnum/b");
   danaTree->Branch("ts", &tTime, "ts/i");
   danaTree->Branch("wf[1280]", tWave, "wf[1280]/S");

   danaTree->SetMaxTreeSize(1000000000LL);


   // random filling options
   // TF1* electrodeFall = new TF1("electrode", "1/(1 + Exp[-x])", )
   TRandom3* randGen = new TRandom3();

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   nentries = 100000;

   int r = 0;
   double h1, h2;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


      // mock data generator
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      tBoard = 0;
      tChan = r%NUM_ELEC;
      tTime = 2000*r/NUM_ELEC + randGen->Integer(400);

      h1 = randGen->Gaus(5000,1500);
      // if(tChan == 1)
      // {
      //    r++; 
      //    continue;
      // }
      if(tChan == 4 || tChan == 5) // grids
      {
         h2 = h1 + randGen->Gaus(2000,400);
      }
      else
      {
         h2 = 0;
      }


      for(int x = 0; x < RECORD_LENGTH; x++)
      {
         tWave[x] = h1/(1.0+TMath::Exp((x-RECORD_LENGTH/2)/50)) + h2/(1.0+TMath::Exp(-(x-RECORD_LENGTH/2 - 400)/50));
      }
      r++;
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



      danaTree->Fill();
      
   }

   danaTree->Write();

   danaFile->Write();
   danaFile->Close();
}
