/*
Author: Stefano Marin
Purpose: Loop through the entire the coincidence tree and create the fission
         tree.
Date: May 14th, Ann Arbor
*/

#include "DetectorSystemClass.h"
// #include "mappingFunctions.h"
#include <fstream>

void DetectorSystemClass::FissionAnalysis()
{
  fissionFile = new TFile(nameFission + ".root", "RECREATE");
  fissionTree = new TTree(nameFission, nameFission);
  InitFiss();

  fissionFile->cd();

  Long64_t nentries = tree->GetEntriesFast();
  // nentries = 1000000;
  Long64_t nbytes = 0, nb = 0;

  // neutron and photon multiplicities
  int nMult, pMult;
  int nBackMult, pBackMult;

  // detection time
  double timeDet;
  int numDet, numTrig;
  double engDet;

  double neutVelocity;

  // cout << "NUM_DET: " << NUM_DETS << endl;
  // for(int d =0; d < NUM_DETS; d++)
  // {
  //     cout << DETECTORS[d] << endl;
  // }

  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
    // load tree
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = tree->GetEntry(jentry);   nbytes += nb;

    if(jentry % 1000000 == 0)
    {
      cout << jentry << " fissions processed." << endl;
    }


	  if(tMult >= NUM_DETS)
	  {
		 continue;
	  }

    numTrig = isTrigger(tChan);

    // allocating the fission info
    f_fisTime = tTime;
    f_fisDep = tDep;
    f_fisChan = tChan;
    f_fisPSP = tPSP;


    // Define offset based on targetCoord.txt
    if (NUM_BEAMS > 0)
    {
      double distance = BEAM_DISTANCE + triggers[numTrig].offset; // Include offset for plate location
      f_beamTime = bTime - triggers[numTrig].beamDelay
        + distance / LIGHT_C;
      f_beamEnergy = MASS_NEUTRONS *
        (1 / pow(1 - pow(distance / f_beamTime / LIGHT_C, 2.), 0.5) - 1);
      // f_beamEnergy = 0.5 * MASS_NEUTRONS / pow(LIGHT_C, 2)
      //   * pow(BEAM_DISTANCE / f_beamTime, 2); Too classical for Ionel

      f_beamDep = bErg;
      f_beamPSP = bPSP;
      f_beamChan = bChan;
      f_beamIndex = bIndex;
    }

    // reset the neutron and photon multiplicities
    nMult = 0;
    pMult = 0;
    nBackMult = 0;
    pBackMult = 0;

    for(int j = 0; j < tMult; j++)
    {

      // find the number of the detector
      numDet = isDetector(totChan[j]);

      // Calculate distance to detector
      double adjZ = detectors[numDet].Z - triggers[numTrig].offset;
      detectors[numDet].distance = pow(pow(detectors[numDet].X, 2.)
        + pow(detectors[numDet].Y, 2.) + pow(adjZ, 2.), 0.5);

      // detection time corrected for delay
      timeDet = totToF[j] -  detectors[numDet].timeDelay[numTrig];

      engDet = totDep[j]/detectors[numDet].calibration;

      //if numdet is broken, continue and skip this detector
      bool quit = 0;
      for(int k=0; k< NUM_EXCLUDED; k++) {
        if(numDet == EXCLUDE_DETECTORS[k]) {
          quit = 1;
        }
      }
      if(quit) {
        continue;
      }

      // cuts for neutrons & lower than something
      if(totPSP[j] < 0 || totPSP[j] > 1)
      {
        continue;
      }

      if(
        (totPSP[j] > detectors[numDet].discPSD->Eval(engDet))
        &
        (timeDet > MIN_TIME_N)
        &
        (timeDet < MAX_TIME_N)
        &
        (engDet > DETECTOR_THRESHOLD)
        )
      {
        neutronDetTimes[nMult] = timeDet;
        neutronLightOut[nMult] = engDet;
        neutronPSD[nMult] = totPSP[j];
        neutVelocity = (1.0/LIGHT_C)*detectors[numDet].distance/timeDet;
        //cout << numDet << " " << detectors[numDet].distance << endl;
        neutronToFErg[nMult] = (1.0/2.0)*MASS_NEUTRONS*pow(neutVelocity,2);
        neutronDet[nMult] = numDet;
        neutronVx[nMult] = detectors[numDet].X/detectors[numDet].distance*neutVelocity;
        neutronVy[nMult] = detectors[numDet].Y/detectors[numDet].distance*neutVelocity;
        neutronVz[nMult] = adjZ /detectors[numDet].distance*neutVelocity;
        nMult++;
      }

      // cuts for gammas
      else if(
        (totPSP[j] < detectors[numDet].discPSD->Eval(engDet))  //^^ -0.03 +0.03
        &
        (timeDet > MIN_TIME_P)
        &
        (timeDet < MAX_TIME_P)
        &
        (engDet > DETECTOR_THRESHOLD)
        )
      {
        photonDetTimes[pMult] = timeDet;
        photonLightOut[pMult] = engDet;
        photonPSD[pMult] = totPSP[j];
        photonDet[pMult] = numDet;
        photonVx[pMult] = detectors[numDet].X/detectors[numDet].distance*LIGHT_C;
        photonVy[pMult] = detectors[numDet].Y/detectors[numDet].distance*LIGHT_C;
        photonVz[pMult] = (detectors[numDet].Z - triggers[numTrig].offset) /detectors[numDet].distance*LIGHT_C;
        pMult++;
      }

      // cuts for background neutrons
      else if(
        (totPSP[j] > detectors[numDet].discPSD->Eval(engDet))
        &
        (timeDet > MIN_TIME_N - BACKGROUND_SHIFT)
        &
        (timeDet < MAX_TIME_N - BACKGROUND_SHIFT)
        &
        (engDet > DETECTOR_THRESHOLD)
        )
      {
        backNeutronDetTimes[nBackMult] = timeDet;
        backNeutronLightOut[nBackMult] = engDet;
        backNeutronPSD[nBackMult] = totPSP[j];
        neutVelocity = detectors[numDet].distance/(timeDet+BACKGROUND_SHIFT);
        backNeutronToFErg[nBackMult] = (1.0/2.0)*MASS_NEUTRONS*pow(neutVelocity,2);
        backNeutronDet[nBackMult] = numDet;
        backNeutronVx[nBackMult] = detectors[numDet].X/detectors[numDet].distance*neutVelocity;
        backNeutronVy[nBackMult] = detectors[numDet].Y/detectors[numDet].distance*neutVelocity;
        backNeutronVz[nBackMult] = detectors[numDet].Z/detectors[numDet].distance*neutVelocity;
        nBackMult++;
      }

      // cuts for background photons
      else if(
        (totPSP[j] < detectors[numDet].discPSD->Eval(engDet))
        &
        (timeDet > MIN_TIME_P - BACKGROUND_SHIFT)
        &
        (timeDet < MAX_TIME_P - BACKGROUND_SHIFT)
        &
        (engDet > DETECTOR_THRESHOLD)
        )
      {
        backPhotonDetTimes[pBackMult] = timeDet;
        backPhotonLightOut[pBackMult] = engDet;
        backPhotonPSD[pBackMult] = totPSP[j];
        backPhotonDet[pBackMult] = numDet;
        backPhotonVx[pBackMult] = detectors[numDet].X/detectors[numDet].distance*LIGHT_C;
        backPhotonVy[pBackMult] = detectors[numDet].Y/detectors[numDet].distance*LIGHT_C;
        backPhotonVz[pBackMult] = detectors[numDet].Z/detectors[numDet].distance*LIGHT_C;
        pBackMult++;
      }

    }

    // set branches of final tree
    f_neutronMult = nMult;
    f_gammaMult = pMult;
    f_neutronBackMult = nBackMult;
    f_gammaBackMult = pBackMult;

    fissionTree->Fill();
  }

  fissionFile = fissionTree->GetCurrentFile();
  fissionFile->Write();
  fissionFile->Close();
}


//deltapsd, save covariances,
void DetectorSystemClass::FissionAnalysisLoop()
{
  fissionFile->cd();
  ofstream myfile;
  string filename = "file" + to_string(DETECTOR_THRESHOLD) + ".csv";
  myfile.open(filename);

  int times = 10;
  double startpsd = -0.03;
  double stoppsd = 0.03;
  double step = (abs(startpsd)+abs(stoppsd))/times;
  double deltapsd = startpsd;

  for(int delta=0; delta<times+1; delta++)
  {
    //create 2d histogram th2i
    TString deltaname= "multHist" + to_string(deltapsd);
    TH2I* multHist = new TH2I(deltaname, "MultHist; Mult N; Mult P", 10, 0, 10, 10, 0, 10);

    Long64_t nentries = tree->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;

    // neutron and photon multiplicities
    int nMult, pMult;
    int nBackMult, pBackMult;

    // detection time
    double timeDet;
    int numDet, numTrig;
    int engDet;

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
       // load tree
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = tree->GetEntry(jentry);   nbytes += nb;

       // allocating the fission info
       f_fisTime = tTime;
       f_fisDep = tDep;

       // reset the neutron and photon multiplicities
       nMult = 0;
       pMult = 0;
       nBackMult = 0;
       pBackMult = 0;

       numTrig = isTrigger(tChan);

       for(int j = 0; j < tMult; j++)
       {
         // find the number of the detector
        numDet = isDetector(totChan[j]);

        // detection time corrected for delay
        timeDet = totToF[j] -  detectors[numDet].timeDelay[numTrig];

        engDet = totDep[j]/detectors[numDet].calibration;


        // cuts for neutrons
        if(
          (totPSP[j] > detectors[numDet].discPSD->Eval(engDet) + deltapsd)  //+deltapsd
          &
          (timeDet > MIN_TIME_N)
          &
          (timeDet < MAX_TIME_N)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             nMult++;
        }

        // cuts for gammas
        else if(
          (totPSP[j] < detectors[numDet].discPSD->Eval(engDet) + deltapsd)  //^^ -0.03 +0.03
          &
          (timeDet > MIN_TIME_P)
          &
          (timeDet < MAX_TIME_P)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             pMult++;
        }

        // cuts for background neutrons
        else if(
          (totPSP[j] > detectors[numDet].discPSD->Eval(engDet) + deltapsd)
          &
          (timeDet > BACKGROUND_SHIFT + MIN_TIME_N)
          &
          (timeDet < BACKGROUND_SHIFT + MAX_TIME_N)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             nBackMult++;
        }

        // cuts for background photons
        else if(
          (totPSP[j] < detectors[numDet].discPSD->Eval(engDet) + deltapsd)
          &
          (timeDet> BACKGROUND_SHIFT + MIN_TIME_P)
          &
          (timeDet< BACKGROUND_SHIFT + MAX_TIME_P)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             pBackMult++;
        }

       }

       // set branches of final tree
       f_neutronMult = nMult;
       f_gammaMult = pMult;
       f_neutronBackMult = nBackMult;
       f_gammaBackMult = pBackMult;

       multHist->Fill(nMult, pMult);
       fissionTree->Fill();
    }
    cout << deltapsd << " " << multHist->GetCovariance() << " " << multHist->GetMean(1) << " " << multHist->GetMean(2) << endl;
    myfile << deltapsd << "," << multHist->GetCovariance() << "," << multHist->GetMean(1) << "," << multHist->GetMean(2) << "\n";
    multHist->Write();
    deltapsd += step;
  }

  myfile.close();
}
