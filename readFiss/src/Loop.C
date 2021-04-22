#include "readFiss.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void readFiss::LoopExp()
{

   if (expTree == 0) return;

   Long64_t nentries = expTree->GetEntriesFast();
  
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadExpTree(jentry);
      if (ientry < 0) break;
      nb = expTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      // loop through neutrons
      for (int i = 0; i < neutronMult; i++)
      {     
        if (neutronLightOut[i] > 0.1) {
            neutronLightOutputExp->Fill(neutronLightOut[i]);
            neutronTofExp->Fill(neutronDetTimes[i]);
            neutronEnergyExp->Fill(neutronToFErg[i]);
        }
      }

      // loop through gamma rays
      for (int i = 0; i < gammaMult; i++)
      { 
        if (photonLightOut[i] > 0.1) 
        {
          photonLightOutputExp->Fill(photonLightOut[i]);
          photonTofExp->Fill(photonDetTimes[i]);
        }
      }

      // loop through back neutrons
      for (int i = 0; i < neutronBackMult; i++)
      {     
        if (backNeutronLightOut[i] > 0.1) {
            neutronLightOutputBack->Fill(backNeutronLightOut[i]);
            neutronTofBack->Fill(backNeutronDetTimes[i]);
            neutronEnergyBack->Fill(backNeutronToFErg[i]);
        }
      }

      // loop through back photons
      for (int i = 0; i < gammaBackMult; i++)
      { 
        if (backPhotonLightOut[i] > 0.1) 
        {
          photonLightOutputBack->Fill(backPhotonLightOut[i]);
          photonTofBack->Fill(backPhotonDetTimes[i]);
        }
      }
   }
}


void readFiss::LoopSim()
{

    if (expTree == 0) return;
    //CHANGE BACK TO SIM TREE
    Long64_t nentries = expTree->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        Long64_t ientry = LoadSimTree(jentry);
        if (ientry < 0) break;
        nb = simTree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        // loop through neutrons
        for (int i = 0; i < neutronMult; i++)
        {
          if (neutronIntegral[i] > 0.1) {
              neutronLightOutputSim->Fill(neutronIntegral[i]);
              neutronTofSim->Fill(neutronDetTimes[i]);
              neutronEnergySim->Fill(neutronEnergy[i]);
          }
        }

        // loop through gamma rays
        for (int i = 0; i < gammaMult; i++)
        {
          if (photonIntegral[i] > 0.1) {
              photonLightOutputSim->Fill(photonIntegral[i]);
              photonTofSim->Fill(photonDetTimes[i]);
          }
        }
                
                
            
        
    }
}