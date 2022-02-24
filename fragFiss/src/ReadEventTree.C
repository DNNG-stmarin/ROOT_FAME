#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void fragFiss::ReadEventTree()
{

   if (eventChain == 0) return;
   // Put this somewhere else
   TH2D* h2_anodeGrid1 = new TH2D("h2_anodeGrid1","h2_anodeGrid1",N_BINS_RATIO,0,1,N_BINS_APH,0,MAX_APH);
   TH2D* h2_anodeGrid2 = new TH2D("h2_anodeGrid2","h2_anodeGrid2",N_BINS_RATIO,0,1,N_BINS_APH,0,MAX_APH);

   Long64_t nentries = eventChain->GetEntries();
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

   // Determine angle
   double *chargeLength1, *chargeLength2, *phAx;
   chargeLength1 = new double[N_BINS_APH];
   chargeLength2 = new double[N_BINS_APH];
   phAx = new double[N_BINS_APH];

   double *dAPH1, *dAPH2, *ratioAx;
   dAPH1 = new double[N_BINS_RATIO - 1];
   dAPH2 = new double[N_BINS_RATIO - 1];
   ratioAx = new double[N_BINS_RATIO - 1];


   for (int b=0; b < N_BINS_APH; b++)
   {
     phAx[b] = h2_anodeGrid1->GetYaxis()->GetBinCenter(b+1);
     TH1D* projAPH1 = h2_anodeGrid1->ProjectionX("projAPH1" + TString(b),b+1,b+2);

     TH1D* projAPH2 = h2_anodeGrid2->ProjectionX("projAPH2" + TString(b),b+1,b+2);


     if (projAPH1->GetEntries() < MIN_ENTRIES_PROJ)
     {
       chargeLength1[b] = 0;
       continue;
     }

     int minBin1 = -1;
     int minBin2 = -1;

     // for (int i = 0; i < N_BINS_RATIO - 1; i++)
     // {
     //   ratioAx[i] = (projAPH1->GetBinCenter(i+2) - projAPH1->GetBinCenter(i+1)) / 2.0;
     //   dAPH1[i] = (projAPH1->GetBinContent(i+2) - projAPH1->GetBinContent(i+1)) / ratioAx[i];
     //   dAPH2[i] = (projAPH2->GetBinContent(i+2) - projAPH2->GetBinContent(i+1)) / ratioAx[i];
     // }

     TGraph* g_APH1 = new TGraph(projAPH1);
     TGraph* g_APH2 = new TGraph(projAPH2);

     double gToF1(Double_t *x, Double_t *) { return g_APH1->Eval(x[o]);}
     double gToF2(Double_t *x, Double_t *) { return g_APH2->Eval(x[o]);}

     TF1* f_APH1 = new TF1("f_APH1", gToF1, 0, 1, 0);
     TF1* f_APH2 = new TF1("f_APH2", gToF2, 0, 1, 0);

     TF1* d_APH1 = f_APH1->Derivative();
     TF1* d_APH2 = f_APH2->Derivative();






     // chargeLength1[b] = ;
   }


   // Put this somewhere else
   fragFile->cd();
   h2_anodeGrid1->Write();
   h2_anodeGrid2->Write();
}
