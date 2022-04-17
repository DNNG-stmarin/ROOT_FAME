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
   TH2D* h2_anodeGrid1 = new TH2D("h2_anodeGrid1","h2_anodeGrid1;gph/aph;aph",N_BINS_RATIO,0,MAX_RATIO,N_BINS_APH,0,MAX_APH);
   TH2D* h2_anodeGrid2 = new TH2D("h2_anodeGrid2","h2_anodeGrid2;gph/aph;aph",N_BINS_RATIO,0,MAX_RATIO,N_BINS_APH,0,MAX_APH);

   const int MIN_BIN_RATIO = (int)(MIN_RATIO/MAX_RATIO*N_BINS_RATIO);
   const int MAX_BIN_RATIO = (int)N_BINS_RATIO;

   const int MIN_BR_1_BIN = (int)(MIN_BR_1/MAX_APH*N_BINS_APH);
   const int MIN_BR_2_BIN = (int)(MIN_BR_2/MAX_APH*N_BINS_APH);
   const int MAX_BR_1_BIN = (int)(MAX_BR_1/MAX_APH*N_BINS_APH);
   const int MAX_BR_2_BIN = (int)(MAX_BR_2/MAX_APH*N_BINS_APH);

   // cout << MIN_BIN_RATIO << " " << MAX_BIN_RATIO << " " << MIN_BR_1_BIN << " " << MAX_BR_1_BIN << endl;

  /*
         _             _       ___ _ _ _   _
        /_\  _ _  __ _| |___  | __(_) | | | |   ___  ___ _ __
       / _ \| ' \/ _` | / -_) | _|| | | | | |__/ _ \/ _ \ '_ \
      /_/ \_\_||_\__, |_\___| |_| |_|_|_| |____\___/\___/ .__/
              |___/                                  |_|
  */

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
     TH1D* projAPH1 = h2_anodeGrid1->ProjectionX("projAPH1" + TString(b),b+1,b+1);
     TH1D* projAPH2 = h2_anodeGrid2->ProjectionX("projAPH2" + TString(b),b+1,b+1);

     // line conversion

     int maxBinAPH1 = -1;
     int maxBinAPH2 = -1;
     double maxAPH1 = -1;
     double maxAPH2 = -1;

     for (int i = MIN_BIN_RATIO; i < N_BINS_RATIO; i++)
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

     double dPrev = 0;
     double dPrev1 = 0;
     double dPrev2 = 0;
     double dNext = 0;
     double dNext1 = 0;
     double dNext2 = 0;

     bool dFound1 = false;
     bool dFound2 = false;

     for (int i = MIN_BIN_RATIO; i < N_BINS_RATIO - 2; i++)
     {
       // calculate local derivative
       dcosAx[i] = (cosAx[i+1]- cosAx[i]) / 2.0;
       dAPH1[i] = (APH1[i+1] - APH1[i]) / dcosAx[i];
       dAPH2[i] = (APH2[i+1] - APH2[i]) / dcosAx[i];

       // calculate backward and forward derivatives
       dPrev    = (cosAx[i]- cosAx[i-1]) / 2.0;
       dPrev1 = (APH1[i] - APH1[i-1]) / dPrev;
       dPrev2 = (APH2[i] - APH2[i-1]) / dPrev;

       dNext    = (cosAx[i+2]- cosAx[i+1]) / 2.0;
       dNext1 = (APH1[i+2] - APH1[i+1]) / dNext;
       dNext2 = (APH2[i+2] - APH2[i+1]) / dNext;

       if ( (dPrev1 < 0) && (dNext1 < 0) )
       {
         // find maximum and bin
         if( (dAPH1[i] < dMinAPH1) && (i >= maxBinAPH1) )
         {
           dMinAPH1 = dAPH1[i];
           dMinBinAPH1 = i;
           dFound1 = true;
         }
       }

       if ( (dPrev2 < 0) && (dNext2 < 0) )
       {
         if( (dAPH2[i] < dMinAPH2) && (i >= maxBinAPH2) )
         {
           dMinAPH2 = dAPH2[i];
           dMinBinAPH2 = i;
           dFound2 = true;
         }
       }

     }
     // cout << phAx[b] << " " << dMinAPH1 << " " << dMinAPH2 << endl;
     //
     if (dFound1) {chargeLength1[b] = projAPH1->GetBinCenter(dMinBinAPH1+1);}
     else {chargeLength1[b] = chargeLength1[b-1];}

     if (dFound2) {chargeLength2[b] = projAPH2->GetBinCenter(dMinBinAPH2+1);}
     else {chargeLength2[b] = chargeLength2[b-1];}

     // chargeLength1[b] = projAPH1->GetBinCenter(dMinBinAPH1+1);
     // chargeLength2[b] = projAPH2->GetBinCenter(dMinBinAPH2+1);

     // cout << phAx[b] << " " << chargeLength1[b] << " "<< chargeLength2[b] << endl;

     if (projAPH1->GetEntries() < MIN_ENTRIES_PROJ)
     {
       chargeLength1[b] = 0;
     }
     if (projAPH2->GetEntries() < MIN_ENTRIES_PROJ)
     {
       chargeLength2[b] = 0;
     }

     // cout << b << ": " << chargeLength1[b] << " " << chargeLength2[b] << endl;

   }

  /*
   ___              _             _     _
  | _ )_ _ ___ __ _| |___ __  ___(_)_ _| |_ ___
  | _ \ '_/ -_) _` | / / '_ \/ _ \ | ' \  _(_-<
  |___/_| \___\__,_|_\_\ .__/\___/_|_||_\__/__/
                      |_|
  */

    int brMinA1, brMinA2;
    int brMaxA1, brMaxA2;

    double minBreak1 = 1;
    double maxBreak1 = -1;
    // find the breakpoints
    for(int b = MIN_BR_1_BIN+1; b < MAX_BR_1_BIN; b++)
    {
      if(chargeLength1[b+1] - chargeLength1[b] >= maxBreak1)
      {
       maxBreak1 = chargeLength1[b+1] - chargeLength1[b];
       brMaxA1 = b;
      }
      if(chargeLength1[b] - chargeLength1[b-1] <= minBreak1)
      {
       minBreak1 = chargeLength1[b] - chargeLength1[b-1] ;
       brMinA1 = b;
      }
    }

   double minBreak2 = 1;
   double maxBreak2 = -1;
   // find the breakpoints
   for(int b = MIN_BR_2_BIN+1; b < MAX_BR_2_BIN; b++)
   {
     if(chargeLength2[b+1] - chargeLength2[b] >= maxBreak2)
     {
      maxBreak2 = chargeLength2[b+1] - chargeLength2[b];
      brMaxA2 = b;
     }
     if(chargeLength2[b] - chargeLength2[b-1] <= minBreak2)
     {
      minBreak2 = chargeLength2[b] - chargeLength2[b-1] ;
      brMinA2 = b;
     }
   }

   cout << "Breakpoints for anode 1: " << phAx[brMinA1] << " " << phAx[brMaxA1] << endl;
   cout << "Breakpoints for anode 2: " << phAx[brMinA2] << " " << phAx[brMaxA2] << endl;


   // // get rid of points past the breakpoints
   for (int b=0; b < N_BINS_APH; b++)
   {
     if(b <= brMaxA1) chargeLength1[b] = chargeLength1[brMaxA1 + 1];
     else if(b >= brMinA1) chargeLength1[b] = chargeLength1[brMinA1 - 1];

     if(b <= brMaxA2) chargeLength2[b] = chargeLength2[brMaxA2 + 1];
     else if(b >= brMinA2) chargeLength2[b] = chargeLength2[brMinA2 - 1];
   }

   // store final results
   g_Ang1 = new TGraph(N_BINS_APH, phAx, chargeLength1);
   g_Ang2 = new TGraph(N_BINS_APH, phAx, chargeLength2);


   // fit functions
   f_ang1 = new TF1("f_ang1", "[0] + [2]*x^2", 0, MAX_APH);
   f_ang2 = new TF1("f_ang2", "[0] + [2]*x^2", 0, MAX_APH);

   // f_ang1->SetParameters(chargeLength1[brMinA1 + 1], (chargeLength1[brMaxA1 - 1] - chargeLength1[brMinA1 + 1])


    //    ___                            _     ___
    // | __| _ __ _ __ _ _ __  ___ _ _| |_  / __| ___ _ __
    // | _| '_/ _` / _` | '  \/ -_) ' \  _| \__ \/ -_) '_ \
    // |_||_| \__,_\__, |_|_|_\___|_||_\__| |___/\___| .__/
    //            |___/                             |_|

   bool fragSepAng = false;

   TGraph *g_centAng1H, *g_centAng1L, *g_centAng2H, *g_centAng2L;

   if(fragSepAng)
   {
     cout << "beginning fragment separation analysis" << endl;
     // set up the fit functions
     TF1* gaussFit = new TF1("gaussFit", "[0] * e^(-((x - [1])^2)/(2*[2]^2))", 0, MAX_APH);
     // yield 1
     TF1* f_gaussYield1 = new TF1("f_gaussYield1", "gaussFit + gaussFit", 0, MAX_APH);
     TF1* f_gaussYield2 = new TF1("f_gaussYield2", "gaussFit + gaussFit", 0, MAX_APH);


     // create arrays to store data
     double** centroids1;
     double** centroids2;

     double* angAxC;
     double* invAngC;

     centroids1 = new double*[2];
     centroids2 = new double*[2];

     angAxC = new double [N_BINS_RATIO];
     invAngC = new double [N_BINS_RATIO];

     for(int i = 0; i < 2; i++)
     {
       centroids1[i] = new double [N_BINS_RATIO];
       centroids2[i] = new double [N_BINS_RATIO];
     }

     int indC = 0;
     // now loop through the allowable values of angles
     for(int c = 0; c < N_BINS_RATIO; c++)
     {

       double angle = h2_anodeGrid1->GetXaxis()->GetBinCenter(c+1); // store the results
       angAxC[indC] = angle;
       invAngC[indC] = 1.0/angle;

       TH1D* h_sliceTKEatt1 = h2_anodeGrid1->ProjectionY("h_sliceTKEatt1_" + TString(c), c, c+2);
       TH1D* h_sliceTKEatt2 = h2_anodeGrid2->ProjectionY("h_sliceTKEatt2_" + TString(c), c, c+2);

       double ampGuess, meanGuess, sigGuess;
       ampGuess = h_sliceTKEatt1->GetMaximum();
       meanGuess = h_sliceTKEatt1->GetMean();
       sigGuess = SIG_ANODE;
       // cout << ampGuess << " " << meanGuess << sigGuess << " " << endl;
       f_gaussYield1->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
       f_gaussYield1->SetParLimits(1, MIN_ANODE2, meanGuess);
       f_gaussYield1->SetParLimits(4, meanGuess, MAX_APH);
       h2_anodeGrid1->Fit(f_gaussYield1, "N Q", "", 0, MAX_APH);

       ampGuess = h_sliceTKEatt2->GetMaximum();
       meanGuess = h_sliceTKEatt2->GetMean();
       sigGuess = SIG_ANODE;
       // cout << ampGuess << " " << meanGuess << " " << sigGuess << endl;
       f_gaussYield2->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
       f_gaussYield2->SetParLimits(1, MIN_ANODE2, meanGuess);
       f_gaussYield2->SetParLimits(4, meanGuess, MAX_APH);
       h2_anodeGrid2->Fit(f_gaussYield2, "N Q", "", 0, MAX_APH);


       centroids1[0][indC] = f_gaussYield1->GetParameter(1);
       centroids1[1][indC] = f_gaussYield1->GetParameter(4);

       centroids2[0][indC] = f_gaussYield2->GetParameter(1);
       centroids2[1][indC] = f_gaussYield2->GetParameter(4);

       cout << centroids1[0][indC] << ", " << centroids1[1][indC] << " | " << centroids2[0][indC] << ", " << centroids2[1][indC] << endl;

       indC++;

     }
     // create the graphs
     g_centAng1H = new TGraph(N_BINS_RATIO, angAxC, centroids1[0]);
     g_centAng1L = new TGraph(N_BINS_RATIO, angAxC, centroids1[1]);

     g_centAng2H = new TGraph(N_BINS_RATIO, angAxC, centroids2[0]);
     g_centAng2L = new TGraph(N_BINS_RATIO, angAxC, centroids2[1]);

     g_centAng1H->SetName("g_cent1H");
     g_centAng1L->SetName("g_cent1L");
     g_centAng2H->SetName("g_cent2H");
     g_centAng2L->SetName("g_cent2L");


     TGraph* g_invAng1H = new TGraph(N_BINS_RATIO, invAngC, centroids1[0]);
     TGraph* g_invAng1L = new TGraph(N_BINS_RATIO, invAngC, centroids1[1]);

     TGraph* g_invAng2H = new TGraph(N_BINS_RATIO, invAngC, centroids2[0]);
     TGraph* g_invAng2L = new TGraph(N_BINS_RATIO, invAngC, centroids2[1]);

     g_invAng1H->SetName("g_inv1H");
     g_invAng1L->SetName("g_inv1L");
     g_invAng2H->SetName("g_inv2H");
     g_invAng2L->SetName("g_inv2L");
   }



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
   TCanvas* c_angleAn = new TCanvas("c_angle1", "c_angle1", 600, 800);
   c_angleAn->Divide(1,2);

   c_angleAn->cd(1);
   h2_anodeGrid1->Draw();
   h2_anodeGrid1->SetOption("COLZ");
   g_xAng1->Draw("SAME");
   g_xAng1->SetLineColor(kRed);
   g_xAng1->SetLineWidth(2);

   if(fragSepAng)
   {
     // fragment lines
     g_centAng1H->SetLineColor(kGreen);
     g_centAng1L->SetLineColor(kGreen);
     g_centAng1H->SetLineWidth(2);
     g_centAng1L->SetLineWidth(2);
     g_centAng1H->Draw("SAME");
     g_centAng1L->Draw("SAME");
   }


   c_angleAn->cd(2);
   h2_anodeGrid2->Draw();
   h2_anodeGrid2->SetOption("COLZ");
   g_xAng2->Draw("SAME");
   g_xAng2->SetLineColor(kRed);
   g_xAng2->SetLineWidth(2);

   if(fragSepAng)
   {
     // fragment lines
     g_centAng2H->SetLineColor(kGreen);
     g_centAng2L->SetLineColor(kGreen);
     g_centAng2H->SetLineWidth(2);
     g_centAng2L->SetLineWidth(2);
     g_centAng2H->Draw("SAME");
     g_centAng2L->Draw("SAME");
   }

   fragFile->cd();
   c_angleAn->Write();

}
