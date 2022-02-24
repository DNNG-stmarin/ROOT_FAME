#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void fragFiss::GainMatching()
{

   if (eventChain == 0) return;
   // Put this somewhere else
   TH1D* h1_uncalibratedA1 = new TH1D("h1_uncalibratedA1","h1_uncalibratedA1", N_BINS_APH, 0, MAX_APH);
   TH1D* h1_uncalibratedA2 = new TH1D("h1_uncalibratedA2","h1_uncalibratedA2", N_BINS_APH, 0, MAX_APH);


  /*
         _             _       ___ _ _ _   _
        /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
       / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
      /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
              |___/                                  |_|
  */

   Long64_t nbytes = 0, nb = 0;
   double cos1, cos2;
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
      ua1 += f_att1->Eval(1.0/cos1) - f_att2->Eval(0);
      ua2 += f_att2->Eval(1.0/cos2) - f_att2->Eval(0);


      if( (ua1 > MIN_ANODE1) && (ua2 > MIN_ANODE2) && (cos1 > MIN_ANG1) && (cos2 > MIN_ANG2))
      {
        h1_uncalibratedA1->Fill(ua1);
        h1_uncalibratedA2->Fill(ua2);
      }

   }

   TF1* gaussFit = new TF1("gaussFit", "[0] * e^(-((x - [1])^2)/(2*[2]^2))", 0, MAX_APH);
   // yield 1
   TF1* f_gaussYield1 = new TF1("f_gaussYield1", "gaussFit + gaussFit", 0, MAX_APH);
   TF1* f_gaussYield2 = new TF1("f_gaussYield2", "gaussFit + gaussFit", 0, MAX_APH);

   double ampGuess, meanGuess, sigGuess;
   ampGuess = h1_uncalibratedA1->GetMaximum();
   meanGuess = h1_uncalibratedA1->GetMean();
   sigGuess = 100;
   // cout << ampGuess << " " << meanGuess << sigGuess << " " << endl;
   f_gaussYield1->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
   // f_gaussYield1->SetParLimits(1, 0, ampGuess);
   // f_gaussYield1->SetParLimits(4, ampGuess, MAX_APH);
    h1_uncalibratedA1->Fit(f_gaussYield1, "N Q", "", 0, MAX_APH);

   ampGuess = h1_uncalibratedA2->GetMaximum();
   meanGuess = h1_uncalibratedA2->GetMean();
   sigGuess = 100;
   // cout << ampGuess << " " << meanGuess << " " << sigGuess << endl;
   f_gaussYield2->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
   // f_gaussYield2->SetParLimits(1, 0, ampGuess);
   // f_gaussYield2->SetParLimits(4, ampGuess, MAX_APH);


   h1_uncalibratedA2->Fit(f_gaussYield2, "N Q", "", 0, MAX_APH);

   double centroids1[2];
   double centroids2[2];

   centroids1[0] = f_gaussYield1->GetParameter(1);
   centroids1[1] = f_gaussYield1->GetParameter(4);

   centroids2[0] = f_gaussYield2->GetParameter(1);
   centroids2[1] = f_gaussYield2->GetParameter(4);

   cout << centroids1[0] << " " << centroids1[1] << endl;
   cout << centroids2[0] << " " << centroids2[1] << endl;

   g_gainMatch = new TGraph(2, centroids2, centroids1);
   g_gainMatch->SetName("g_gainMatch");


   //
   // // canvas with gain matched results
   TCanvas* c_gain = new TCanvas("c_gain", "c_gain", 400, 500);
   c_gain->Divide(1,3);

   // draw
   c_gain->cd(1);
   h1_uncalibratedA1->SetLineColor(kBlue);
   h1_uncalibratedA2->SetLineColor(kRed);
   h1_uncalibratedA1->Draw();
   h1_uncalibratedA2->Draw("SAME");
   f_gaussYield1->SetLineColor(kBlue);
   f_gaussYield2->SetLineColor(kRed);
   f_gaussYield1->Draw("SAME");
   f_gaussYield2->Draw("SAME");

   c_gain->cd(2);
   g_gainMatch->Draw();

   // write file
   fragFile->cd();
   c_gain->Write();


 }
