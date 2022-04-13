#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void fragFiss::ELossCorrection()
{

   if (eventChain == 0) return;
   // Put this somewhere else
   TH2D* h2_backingRem1 = new TH2D("h2_backingRem1","h2_backingRem1", N_BINS_RATIO,-1, 5, N_BINS_APH, 0, MAX_APH);
   TH2D* h2_backingRem2 = new TH2D("h2_backingRem2","h2_backingRem2", N_BINS_RATIO,-1, 5, N_BINS_APH, 0, MAX_APH);


  /*
         _             _       ___ _ _ _   _
        /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
       / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
      /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
              |___/                                  |_|
  */

   Long64_t nbytes = 0, nb = 0;
   double cos1, cos2;
   cout << "Filling backing attenuation histograms" << endl;
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

      if(aph[0] > MIN_ANODE1) h2_backingRem1->Fill(1.0/cos1, aph[0]);
      if(aph[1] > MIN_ANODE2) h2_backingRem2->Fill(1.0/cos2, aph[1]);
   }


   TProfile* p1_backing1 = h2_backingRem1->ProfileX();
   TProfile* p1_backing2 = h2_backingRem2->ProfileX();


   // fits
   f_att1 = new TF1("f_att1", "[0] + [1]*x", -10, 10);
   f_att2 = new TF1("f_att1", "[0] + [1]*x", -10, 10);

   p1_backing1->Fit(f_att1, "Q", "", MIN_INV_ANG_FIT, MAX_INV_ANG_FIT);
   p1_backing2->Fit(f_att2, "Q", "", MIN_INV_ANG_FIT, MAX_INV_ANG_FIT);

   // analysis of fits
   // ask fredrik about this
   double crossPoint = (f_att2->GetParameter(0) - f_att1->GetParameter(0))/(f_att1->GetParameter(1) - f_att2->GetParameter(1));
   double heightPoint = f_att1->Eval(crossPoint);

   cout << "The intersect point is " << crossPoint << " and the height is " << heightPoint << endl;

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
