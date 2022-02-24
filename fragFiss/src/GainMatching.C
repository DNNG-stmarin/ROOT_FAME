#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void GainMatching::ELossCorrection()
{

   if (eventChain == 0) return;
   // Put this somewhere else
   TH2D* h1_uncalibratedA1 = new TH1D("h1_uncalibratedA1","h1_uncalibratedA1", N_BINS_APH, 0, MAX_APH);
   TH2D* h2_uncalibratedA1 = new TH1D("h1_uncalibratedA1","h1_uncalibratedA1", N_BINS_APH, 0, MAX_APH);


  /*
         _             _       ___ _ _ _   _
        /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
       / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
      /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
              |___/                                  |_|
  */

   Long64_t nbytes = 0, nb = 0;
   double ua1, ua2;
   cout << "Filling gain matching histograms" << endl;

   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = eventChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // calculate the angles
      if(g_Ang1->Eval(aph[0]) > 0)
      {
        cos1  = (gph[0]/aph[0])/g_Ang1->Eval(aph[0]);
      }
      else cos1  = -1;

      if(g_Ang2->Eval(aph[1]) > 0)
      {
        cos2  = (gph[1]/aph[1])/g_Ang1->Eval(aph[1]);
      }
      else cos2  = -1;


      // kinetic energy
      ua1 = aph[0];
      ua2 = aph[1];
      ua1 += f_att1->Eval(1.0/fThetaL) - f_att2->Eval(0);
      ua2 += f_att2->Eval(1.0/fThetaL) - f_att2->Eval(0);


      if( (ua1 > MIN_ANODE1) && (ua2 > MIN_ANODE2) && (cos1 > MIN_ANG1) && (cos2 > MIN_ANG2))
      {
        h1_uncalibratedA1->Fill(ua1);
        h1_uncalibratedA2->Fill(ua2);
      }
   }




   // canvas with angle results
   TCanvas* c_backing = new TCanvas("c_backing", "c_backing", 400, 500);

   c_backing->cd(1);
   p1_backing1->Draw();
   p1_backing1->SetName("p1_backing1");
   p1_backing1->SetLineColor(kBlue);
   f_att1->Draw("SAME");

   p1_backing2->Draw("SAME");
   p1_backing2->SetName("p1_backing2");
   p1_backing2->SetLineColor(kRed);
   f_att2->Draw("SAME");

   fragFile->cd();
   c_backing->Write();


 }
