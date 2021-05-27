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
   if(mode == 2)
   {
     cout << "Now looping through beam. " << endl;
   }

   if (expTree == 0) return;

   expEntries = expTree->GetEntries();
   cout << "Analyzing " << expEntries << " experimental events \n";
   if(mode == 2)
   {
     cout << "Analyzing (again)" << expEntries << " experimental events \n ";
   }

   int nMult, gMult, nMultBack, gMultBack, indexChannel;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<expEntries;jentry++) {
      Long64_t ientry = LoadExpTree(jentry);
      if (ientry < 0) break;
      nb = expTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(mode == 2)
      {
        indexChannel = isTrigger(fisChan); // this should be a function of fisChan

        if(indexChannel < 0)
        {
          cout << "Trigger number " << fisChan << " not recognized." << endl;
          exit(10);
        }
      }

      bool validBeam = (mode == 2) &&
                       (beamEnergy > BEAM_ERG_MIN && beamEnergy < BEAM_ERG_MAX);
      if(validBeam)
      {
        h_fisDep[indexChannel]->Fill(fisDep);
        h_beamTime[indexChannel]->Fill(beamTime);
        h2_fisDepErg[indexChannel]->Fill(fisDep, beamEnergy);
      }

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
            neutronEnergyLO->Fill(neutronToFErg[i], neutronLightOut[i]);
            neutronLightOutPSD->Fill(neutronLightOut[i], neutronPSD[i]);
        }
      }
      neutronMultExp->Fill(nMult);
      if(validBeam)
      {
        h2_neutronMultDep[indexChannel]->Fill(fisDep, nMult);
        if (fisDep > THRESHOLD_DEP)
        {
          h2_neutronMultErg[indexChannel]->Fill(beamEnergy, nMult);
        }
      }

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
      if(validBeam)
      {
        h2_gammaMultDep[indexChannel]->Fill(fisDep, gMult);
        if (fisDep > THRESHOLD_DEP)
        {
          h2_gammaMultErg[indexChannel]->Fill(beamEnergy, gMult);
        }
      }

      // loop through back neutrons
      for (int i = 0; i < neutronBackMult; i++)
      {
        if ((backNeutronLightOut[i] > THRESHOLD) && (backNeutronDetTimes[i] + BACKGROUND_DELAY < MAX_TIME_N))
        {
            nMultBack++;
            neutronLightOutputBack->Fill(backNeutronLightOut[i]);
            neutronTofBack->Fill(backNeutronDetTimes[i] + BACKGROUND_DELAY);
            neutronEnergyBack->Fill(backNeutronToFErg[i]);
            neutronPSDBack->Fill(backNeutronPSD[i]);
            neutronSinglesBack->Fill(backNeutronDet[i]);
        }
      }
      neutronMultBack->Fill(nMultBack);
      if(validBeam)
      {
        h2_backNeutronMultDep[indexChannel]->Fill(fisDep, nMultBack);
        if (fisDep > THRESHOLD_DEP)
        {
          h2_backNeutronMultErg[indexChannel]->Fill(beamEnergy, nMultBack);
        }
      }

      // loop through back photons
      for (int i = 0; i < gammaBackMult; i++)
      {
        if (backPhotonLightOut[i] > THRESHOLD)
        {
          gMultBack++;
          photonLightOutputBack->Fill(backPhotonLightOut[i]);
          photonTofBack->Fill(backPhotonDetTimes[i] + BACKGROUND_DELAY);
          photonPSDBack->Fill(photonPSD[i]);
          photonSinglesBack->Fill(backPhotonDet[i]);
        }
      }
      photonMultBack->Fill(gMultBack);
      if(validBeam)
      {
        h2_backGammaMultDep[indexChannel]->Fill(fisDep, gMultBack);
        if (fisDep > THRESHOLD_DEP)
        {
          h2_backGammaMultErg[indexChannel]->Fill(beamEnergy, gMultBack);
        }
      }

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
    cout << "Now looping through beam. " << endl;

    if (expTree == 0) return;
    //CHANGE BACK TO SIM TREE
    expEntries = expTree->GetEntries();
    cout << "Analyzing (again)" << expEntries << " experimental events \n ";

    int nMult, gMult, nMultBack, gMultBack, indexChannel;
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < expEntries; jentry++)
    {
        Long64_t ientry = LoadExpTree(jentry);
        if (ientry < 0) break;
        nb = expTree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;
        indexChannel = isTrigger(fisChan); // this should be a function of fisChan

        if(indexChannel < 0)
        {
          cout << "Trigger number " << fisChan << " not recognized." << endl;
          exit(10);
        }

        // skip if the energy of the beam is outside the range
        // nathan remove

        if(beamEnergy > BEAM_ERG_MIN && beamEnergy < BEAM_ERG_MAX)
        {
          h_fisDep[indexChannel]->Fill(fisDep);
          h_beamTime[indexChannel]->Fill(beamTime);
          h2_fisDepErg[indexChannel]->Fill(fisDep, beamEnergy);

        }
        else
        {
          // cout << "energy not recognized: " << beamEnergy << endl;
          continue;
        }

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
          }
        }
        h2_neutronMultDep[indexChannel]->Fill(fisDep, nMult);

        if (fisDep > THRESHOLD_DEP)
        {
          h2_neutronMultErg[indexChannel]->Fill(beamEnergy, nMult);
        }

        // loop through gamma rays
        for (int i = 0; i < gammaMult; i++)
        {
          if (photonLightOut[i] > THRESHOLD)
          {
            gMult++;
          }
        }
        h2_gammaMultDep[indexChannel]->Fill(fisDep, gMult);

        if (fisDep > THRESHOLD_DEP)
        {
          h2_gammaMultErg[indexChannel]->Fill(beamEnergy, gMult);
        }

        // loop through back neutrons
        for (int i = 0; i < neutronBackMult; i++)
        {
          if ((backNeutronLightOut[i] > THRESHOLD) && (backNeutronDetTimes[i] + BACKGROUND_DELAY < MAX_TIME_N))
          {
            nMultBack++;
          }
        }
        h2_backNeutronMultDep[indexChannel]->Fill(fisDep, nMultBack);

        if (fisDep > THRESHOLD_DEP)
        {
          h2_backNeutronMultErg[indexChannel]->Fill(beamEnergy, nMultBack);
        }


        // loop through back photons
        for (int i = 0; i < gammaBackMult; i++)
        {
          if (backPhotonLightOut[i] > THRESHOLD)
          {
            gMultBack++;
          }
        }
        h2_backGammaMultDep[indexChannel]->Fill(fisDep, gMultBack);

        if (fisDep > THRESHOLD_DEP)
        {
          h2_backGammaMultErg[indexChannel]->Fill(beamEnergy, gMultBack);
        }
    }
}
