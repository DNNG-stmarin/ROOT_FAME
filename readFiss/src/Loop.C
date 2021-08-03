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
   long int numFissIter = 0;

   int nMult, gMult, nMultBack, gMultBack, indexChannel, indexDet;
   Double_t currTime;
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<expEntries;jentry++)
   {
      Long64_t ientry = LoadExpTree(jentry);
      if (ientry < 0) break;
      nb = expTree->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      indexChannel = isTrigger(fisChan); // this should be a function of fisChan



      // broken channel cut
      if(indexChannel < 0)
      {
        cout << "Trigger number " << fisChan << " not recognized." << endl;
        exit(10);
      }

       // beam coincidence cut
      bool validBeam = (mode == 2) &&
                       (beamEnergy > BEAM_ERG_MIN && beamEnergy < BEAM_ERG_MAX);
      if((mode == 2) && !(beamEnergy > BEAM_ERG_MIN && beamEnergy < BEAM_ERG_MAX))
      {
        fissRej->Fill(CUT_BEAM);
        continue;
      }

      // store values for histograms
      h_fisDep[indexChannel]->Fill(fisDep);
      if(mode == 2)
      {
        h_beamTime[indexChannel]->Fill(beamTime);
        h2_fisDepErg[indexChannel]->Fill(fisDep, beamEnergy);
      }

      // trigger threshold cut
      if(!(fisDep > THRESHOLD_DEP && fisDep < CLIPPING_DEP))
      {
        fissRej->Fill(CUT_DEP);
        continue;
      }

      // fission pile-up test
      currTime = fisTime;
      expTree->GetEntry(jentry+1);
      h_timeDiffTrig[indexChannel]->Fill(fisTime - currTime);
      if(fisTime - currTime < FISS_PILEUP_TIME)
      {
        fissRej->Fill(CUT_PILEUP);
        continue;
      }
      else
      {
        expTree->GetEntry(jentry);
      }

      // fissions passed all the tests, delete
      numFissIter++;
      // cout << numFissIter << endl;
      fissRej->Fill(ACCEPTED_SIGNAL);
      if(numFissIter%1000000 == 0)
      {
        cout << "finished processing " << numFissIter << " fissions" << endl;
      }
      nMult = 0;
      gMult = 0;
      nMultBack = 0;
      gMultBack = 0;

      // loop through neutrons
      int n1 = -1, n2 = -1;
      for (int i = 0; i < neutronMult; i++)
      {
        if ((neutronLightOut[i] > THRESHOLD) && (neutronLightOut[i] < CLIPPING) && (neutronDetTimes[i] < MAX_TIME_N) )
        {
            nMult++;
            neutronLightOutputExp->Fill(neutronLightOut[i]);
            neutronTofExp->Fill(neutronDetTimes[i]);
            neutronEnergyExp->Fill(neutronToFErg[i]);
            neutronPSDExp->Fill(neutronPSD[i]);
            neutronSinglesExp->Fill(neutronDet[i]);
            neutronEnergyLOExp->Fill(neutronToFErg[i], neutronLightOut[i]);
            neutronLightOutPSDExp->Fill(neutronLightOut[i], neutronPSD[i]);

            indexDet = neutronDet[i];
            if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
            {
              cout << "Detector number " << neutronDet[i] << " not recognized from neutronDet." << endl;
              exit(10);
            }

            IndivNeutronLightOutputExp[indexDet]->Fill(neutronLightOut[i]);
            IndivNeutronTofExp[indexDet]->Fill(neutronDetTimes[i]);
            IndivNeutronEnergyExp[indexDet]->Fill(neutronToFErg[i]);
            IndivNeutronPSDExp[indexDet]->Fill(neutronPSD[i]);

            IndivNeutronEnergyLOExp[indexDet]->Fill(neutronToFErg[i], neutronLightOut[i]);
            IndivNeutronLightOutPSDExp[indexDet]->Fill(neutronLightOut[i], neutronPSD[i]);

            if(nMult == 1)
            {
              n1 = i;
            }
            else if(nMult == 2)
            {
              n2 = i;
            }

            if (validBeam)
            {
              h2_nLightOutErg[indexChannel]->Fill(beamEnergy, neutronLightOut[i]);
              h2_nToFErg[indexChannel]->Fill(beamEnergy, neutronToFErg[i]);
            }
        }
      }
      neutronMultExp->Fill(nMult);
      h2_neutronMultDep[indexChannel]->Fill(fisDep, nMult);
      if(nMult == 2)
      {
        // double dot = (neutronVx[n1] * neutronVx[n2]) +
        //              (neutronVy[n1] * neutronVy[n2]) +
        //              (neutronVz[n1] * neutronVz[n2]); // dot product
        // double Mn1 = sqrt(pow(neutronVx[n1], 2) +
        //                   pow(neutronVy[n1], 2) +
        //                   pow(neutronVz[n1], 2)); // magnitude of n1
        // double Mn2 = sqrt(pow(neutronVx[n2], 2) +
        //                   pow(neutronVy[n2], 2) +
        //                   pow(neutronVz[n2], 2)); // magnitude of n2
        // neutronAngleCorr->Fill(dot / (Mn1 * Mn2));
        if(neutronDet[n1] > neutronDet[n2])
        {
          neutronDoublesMat->Fill((int)neutronDet[n1], (int)neutronDet[n2]);
          // neutronAngleCorr->Fill(angles[(int)neutronDet[n1]][(int)neutronDet[n2]]);
        }
        else
        {
          neutronDoublesMat->Fill((int)neutronDet[n2], (int)neutronDet[n1]);
          // neutronAngleCorr->Fill(angles[(int)neutronDet[n2]][(int)neutronDet[n1]]);
        }
      }

      if(validBeam)
      {
        h2_neutronMultErg[indexChannel]->Fill(beamEnergy, nMult);
      }

      // loop through gamma rays
      for (int i = 0; i < gammaMult; i++)
      {
        if (photonLightOut[i] > THRESHOLD && photonLightOut[i] < CLIPPING )
        {
          gMult++;
          photonLightOutputExp->Fill(photonLightOut[i]);
          photonTofExp->Fill(photonDetTimes[i]);
          photonPSDExp->Fill(photonPSD[i]);
          photonSinglesExp->Fill(photonDet[i]);
          neutronMultPhotonLOExp->Fill(nMult, photonLightOut[i]);
          photonLightOutPSDExp->Fill(photonLightOut[i], photonPSD[i]);

          indexDet = photonDet[i];
          if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
          {
            cout << "Detector number " << photonDet[i] << " not recognized from photonDet." << endl;
            exit(10);
          }

          IndivPhotonLightOutputExp[indexDet]->Fill(photonLightOut[i]);
          IndivPhotonTofExp[indexDet]->Fill(photonDetTimes[i]);
          IndivPhotonPSDExp[indexDet]->Fill(photonPSD[i]);

          IndivPhotonLightOutPSDExp[indexDet]->Fill(photonLightOut[i], photonPSD[i]);

          if (validBeam)
          {
            h2_photonLightOutErg[indexChannel]->Fill(beamEnergy, photonLightOut[i]);
          }
        }
      }
      photonMultExp->Fill(gMult);
      h2_gammaMultDep[indexChannel]->Fill(fisDep, gMult);
      if(validBeam)
      {
        h2_gammaMultErg[indexChannel]->Fill(beamEnergy, gMult);
      }


      neutronGammaMultExp->Fill(nMult, gMult); // correlated plot

      // loop through back neutrons
      for (int i = 0; i < neutronBackMult; i++)
      {
        if ((backNeutronLightOut[i] > THRESHOLD) && (backNeutronLightOut[i] < CLIPPING ) && (backNeutronDetTimes[i] + BACKGROUND_DELAY < MAX_TIME_N))
        {
            nMultBack++;
            neutronLightOutputBack->Fill(backNeutronLightOut[i]);
            neutronTofBack->Fill(backNeutronDetTimes[i] + BACKGROUND_DELAY);
            neutronEnergyBack->Fill(backNeutronToFErg[i]);
            neutronPSDBack->Fill(backNeutronPSD[i]);
            neutronSinglesBack->Fill(backNeutronDet[i]);

            indexDet = backNeutronDet[i];
            if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
            {
              cout << "Detector number " << backNeutronDet[i] << " not recognized from backNeutronDet." << endl;
              exit(10);
            }

            IndivNeutronLightOutputBack[indexDet]->Fill(backNeutronLightOut[i]);
            IndivNeutronTofBack[indexDet]->Fill(backNeutronDetTimes[i] + BACKGROUND_DELAY);
            IndivNeutronEnergyBack[indexDet]->Fill(backNeutronToFErg[i]);
            IndivNeutronPSDBack[indexDet]->Fill(backNeutronPSD[i]);

            if(validBeam)
            {
              h2_nBackToFErg[indexChannel]->Fill(beamEnergy, backNeutronToFErg[i]);
              h2_nBackLightOutErg[indexChannel]->Fill(beamEnergy, backNeutronLightOut[i]);
              //get LO background
            }
        }
      }
      neutronMultBack->Fill(nMultBack);
      h2_backNeutronMultDep[indexChannel]->Fill(fisDep, nMultBack);
      if(validBeam)
      {
        h2_backNeutronMultErg[indexChannel]->Fill(beamEnergy, nMultBack);
      }

      // loop through back photons
      for (int i = 0; i < gammaBackMult; i++)
      {
        if (backPhotonLightOut[i] > THRESHOLD && backPhotonLightOut[i] < CLIPPING )
        {
          gMultBack++;
          photonLightOutputBack->Fill(backPhotonLightOut[i]);
          photonTofBack->Fill(backPhotonDetTimes[i] + BACKGROUND_DELAY);
          photonPSDBack->Fill(backPhotonPSD[i]);
          photonSinglesBack->Fill(backPhotonDet[i]);

          indexDet = backPhotonDet[i];
          if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
          {
            cout << "Detector number " << backPhotonDet[i] << " not recognized from backPhotonDet." << endl;
            exit(10);
          }

          IndivPhotonLightOutputBack[indexDet]->Fill(backPhotonLightOut[i]);
          IndivPhotonTofBack[indexDet]->Fill(backPhotonDetTimes[i] + BACKGROUND_DELAY);
          IndivPhotonPSDBack[indexDet]->Fill(backPhotonPSD[i]);

          if (validBeam)
          {
            h2_photonBackLightOutErg[indexChannel]->Fill(beamEnergy, backPhotonLightOut[i]);
          }
          //get background
        }
      }
      photonMultBack->Fill(gMultBack);
      h2_backGammaMultDep[indexChannel]->Fill(fisDep, gMultBack);
      if(validBeam)
      {
        h2_backGammaMultErg[indexChannel]->Fill(beamEnergy, gMultBack);
      }

      neutronGammaMultBack->Fill(nMultBack, gMultBack);
   }

   expEntries = numFissIter;
   cout << "We found " << expEntries << "valid measured fissions" << endl;

   fissRej->Draw();
}


void readFiss::LoopSim()
{
    cout << "Now looping through simulation. " << endl;

    if (expTree == 0) return;
    //CHANGE BACK TO SIM TREE
    simEntries = simTree->GetEntries();
    cout << "Analyzing " << simEntries << " simulated events \n ";

    long int numFissIter = 0;
    int nMult, gMult, nMultBack, gMultBack, indexDet;
    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < simEntries; jentry++)
    {
        Long64_t ientry = LoadSimTree(jentry);
        if (ientry < 0) break;
        nb = simTree->GetEntry(jentry);   nbytes += nb;
        // if (Cut(ientry) < 0) continue;

        nMult = 0;
        gMult = 0;
        nMultBack = 0;
        gMultBack = 0;
        numFissIter++;
        if(numFissIter%1000000 == 0)
        {
          cout << "finished processing " << numFissIter << " simulated fissions" << endl;
        }


        // loop through neutrons
        for (int i = 0; i < neutronMult; i++)
        {
          if (neutronIntegral[i] > THRESHOLD && neutronIntegral[i] < CLIPPING && neutronDetTimes[i] < MAX_TIME_N)
          {
              nMult++;
              neutronLightOutputSim->Fill(neutronIntegral[i]);
              neutronTofSim->Fill(neutronDetTimes[i]);
              neutronEnergySim->Fill(neutronEnergy[i]);
              neutronSinglesSim->Fill(neutronChannel[i]);

              indexDet = neutronChannel[i];
              if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
              {
                cout << "Detector number " << neutronChannel[i] << " not recognized from neutronChannel." << endl;
                exit(10);
              }

              IndivNeutronLightOutputSim[indexDet]->Fill(neutronIntegral[i]);
              IndivNeutronTofSim[indexDet]->Fill(neutronDetTimes[i]);
              IndivNeutronEnergySim[indexDet]->Fill(neutronEnergy[i]);
              IndivNeutronPSDSim[indexDet]->Fill(neutronPSD[i]);
          }
        }
        neutronMultSim->Fill(nMult);

        // loop through gamma rays
        for (int i = 0; i < gammaMult; i++)
        {
          if (photonIntegral[i] > THRESHOLD && photonIntegral[i] < CLIPPING )
          {
              gMult++;
              photonLightOutputSim->Fill(photonIntegral[i]);
              photonTofSim->Fill(photonDetTimes[i]);
              photonSinglesSim->Fill(photonChannel[i]);

              indexDet = photonChannel[i];
              if(indexDet < 0 || indexDet > (NUM_DETECTORS - 1))
              {
                cout << "Detector number " << photonChannel[i] << " not recognized from photonChannel." << endl;
                exit(10);
              }

              IndivPhotonLightOutputSim[indexDet]->Fill(photonIntegral[i]);
              IndivPhotonTofSim[indexDet]->Fill(photonDetTimes[i]);
              IndivPhotonPSDSim[indexDet]->Fill(photonPSD[i]);
          }
        }
        photonMultSim->Fill(gMult);

    }

    cout << "We found " << simEntries << " valid simulated fissions" << endl;
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
