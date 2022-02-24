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

     if(fKE1 > fKE2)
     {
       fKEL = fKE1;
       fThetaL = fTheta1;
       fKEH = fKE2;
       fThetaH = fTheta2;
     }
     else
     {
       fKEL = fKE2;
       fThetaL = fTheta2;
       fKEH = fKE1;
       fThetaH = fTheta1;
     }


     // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     // NATHAN put mass calculations here
     // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


     // rejection
     if( (fKE1 > MIN_ANODE1) && (fKE2 > MIN_ANODE2) && (fTheta1 > MIN_ANG1) && (fTheta2 > MIN_ANG2))
     {
       fragTree->Fill();
     }

  }


  cout << "finished filling tree" << endl;


  fragFile->cd();
  fragTree->Write();

}
