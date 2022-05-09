
#include "fragFiss.h"
#include "constants.h"

#include <TH2.h>
#include <TF1.h>
#include <TH3.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TCanvas.h>


void fragFiss::PostAngle(int iterationPost)
{
  TH2D* h2_massAttU1 = new TH2D("h2_massAttU1_" + TString(to_string(iterationPost)),"h2_massAttU1;attenuation;kinetic energy",N_BINS_RATIO,-1, 5,N_BINS_APH, 0, MAX_APH);
  TH2D* h2_massAttU2 = new TH2D("h2_massAttU2_" + TString(to_string(iterationPost)),"h2_massAttU2;attenuation;kinetic energy",N_BINS_RATIO,-1, 5,N_BINS_APH, 0, MAX_APH);

  TH2D* h2_massAtt1 = new TH2D("h2_massAtt1_" + TString(to_string(iterationPost)),"h2_massAtt1;attenuation;kinetic energy",N_BINS_RATIO,-1, 5,N_BINS_APH, 0, MAX_APH);
  TH2D* h2_massAtt2 = new TH2D("h2_massAtt2_" + TString(to_string(iterationPost)),"h2_massAtt2;attenuation;kinetic energy",N_BINS_RATIO,-1, 5,N_BINS_APH, 0, MAX_APH);

  cout << "Filling mass attenuation histograms" << endl;
  Long64_t nentries = fragTree->GetEntries();

  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
     Long64_t ientry = fragTree->LoadTree(jentry);
     if(ientry < 0) break; // reached the end of the file

     fragTree->GetEntry(jentry);

     // grid inefficiency corrections
     pAn1 = (pAn1 - GRID_INEFFICIENCY*(pAn1 + pGr1))/(1 - GRID_INEFFICIENCY);
     pAn2 = (pAn2 - GRID_INEFFICIENCY*(pAn2 + pGr2))/(1 - GRID_INEFFICIENCY);

     h2_massAttU1->Fill(1.0/pTheta1, pAn1);
     h2_massAttU2->Fill(1.0/pTheta2, pAn2);

     pTheta1 /= g_AngMass1->Eval(pA1);
     pTheta2 /= g_AngMass2->Eval(pA2);

     h2_massAtt1->Fill(1.0/pTheta1, pAn1);
     h2_massAtt2->Fill(1.0/pTheta2, pAn2);

  }

  // find the profiles
  TProfile* p1_massBacking1 = h2_massAtt1->ProfileX();
  TProfile* p1_massBacking2 = h2_massAtt2->ProfileX();

  TProfile* p1_massBackingU1 = h2_massAttU1->ProfileX();
  TProfile* p1_massBackingU2 = h2_massAttU2->ProfileX();

  // fits
  f_massAtt1 = new TF1("f_massAtt1", "[0] + [1]*x", -10, 10);
  f_massAtt2 = new TF1("f_massAtt2", "[0] + [1]*x", -10, 10);

  p1_massBacking1->Fit(f_massAtt1, "Q", "", MIN_INV_ANG_FIT, MAX_INV_ANG_FIT);
  p1_massBacking2->Fit(f_massAtt2, "Q", "", MIN_INV_ANG_FIT, MAX_INV_ANG_FIT);

  // // analysis of fits
  // // ask fredrik about this
  // double crossPoint = (f_att2->GetParameter(0) - f_att1->GetParameter(0))/(f_att1->GetParameter(1) - f_att2->GetParameter(1));
  // double heightPoint = f_att1->Eval(crossPoint);
  //
  // cout << "The intersect point is " << crossPoint << " and the height is " << heightPoint << endl;

  // canvas with angle results
  TCanvas* c_massAtt = new TCanvas("c_massAtt_" + TString(to_string(iterationPost)), "c_massAtt", 400, 500);
  // c_massAtt->Divide(3,1);

  c_massAtt->cd(1);
  p1_massBacking1->Draw();
  p1_massBacking1->SetName("p1_massBacking1");
  p1_massBacking1->SetLineColor(kBlue);
  f_massAtt1->Draw("SAME");
  p1_massBackingU1->SetLineColor(kBlue);
  p1_massBackingU1->SetLineStyle(kDashed);
  p1_massBackingU1->Draw("SAME");

  p1_massBacking2->Draw("SAME");
  p1_massBacking2->SetName("p1_massBacking2");
  p1_massBacking2->SetLineColor(kRed);
  f_massAtt2->Draw("SAME");
  p1_massBackingU2->SetLineColor(kRed);
  p1_massBackingU2->SetLineStyle(kDashed);
  p1_massBackingU2->Draw("SAME");

  fragDiagnostics->cd();
  cd_recursion->cd();
  c_massAtt->Write();



}
