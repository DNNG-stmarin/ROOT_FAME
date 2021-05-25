#include "readFiss.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void readFiss::LoopExp()
{

   cout << "Now looping through experiment. " << endl;

   if (expTree == 0) return;

   expEntries = expTree->GetEntries();
   cout << "Analyzing " << expEntries << " experimental events \n";

   int nMult, gMult, nMultBack, gMultBack;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<expEntries;jentry++) {
      Long64_t ientry = LoadExpTree(jentry);
      if (ientry < 0) break;
      nb = expTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      nMult = 0;
      gMult = 0;
      nMultBack = 0;
      gMultBack = 0;

      // loop through neutrons
      for (int i = 0; i < neutronMult; i++)
      {
        if ((neutronLightOut[i] > THRESHOLD) && (neutronDetTimes[i] < MAX_TIME_N) )
        {
            nMult++;
            neutronLightOutputExp->Fill(neutronLightOut[i]);
            neutronTofExp->Fill(neutronDetTimes[i]);
            neutronEnergyExp->Fill(neutronToFErg[i]);
            neutronPSDExp->Fill(neutronPSD[i]);
            neutronSinglesExp->Fill(neutronDet[i]);
        }
      }
      neutronMultExp->Fill(nMult);

      // loop through gamma rays
      for (int i = 0; i < gammaMult; i++)
      {
        if (photonLightOut[i] > THRESHOLD)
        {
          gMult++;
          photonLightOutputExp->Fill(photonLightOut[i]);
          photonTofExp->Fill(photonDetTimes[i]);
          photonPSDExp->Fill(photonPSD[i]);
          photonSinglesExp->Fill(photonDet[i]);
          neutronMultPhotonLO->Fill(nMult, photonLightOut[i]);
        }
      }
      photonMultExp->Fill(gMult);
      neutronGammaMult->Fill(nMult, gMult);

      // loop through back neutrons
      for (int i = 0; i < neutronBackMult; i++)
      {
        if (backNeutronLightOut[i] > THRESHOLD)
        {
            nMultBack++;
            neutronLightOutputBack->Fill(backNeutronLightOut[i]);
            neutronTofBack->Fill(backNeutronDetTimes[i]);
            neutronEnergyBack->Fill(backNeutronToFErg[i]);
            neutronPSDBack->Fill(backNeutronPSD[i]);
            neutronSinglesBack->Fill(backNeutronDet[i]);
        }
      }
      neutronMultBack->Fill(nMultBack);

      // loop through back photons
      for (int i = 0; i < gammaBackMult; i++)
      {
        if (backPhotonLightOut[i] > THRESHOLD)
        {
          gMultBack++;
          photonLightOutputBack->Fill(backPhotonLightOut[i]);
          photonTofBack->Fill(backPhotonDetTimes[i]);
          photonPSDBack->Fill(photonPSD[i]);
          photonSinglesBack->Fill(backPhotonDet[i]);
        }
      }
      photonMultBack->Fill(gMultBack);

   }
}


void readFiss::LoopSim()
{
    cout << "Now looping through simulation. " << endl;

    if (expTree == 0) return;
    //CHANGE BACK TO SIM TREE
    simEntries = simTree->GetEntries();
    cout << "Analyzing " << simEntries << " simulated events \n ";

    int nMult, gMult, nMultBack, gMultBack;
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < simEntries; jentry++) {
        Long64_t ientry = LoadSimTree(jentry);
        if (ientry < 0) break;
        nb = simTree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        nMult = 0;
        gMult = 0;
        nMultBack = 0;
        gMultBack = 0;


        // loop through neutrons
        for (int i = 0; i < neutronMult; i++)
        {
          if (neutronIntegral[i] > THRESHOLD && neutronDetTimes[i] < MAX_TIME_N)
          {
              nMult++;
              neutronLightOutputSim->Fill(neutronIntegral[i]);
              neutronTofSim->Fill(neutronDetTimes[i]);
              neutronEnergySim->Fill(neutronEnergy[i]);
              neutronSinglesSim->Fill(neutronChannel[i]);
          }
        }
        neutronMultSim->Fill(nMult);

        // loop through gamma rays
        for (int i = 0; i < gammaMult; i++)
        {
          if (photonIntegral[i] > THRESHOLD)
          {
              gMult++;
              photonLightOutputSim->Fill(photonIntegral[i]);
              photonTofSim->Fill(photonDetTimes[i]);
              photonSinglesSim->Fill(photonChannel[i]);
          }
        }
        photonMultSim->Fill(gMult);

    }
}

void readFiss::LoopBeam()
{
  cout << "LoopBeam NOT IMPLEMENTED. INPUT ANYTHING IF YOU UNDERSTAND." <<
          " THE PROGRAM WILL CRASH." << endl;
  TString IUnderstandAndIAmImplementingTheMethod;
  cin >> IUnderstandAndIAmImplementingTheMethod;
}
