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
  double preA[2], preAb[2], postA[2], preE[2], postE[2];

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

     if(g_Ang2->Eval(aph[0]) > 0)
     {
     fTheta2  = (gph[1]/aph[1])/g_Ang2->Eval(aph[1]);
     }
     else fTheta2  = -1;


     // kinetic energy
     fKE1 = aph[0];
     fKE2 = aph[1];
     // attenutation correction
     fKE1 += f_att1->Eval(1.0/fThetaL) - f_att2->Eval(0);
     fKE2 += f_att2->Eval(1.0/fThetaL) - f_att2->Eval(0);
     // gain matching
     fKE2 = g_gainMatch->Eval(fKE2);



     // mass calculation

     for (int in = 0; in < 2; in++)
     {
       preA[in] = A_TOT / 2.0;
       preAb[in] = A_TOT / 2.0;
       postA[in] = A_TOT / 2.0;
     }
     preE[0] = fKE1;
     preE[1] = fKE2;
     postE[0] = fKE1;
     postE[1] = fKE2;

     int iterCounter = 0;
     while(iterCounter < MAX_CONV_ITER)
     {
       preAb[0] = preA[0];
       preAb[1] = preA[1];

       for (int i = 0; i < 2; i++)
       {
         postA[i] = preA[i] - g_sawtooth->Eval(postA[i]);
       }
       // for (int i = 0; i < 2; i++)
       // {
       //   postE[j] =  A0*(iph[j] + phd_lookup(mpost[j],pulseheight));
       // }
       double B = preA[1] * postA[0] / (preA[0] * postA[1]);

       preA[0] = A_TOT * postE[1] / (postE[0] / B + postE[1]);
       preA[1] = A_TOT * postE[0] / (postE[1] * B + postE[0]);

       for (int i = 0; i < 2; i++)
       {
         preE[i] = postE[i] * preA[i] / postA[i];
       }

       if (abs(preA[0] - preAb[0]) < CONVERGEANCE_CONST &&
           abs(preA[1] - preAb[1]) < CONVERGEANCE_CONST)
       {
         break;
       }
       iterCounter++;
     }

     if (preA[0] > preA[1])
     {
       fAH = preA[0];
       fAL = preA[1];
       fKEL = preE[0];
       fThetaL = fTheta1;
       fKEH = preE[1];
       fThetaH = fTheta2;

     }
     else
     {
       fAH = preA[1];
       fAL = preA[0];
       fKEL = preE[0];
       fThetaL = fTheta2;
       fKEH = preE[1];
       fThetaH = fTheta1;

     }
     // if (jentry % 1000 == 0) cout << fAH << " " << fAL << endl;

     // convert to energy
     fKEL = g_calib->Eval(fKE1);
     fKEH = g_calib->Eval(fKE2);

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
