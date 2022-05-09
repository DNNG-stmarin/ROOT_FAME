#include "fragFiss.h"
#include "constants.h"

#include <TH2.h>
#include <TF1.h>
#include <TH3.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>

void fragFiss::PostCalib(int iterationPost)
{
  cout << "Recalibrating anode histograms" << endl;

  TH1D* h1_massAn1 = new TH1D("h1_massAn1_"+ TString(to_string(iterationPost)),"h1_massAn1", N_BINS_APH, 0, MAX_APH);
  TH1D* h1_massAn2 = new TH1D("h1_massAn2_"+ TString(to_string(iterationPost)),"h1_massAn2", N_BINS_APH, 0, MAX_APH);

  Long64_t nentries = fragTree->GetEntries();

  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
     Long64_t ientry = fragTree->LoadTree(jentry);
     if(ientry < 0) break; // reached the end of the file

     fragTree->GetEntry(jentry);

     // grid inefficiency correction
     pAn1 = (pAn1 - GRID_INEFFICIENCY*(pAn1 + pGr1))/(1 - GRID_INEFFICIENCY);
     pAn2 = (pAn2 - GRID_INEFFICIENCY*(pAn2 + pGr2))/(1 - GRID_INEFFICIENCY);

     pTheta1 /= g_AngMass1->Eval(pA1);
     pTheta2 /= g_AngMass2->Eval(pA2);

     pAn1 -= f_massAtt1->Eval(1.0/pTheta1) - f_massAtt1->Eval(0.0);
     pAn2 -= f_massAtt2->Eval(1.0/pTheta2) - f_massAtt2->Eval(0.0);

     if( (pAn1 > MIN_ANODE1) && (pAn2 > MIN_ANODE2) && (pTheta1 > MIN_ANG1) && (pTheta2 > MIN_ANG2) )
     {
       h1_massAn1->Fill(pAn1);
       h1_massAn2->Fill(pAn2);
     }
   }


  TF1* gaussFit = new TF1("gaussFit", "[0] * e^(-((x - [1])^2)/(2*[2]^2))", 0, MAX_APH);
  // yield 1
  TF1* f_gaussYield1 = new TF1("f_gaussYield1", "gaussFit + gaussFit", 0, MAX_APH);
  TF1* f_gaussYield2 = new TF1("f_gaussYield2", "gaussFit + gaussFit", 0, MAX_APH);

  double ampGuess, meanGuess, sigGuess;
  ampGuess = h1_massAn1->GetMaximum();
  meanGuess = h1_massAn1->GetMean();
  sigGuess = SIG_ANODE;
  // cout << ampGuess << " " << meanGuess << sigGuess << " " << endl;
  f_gaussYield1->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
  f_gaussYield1->SetParLimits(1, MIN_ANODE2, meanGuess);
  f_gaussYield1->SetParLimits(4, meanGuess, MAX_APH);
  h1_massAn1->Fit(f_gaussYield1, "N Q", "", 0, MAX_APH);

  ampGuess = h1_massAn2->GetMaximum();
  meanGuess = h1_massAn2->GetMean();
  sigGuess = SIG_ANODE;
  // cout << ampGuess << " " << meanGuess << " " << sigGuess << endl;
  f_gaussYield2->SetParameters(ampGuess*0.8, meanGuess*0.8, sigGuess, ampGuess, meanGuess*1.2, sigGuess);
  f_gaussYield2->SetParLimits(1, MIN_ANODE2, meanGuess);
  f_gaussYield2->SetParLimits(4, meanGuess, MAX_APH);
  h1_massAn2->Fit(f_gaussYield2, "N Q", "", 0, MAX_APH);

  double centroids1[2];
  double centroids2[2];
  double sigmas1[2];
  double sigmas2[2];

  centroids1[0] = f_gaussYield1->GetParameter(1);
  centroids1[1] = f_gaussYield1->GetParameter(4);

  sigmas1[0] = f_gaussYield1->GetParameter(2);
  sigmas1[1] = f_gaussYield1->GetParameter(5);

  centroids2[0] = f_gaussYield2->GetParameter(1);
  centroids2[1] = f_gaussYield2->GetParameter(4);

  sigmas2[0] = f_gaussYield2->GetParameter(2);
  sigmas2[1] = f_gaussYield2->GetParameter(5);



  if (centroids1[0] > centroids1[1])
  {
    double temp = centroids1[0];
    centroids1[0] = centroids1[1];
    centroids1[1] = temp;

    temp = sigmas1[0];
    sigmas1[0] = sigmas1[1];
    sigmas1[1] = temp;
  }
  if (centroids2[0] > centroids2[1])
  {
    double temp = centroids2[0];
    centroids2[0] = centroids2[1];
    centroids2[1] = temp;

    temp = sigmas2[0];
    sigmas2[0] = sigmas2[1];
    sigmas2[1] = temp;
  }

  // cout << "centroids source side: " << centroids1[0] << " " << centroids1[1] << endl;
  // cout << "centroids backing side: " << centroids2[0] << " " << centroids2[1] << endl;
  //
  // cout << "sigmas source side: " << sigmas1[0] << " " << sigmas1[1] << endl;
  // cout << "sigmas backing side: " << sigmas2[0] << " " << sigmas2[1] << endl;

  // calibrated results
  double kineticPeaks[2];
  kineticPeaks[0] = infoSystem->KINETIC_PEAKS[0] - PHD_H;
  kineticPeaks[1] = infoSystem->KINETIC_PEAKS[1] - PHD_L;

  TF1* f_calib1 = new TF1("f_calib1", "[0] + [1]*x", 0, 2*infoSystem->KINETIC_PEAKS[1]);
  TF1* f_calib2 = new TF1("f_calib2", "[0] + [1]*x", 0, 2*infoSystem->KINETIC_PEAKS[1]);

  g_massCalib1 = new TGraph(2, centroids1, kineticPeaks);
  g_massCalib1->SetName("g_massCalib1");
  g_massCalib1->Fit(f_calib1, "Q");

  g_massCalib2 = new TGraph(2, centroids2, kineticPeaks);
  g_massCalib2->SetName("g_massCalib2");
  g_massCalib2->Fit(f_calib2, "Q");

  // cout << "f_calib1: " << f_calib1->GetParameter(0) << " " << f_calib1->GetParameter(1) << endl;
  // cout << "f_calib2: " << f_calib2->GetParameter(0) << " " << f_calib2->GetParameter(1) << endl;

  TCanvas* c_gain = new TCanvas("c_massGain_" + TString(to_string(iterationPost)), "c_massGain", 400, 500);
  //c_gain->Divide(1,3);

  // draw
  c_gain->cd(1);
  h1_massAn1->SetLineColor(kBlue);
  h1_massAn2->SetLineColor(kRed);
  h1_massAn1->Draw();
  h1_massAn2->Draw("SAME");
  f_gaussYield1->SetLineColor(kBlue);
  f_gaussYield2->SetLineColor(kRed);
  f_gaussYield1->Draw("SAME");
  f_gaussYield2->Draw("SAME");

  fragDiagnostics->cd();
  cd_recursion->cd();
  c_gain->Write();




}
