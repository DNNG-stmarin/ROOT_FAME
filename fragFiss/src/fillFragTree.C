#include "fragFiss.h"
#include "constants.h"
#include <TH2.h>
#include <TF1.h>

void fragFiss::FillFragTree()
{

  cout << "Creating fragment tree " << endl;
  // initialize fragment tree
  fragTree = new TTree("FragmentTree", "Tree of Fragments");
  fragTree->SetFileNumber(0);
  fragTree->SetMaxTreeSize(1000000000LL);


  // fragment assigned quantities
  double fT;
  double fAL, fAH;
  double fKEL, fKEH;
  double fThetaL, fThetaH;
  double fEX;

  fragTree->Branch("fT", &fT, "fT/D");
  fragTree->Branch("fAL", &fAL, "fAL/D");
  fragTree->Branch("fAH", &fAH, "fAH/D");
  fragTree->Branch("fKEL", &fKEL, "fKEL/D");
  fragTree->Branch("fKEH", &fKEH, "fKEH/D");
  fragTree->Branch("fThetaL", &fThetaL, "fThetaL/D");
  fragTree->Branch("fThetaH", &fThetaH, "fThetaH/D");
  fragTree->Branch("fEX", &fEX, "fEX/D");

  // fragment agnostic quantities
  double fA1, fA2;
  double fKE1, fKE2;
  double fTheta1, fTheta2;

  double preA1, preA2, preA1b, preA2b, postA1, postA2, preE1, preE2, postE1, postE2;


  // fill frag tree
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     eventChain->GetEntry(jentry);

     // import the time
     fT = ct;

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
     else fTheta2  = -1;

     if (fTheta1 > 2) fTheta1 = 2;
     if (fTheta2 > 2) fTheta2 = 2;

     // kinetic energy
     fKE1 = aph[0];
     fKE2 = aph[1];
     // attenutation correction
     fKE1 += f_att1->Eval(1.0/fTheta1) - f_att1->Eval(0);
     fKE2 += f_att2->Eval(1.0/fTheta2) - f_att2->Eval(0);
     // gain matching
     fKE2 = g_gainMatch->Eval(fKE2);

     // mass calculation

     preA1 = A_TOT * fKE2 / (fKE1 + fKE2);
     preA2 = A_TOT * fKE1 / (fKE1 + fKE2);
     postA1 = preA1;
     postA2 = preA2;

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

       // for (int i = 0; i < 2; i++)
       // {
       //   postE[j] =  A0*(iph[j] + phd_lookup(mpost[j],pulseheight));
       // }
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


     if (preA1 > preA2)
     {
       fAH = preA1;
       fKEH = preE1;
       fThetaH = fTheta1;

       fAL = preA2;
       fKEL = preE2;
       fThetaL = fTheta2;

       // if (jentry % 1000 == 0)
       // {
       //   cout << jentry << " " << preA1 << " " << preE1 << " | " <<  preA2 << " " << preE2 << endl;
       // }

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

     // convert to energy
     fKEL = g_calib->Eval(fKEL);
     fKEH = g_calib->Eval(fKEH);

     // extract excitation energy
     fEX = g_bindErg->Eval(fAL) - (fKEL + fKEH);

     // if event passes the test, fill the tree
     if( (fKE1 > MIN_ANODE1) && (fKE2 > MIN_ANODE2) && (fTheta1 > MIN_ANG1) && (fTheta2 > MIN_ANG2))
     {
       fragTree->Fill();
     }

  }


  cout << "finished filling tree" << endl;


  fragFile->cd();
  // g_sawtooth->Write();
  fragTree->Write();

}
