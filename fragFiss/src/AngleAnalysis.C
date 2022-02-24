#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void fragFiss::AngleAnalysis()
{

   if (eventChain == 0) return;
   // Put this somewhere else
   TH2D* h2_anodeGrid1 = new TH2D("h2_anodeGrid1","h2_anodeGrid1",N_BINS_RATIO,0,1,N_BINS_APH,0,MAX_APH);
   TH2D* h2_anodeGrid2 = new TH2D("h2_anodeGrid2","h2_anodeGrid2",N_BINS_RATIO,0,1,N_BINS_APH,0,MAX_APH);


  /*
         _             _       ___ _ _ _   _
        /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
       / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
      /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
              |___/                                  |_|
  */

   nentries = eventChain->GetEntries();
   cout << "Analyzing " << nentries << " events" << endl;

   Long64_t nbytes = 0, nb = 0;
   cout << "Filling angle histograms" << endl;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = eventChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      h2_anodeGrid1->Fill(gph[0]/aph[0], aph[0]);
      h2_anodeGrid2->Fill(gph[1]/aph[1], aph[1]);
   }

  /*
     _             _       ___ _ _ _   _
    /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
   / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
  /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
             |___/                                  |_|
  */
   // Determine angle
   double *chargeLength1, *chargeLength2, *phAx;
   chargeLength1 = new double[N_BINS_APH];
   chargeLength2 = new double[N_BINS_APH];
   phAx = new double[N_BINS_APH];


   // analysis for selection
   double *APH1, *APH2, *cosAx;
   APH1 = new double[N_BINS_RATIO];
   APH2 = new double[N_BINS_RATIO];
   cosAx = new double[N_BINS_RATIO];

   double *dAPH1, *dAPH2, *dcosAx;
   dAPH1 = new double[N_BINS_RATIO - 1];
   dAPH2 = new double[N_BINS_RATIO - 1];
   dcosAx = new double[N_BINS_RATIO - 1];


   for (int b=0; b < N_BINS_APH; b++)
   {
     phAx[b] = h2_anodeGrid1->GetYaxis()->GetBinCenter(b+1);
     TH1D* projAPH1 = h2_anodeGrid1->ProjectionX("projAPH1" + TString(b),b+1,b+2);
     TH1D* projAPH2 = h2_anodeGrid2->ProjectionX("projAPH2" + TString(b),b+1,b+2);

     // line conversion

     int maxBinAPH1 = -1;
     int maxBinAPH2 = -1;
     double maxAPH1 = -1;
     double maxAPH2 = -1;

     for (int i = 0; i < N_BINS_RATIO; i++)
     {
       cosAx[i] = projAPH1->GetBinCenter(i+1);
       APH1[i]  = projAPH1->GetBinContent(i+1);
       APH2[i]  = projAPH2->GetBinContent(i+2);

       // find maximum and bin
       if(APH1[i] > maxAPH1)
       {
         maxAPH1 = APH1[i];
         maxBinAPH1 = i;
       }

       if(APH2[i] > maxAPH2)
       {
         maxAPH2 = APH2[i];
         maxBinAPH2 = i;
       }

     }

     // derivative filter

     int dMinBinAPH1 = -1;
     int dMinBinAPH2 = -1;
     double dMinAPH1 = -1;
     double dMinAPH2 = -1;

     for (int i = 0; i < N_BINS_RATIO-1; i++)
     {
       dcosAx[i] = (cosAx[i+1]- cosAx[i]) / 2.0;
       dAPH1[i] = (APH1[i+1] - APH1[i]) / dcosAx[i];
       dAPH2[i] = (APH2[i+1] - APH2[i]) / dcosAx[i];

       // find maximum and bin
       if( (dAPH1[i] < dMinAPH1) && (i >= maxBinAPH1) )
       {
         dMinAPH1 = dAPH1[i];
         dMinBinAPH1 = i;
       }

       if( (dAPH2[i] < dMinAPH2) && (i >= maxBinAPH2) )
       {
         dMinAPH2 = dAPH2[i];
         dMinBinAPH2 = i;
       }
     }

     chargeLength1[b] = projAPH1->GetBinCenter(dMinBinAPH1+1);
     chargeLength2[b] = projAPH2->GetBinCenter(dMinBinAPH2+1);

     // cout << phAx[b] << " " << chargeLength1[b] << " "<< chargeLength2[b] << endl;

     if (projAPH1->GetEntries() < MIN_ENTRIES_PROJ)
     {
       chargeLength1[b] = 0;
     }
     if (projAPH2->GetEntries() < MIN_ENTRIES_PROJ)
     {
       chargeLength2[b] = 0;
     }

   }

   // store final results
   g_Ang1 = new TGraph(N_BINS_APH, phAx, chargeLength1);
   g_Ang2 = new TGraph(N_BINS_APH, phAx, chargeLength2);

    /*
     ___ _     _
    | _ \ |___| |_
    |  _/ / _ \  _|
    |_| |_\___/\__|

    */

   TGraph* g_xAng1 = new TGraph(N_BINS_APH, chargeLength1, phAx);
   g_xAng1->SetName("g_xAng1");
   TGraph* g_xAng2 = new TGraph(N_BINS_APH, chargeLength2, phAx);
   g_xAng1->SetName("g_xAng2");

   // canvas with angle results
   TCanvas* c_angleAn = new TCanvas("c_angle1", "c_angle1", 400, 500);
   c_angleAn->Divide(1,2);

   c_angleAn->cd(1);
   h2_anodeGrid1->Draw();
   h2_anodeGrid1->SetOption("COLZ");
   g_xAng1->Draw("SAME");
   g_xAng1->SetLineColor(kRed);
   g_xAng1->SetLineWidth(2);

   c_angleAn->cd(2);
   h2_anodeGrid2->Draw();
   h2_anodeGrid2->SetOption("COLZ");
   g_xAng2->Draw("SAME");
   g_xAng2->SetLineColor(kRed);
   g_xAng2->SetLineWidth(2);

   fragFile->cd();
   c_angleAn->Write();

}
