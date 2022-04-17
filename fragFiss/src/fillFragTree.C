#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>
#include <TProfile.h>
#include <TCanvas.h>

void fragFiss::FillFragTree()
{

  cout << "Creating fragment tree " << endl;
  // initialize fragment tree
  fragTree = new TTree("FragmentTree", "Tree of Fragments");
  fragTree->SetFileNumber(0);
  fragTree->SetMaxTreeSize(1000000000LL);

  TH2D* h2_massAngle = new TH2D("h2_massAngle", "h2_massAngle;cos;A", 100,0,2,160,40,200);

  TH1D* h_calibratedAn1 = new TH1D("h_calibratedAn1","h_calibratedAn1;KE (MeV);Counts", N_BINS_APH, 0, MAX_KE);
  TH1D* h_calibratedAn2 = new TH1D("h_calibratedAn2","h_calibratedAn2;KE (MeV);Counts", N_BINS_APH, 0, MAX_KE);

  TH1D* h_rawAn1 = new TH1D("h_rawAn1","h_rawAn1;KE (MeV);Counts", N_BINS_APH, 0, MAX_APH);
  TH1D* h_rawAn2 = new TH1D("h_rawAn2","h_rawAn2;KE (MeV);Counts", N_BINS_APH, 0, MAX_APH);

  TH1D* h_corAn1 = new TH1D("h_corAn1","h_corAn1;KE (MeV);Counts", N_BINS_APH, 0, MAX_APH);
  TH1D* h_corAn2 = new TH1D("h_corAn2","h_corAn2;KE (MeV);Counts", N_BINS_APH, 0, MAX_APH);

  TH1D* h_rawMass1 = new TH1D("h_rawMass1","h_rawMass1;amu;Counts", 252, 0, 252);
  TH1D* h_rawMass2 = new TH1D("h_rawMass2","h_rawMass2;amu;Counts", 252, 0, 252);

  TH1D* h_finalMass = new TH1D("h_finalMass","h_finalMass;amu;Counts", 252, 0, 252);
  TH2D* h2_finalTKE = new TH2D("h2_finalTKE","h2_finalTKE;amu;<TKE>", 252, 0, 252, N_BINS_APH, 0, 2 * MAX_KE);


  // fragment assigned quantities
  double fT;
  Double_t fAL, fAH;
  Double_t fKEL, fKEH;
  Double_t fThetaL, fThetaH;
  Double_t fEX;
  Double_t fAn1, fAn2, fGr1, fGr2;
  Double_t fCat;


  fragTree->Branch("fT", &fT, "fT/D");
  fragTree->Branch("fAL", &fAL, "fAL/D");
  fragTree->Branch("fAH", &fAH, "fAH/D");
  fragTree->Branch("fKEL", &fKEL, "fKEL/D");
  fragTree->Branch("fKEH", &fKEH, "fKEH/D");
  fragTree->Branch("fThetaL", &fThetaL, "fThetaL/D");
  fragTree->Branch("fThetaH", &fThetaH, "fThetaH/D");
  fragTree->Branch("fEX", &fEX, "fEX/D");

  fragTree->Branch("fAn1", &fAn1, "fAn1/D");
  fragTree->Branch("fAn2", &fAn2, "fAn2/D");
  fragTree->Branch("fGr1", &fGr1, "fGr1/D");
  fragTree->Branch("fGr2", &fGr2, "fGr2/D");
  fragTree->Branch("fCat", &fCat, "fCat/D");

  // fragment agnostic quantities
  double fA1, fA2;
  double fKE1, fKE2;
  double fTheta1, fTheta2;

  fragTree->Branch("fKE1", &fKE1, "fKE1/D");
  fragTree->Branch("fKE2", &fKE2, "fKE2/D");

  double preA1, preA2, preA1b, preA2b, postA1, postA2, preE1, preE2, postE1, postE2;
  double fPH1, fPH2;

  Long64_t cumulatEntries = 0;
  long int nFiss = 0;
  // fill frag tree
  for(int fil = 0; fil < numFiles; fil++)
  {
    cumulatEntries += nentriesTree[fil];

    for (Long64_t jentry = cumulatEntries; jentry > cumulatEntries - nentriesTree[fil]; jentry--)
    {
       Long64_t ientry = LoadTree(jentry);

       if(ientry < 0) break; // reached the end of the file

       eventChain->GetEntry(jentry);

       // import the time and heights
       fT = ct;
       fAn1 = aph[0];
       fAn2 = aph[1];
       fGr1 = gph[0];
       fGr2 = gph[1];
       fCat = cph;



       // get attenuation corrected anode specs
       h_rawAn1->Fill(aph[0]);
       h_rawAn2->Fill(aph[1]);

       // angle filling
       if(g_Ang1->Eval(aph[0]) > 0)
       {
         fTheta1  = (gph[0]/aph[0])/g_Ang1->Eval(aph[0]);
       }
       else fTheta1 = -1;

       if(g_Ang2->Eval(aph[1]) > 0)
       {
         fTheta2  = (gph[1]/aph[1])/g_Ang2->Eval(aph[1]);
       }
       else fTheta2 = -1;

       if (fTheta1 > 2) fTheta1 = 2;
       if (fTheta2 > 2) fTheta2 = 2;

       // kinetic energy
       fKE1 = aph[0];
       fKE2 = aph[1];
       // attenutation correction
       fKE1 += f_att1->Eval(1.0/fTheta1) - f_att1->Eval(0);
       fKE2 += f_att2->Eval(1.0/fTheta2) - f_att2->Eval(0);
       // gain matching
       // fKE2 = g_gainMatch->Eval(fKE2);


       // store pulse heights
       fPH1 = fKE1;
       fPH2 = fKE2;

       h_corAn1->Fill(fPH1);
       h_corAn2->Fill(fPH2);

       // convert to energy
       fKE1 = g_calib1->Eval(fKE1);
       fKE2 = g_calib2->Eval(fKE2);


       // mass calculation
       preA1 = A_TOT * fKE2 / (fKE1 + fKE2);
       preA2 = A_TOT * fKE1 / (fKE1 + fKE2);
       postA1 = preA1;
       postA2 = preA2;

       h_rawMass1->Fill(preA1);
       h_rawMass2->Fill(preA2);

       preE1 = fKE1;
       postE1 = fKE1;
       preE2 = fKE2;
       postE2 = fKE2;

       int iterCounter = 0;
       // cout << endl;
       while(iterCounter < MAX_CONV_ITER)
       {

         // cout << preE[0] << " " << preE[1] << postE[0] << " " << postE[1] << endl;

         preA1b = preA1;
         preA2b = preA2;

         postA1 = preA1 - g_sawtooth->Eval(preA1);
         postA2 = preA2 - g_sawtooth->Eval(preA2);

         // Correct for PHD
         postE1 = fKE1 + g_phd->Eval(postA1);
         postE2 = fKE2 + g_phd->Eval(postA2);

         double B = preA2 * postA1 / (preA1 * postA2);

         preA1 = A_TOT * postE2 / (postE1 / B + postE2);
         preA2 = A_TOT * postE1 / (postE2 * B + postE1);

         preE1 = postE1 * preA1 / postA1;
         preE2 = postE2 * preA2 / postA2;


         if (abs(preA1 - preA1b) < CONVERGEANCE_CONST &&
             abs(preA2 - preA2b) < CONVERGEANCE_CONST)
         {
           break;
         }
         iterCounter++;
       }

       // save the corrected anode kinetic energies
       fKE1 = preE1;
       fKE2 = preE2;

       // assign to light and heavy
       if (preA1 > preA2)
       {
         fAH = preA1;
         fKEH = preE1;
         fThetaH = fTheta1;

         fAL = preA2;
         fKEL = preE2;
         fThetaL = fTheta2;

       }
       else
       {
         fAL = preA1;
         fKEL = preE1;
         fThetaL = fTheta1;

         fAH = preA2;
         fKEH = preE2;
         fThetaH = fTheta2;
       }

       if (iterCounter == MAX_CONV_ITER - 1) {cout << "Reached max. iterations"<< endl;}

       // if (jentry % 1000 == 0) cout << fAH << " " << fAL << endl;


       // extract excitation energy
       fEX = g_bindErg->Eval(fAL) - (fKEL + fKEH);

       // cout << fKEL + fKEH << endl;


       h2_massAngle->Fill(fThetaL, fAL);
       h2_massAngle->Fill(fThetaH, fAH);


       // if event passes the test, fill the tree
       if( (fPH1 > MIN_ANODE1) && (fPH2 > MIN_ANODE2) && (fTheta1 > MIN_ANG1) && (fTheta2 > MIN_ANG2)
            && (fTheta1 < MAX_ANG1) && (fTheta2 < MAX_ANG2))
       {
         fragTree->Fill();
         nFiss++;
         h_calibratedAn1->Fill(fKE1);
         h_calibratedAn2->Fill(fKE2);

         h_finalMass->Fill(fAL);
         h_finalMass->Fill(fAH);

         h2_finalTKE->Fill(fAL, fKEL + fKEH);
         h2_finalTKE->Fill(fAH, fKEL + fKEH);
       }

       if(jentry%100000 == 0) cout << "Filled " << nFiss << " fissions in " << fT/1e9 <<  " seconds." <<  endl;

    }
    // end of file loop
  }



  cout << "Finished filling tree" << endl;


  fragFile->cd();

  TCanvas* c_rawMass = new TCanvas("c_rawMass", "c_rawMass", 600, 400);
  c_rawMass->cd();
  h_rawMass1->SetLineColor(kBlue);
  h_rawMass2->SetLineColor(kRed);
  h_rawMass1->Draw();
  h_rawMass2->Draw("SAME");

  TCanvas* c_massAngle = new TCanvas("c_massAngle", "c_massAngle", 600, 400);
  c_massAngle->cd();
  h2_massAngle->Draw("COLZ");

  TCanvas* c_rawAn = new TCanvas("c_rawAn", "c_rawAn", 600, 400);
  c_rawAn->cd();
  h_rawAn1->SetLineColor(kBlue);
  h_rawAn2->SetLineColor(kRed);
  h_corAn1->SetLineColor(kBlue);
  h_corAn2->SetLineColor(kRed);
  h_corAn1->SetLineStyle(kDashed);
  h_corAn2->SetLineStyle(kDashed);
  h_rawAn1->Draw();
  h_rawAn2->Draw("SAME");
  h_corAn1->Draw("SAME");
  h_corAn2->Draw("SAME");


  TCanvas* c_calAn = new TCanvas("c_calAn", "c_calAn", 600, 400);
  c_calAn->cd();
  h_calibratedAn1->SetLineColor(kBlue);
  h_calibratedAn2->SetLineColor(kRed);
  h_calibratedAn1->Draw();
  h_calibratedAn2->Draw("SAME");

  TCanvas* c_finalMass = new TCanvas("c_finalMass","c_finalMass",600,400);
  c_finalMass->cd();
  h_finalMass->Draw();
  double scale = h_finalMass->GetMaximum() / g_fpy->Eval(h_finalMass->GetMaximumBin());
  for (int i = 0; i < g_fpy->GetN(); i++) g_fpy->GetY()[i] *= scale;
  g_fpy->Draw("SAME");

  TCanvas* c_tke = new TCanvas("c_tke","c_tke",600,400);
  c_tke->cd();
  h2_finalTKE->ProfileX()->Draw();
  g_tke->Draw("SAME");

  c_rawAn->Write();
  c_calAn->Write();

  c_rawMass->Write();
  c_finalMass->Write();
  c_massAngle->Write();


  c_tke->Write();
  h2_finalTKE->Write();

  fragFile = fragTree->GetCurrentFile();
  fragFile->Write();
}
