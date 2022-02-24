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

  // fill frag tree
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     eventChain->GetEntry(jentry);

     // rejection region


     // angle filling
     if(g_Ang1->Eval(aph[0]) > 0)
     {
       fThetaL  = (gph[0]/aph[0])/g_Ang1->Eval(aph[0]);
     }
     else fThetaL  = -1;

     if(g_Ang2->Eval(aph[0]) > 0)
     {
     fThetaH  = (gph[1]/aph[1])/g_Ang2->Eval(aph[1]);
     }
     else fThetaH  = -1;


     // kinetic energy
     fKEL = aph[0];
     fKEH = aph[1];


     fKEL += f_att1->Eval(1.0/fThetaL) - f_att2->Eval(0);
     fKEH += f_att2->Eval(1.0/fThetaL) - f_att2->Eval(0);

     fragTree->Fill();
  }


  cout << "finished filling tree" << endl;


  fragFile->cd();
  fragTree->Write();

}
