#include "SFAME.h"

void sfame::readCollisionTree()
{

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("CollisionFile.root");
  if (!f || !f->IsOpen())
  {
    f = new TFile("CollisionFile.root");
  }
  f->GetObject("CollisionFile", collisionTree);


  fCurrent = -1;
  collisionTree->SetMakeClass(1);

  collisionTree->SetBranchAddress("History", &f_history, &b_History);
  collisionTree->SetBranchAddress("NumLines", &f_numLines, &b_NumLines);
  collisionTree->SetBranchAddress("Type", f_type, &b_Type);
  collisionTree->SetBranchAddress("EnergyDeposited", f_energyDep, &b_EnergyDeposited);
  collisionTree->SetBranchAddress("EnergyIncident", f_energyInc, &b_EnergyIncident);
  collisionTree->SetBranchAddress("Interaction", f_interaction, &b_Interaction);
  collisionTree->SetBranchAddress("Zaid", f_zaid, &b_Zaid);
  collisionTree->SetBranchAddress("Cell", f_cell, &b_Cell);
  collisionTree->SetBranchAddress("Time", f_time, &b_Time);
  collisionTree->SetBranchAddress("Weight", f_weight, &b_Weight);
  collisionTree->SetBranchAddress("PositionX", f_posX, &b_PositionX);
  collisionTree->SetBranchAddress("PositionY", f_posY, &b_PositionY);
  collisionTree->SetBranchAddress("PositionZ", f_posZ, &b_PositionZ);
  collisionTree->SetBranchAddress("Particle", f_particle, &b_Particle);
  collisionTree->SetBranchAddress("Scatters", f_scatters, &b_Scatters);
  collisionTree->SetBranchAddress("Code", f_code, &b_Code);
  collisionTree->SetBranchAddress("Generation", f_generation, &b_Generation);
}


Int_t sfame::GetCollEntry(Long64_t entry)
{
// Read contents of entry.
   if (!collisionTree) return 0;
   return collisionTree->GetEntry(entry);
}

Long64_t sfame::LoadCollTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!collisionTree) return -5;
   Long64_t centry = collisionTree->LoadTree(entry);
   if (centry < 0) return centry;
   if (collisionTree->GetTreeNumber() != fCurrent) {
      fCurrent = collisionTree->GetTreeNumber();
   }
   return centry;
}
