/*
Author: Stefano Marin
Purpose: Loop through the entire the coincidence tree and create the fission
         tree.
Date: May 14th, Ann Arbor
*/

//pls work
#include "DetectorSystemClass.h"
#include "mappingFunctions.h"
#include <fstream>

void DetectorSystemClass::FissionAnalysis()
{
  fissionFile->cd();

  Long64_t nentries = tree->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  // neutron and photon multiplicities
  int nMult, pMult;
  int nBackMult, pBackMult;

  // detection time
  double timeDet;
  int numDet;
  double engDet;

  double neutVelocity;

  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
    // load tree
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = tree->GetEntry(jentry);   nbytes += nb;

    // allocating the fission info
    f_fisTime = tTime;
    f_fisErg = tDep;
    //f_fisErg = tDep/detectors[numDet].calibration;

    // reset the neutron and photon multiplicities
    nMult = 0;
    pMult = 0;
    nBackMult = 0;
    pBackMult = 0;

    // for(int reset=0; reset<(sizeof(neutronDetTimes)/sizeof(neutronDetTimes[0])); reset++) {
    //   neutronDetTimes[reset] = 0;
    //   neutronLightOut[reset] = 0;
    //   neutronPSD[reset] = 0;
    //   neutronToFErg[reset] = 0;
    //   neutronDet[reset] = 0;
    //   neutronVx[reset] = 0;
    //   neutronVy[reset] = 0;
    //   neutronVz[reset] = 0;
    //   photonDetTimes[reset] = 0;
    //   photonLightOut[reset] = 0;
    //   photonPSD[reset] = 0;
    //   photonDet[reset] = 0;
    //   photonVx[reset] = 0;
    //   photonVy[reset] = 0;
    //   photonVz[reset] = 0;
    //   backNeutronDetTimes[reset] = 0;
    //   backNeutronLightOut[reset] = 0;
    //   backNeutronPSD[reset] = 0;
    //   backNeutronToFErg[reset] = 0;
    //   backNeutronDet[reset] = 0;
    //   backNeutronVx[reset] = 0;
    //   backNeutronVy[reset] = 0;
    //   backNeutronVz[reset] = 0;
    //   backPhotonDetTimes[reset] = 0;
    //   backPhotonLightOut[reset] = 0;
    //   backPhotonPSD[reset] = 0;
    //   backPhotonDet[reset] = 0;
    //   backPhotonVx[reset] = 0;
    //   backPhotonVy[reset] = 0;
    //   backPhotonVz[reset] = 0;
    // }

    for(int j = 0; j < tMult; j++)
    {
      // find the number of the detector
      numDet = isDetector(totChan[j]);

      // detection time corrected for delay
      timeDet = totToF[j] -  detectors[numDet].timeDelay;

      engDet = totDep[j]/detectors[numDet].calibration;

      // cuts for neutrons & lower than something
      if(totPSP[j] < 0 || totPSP[j] > 1)
      {
        continue;
      }

      if(
        (totPSP[j] > detectors[numDet].discPSDPoint->Eval(engDet))
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
        neutVelocity = pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        neutronToFErg[nMult] = (1.0/2.0)*MASS_NEUTRONS*neutVelocity;
        neutronDet[nMult] = numDet;
        neutronVx[nMult] = detectors[numDet].X*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        neutronVy[nMult] = detectors[numDet].Y*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        neutronVz[nMult] = detectors[numDet].Z*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        nMult++;
      }

      // cuts for gammas
      else if(
        (totPSP[j] < detectors[numDet].discPSDPoint->Eval(engDet))  //^^ -0.03 +0.03
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
        photonVx[pMult] = detectors[numDet].X*LIGHT_C;
        photonVy[pMult] = detectors[numDet].Y*LIGHT_C;
        photonVz[pMult] = detectors[numDet].Z*LIGHT_C;
        pMult++;
      }

      // cuts for background neutrons
      else if(
        (totPSP[j] > detectors[numDet].discPSDPoint->Eval(engDet))
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
        neutVelocity = pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        backNeutronToFErg[nBackMult] = (1.0/2.0)*MASS_NEUTRONS*neutVelocity;
        backNeutronDet[nBackMult] = numDet;
        backNeutronVx[nBackMult] = detectors[numDet].X*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        backNeutronVy[nBackMult] = detectors[numDet].Y*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        backNeutronVz[nBackMult] = detectors[numDet].Z*pow(LIGHT_C,2)*pow(detectors[numDet].distance/timeDet,2);
        nBackMult++;
      }

      // cuts for background photons
      else if(
        (totPSP[j] < detectors[numDet].discPSDPoint->Eval(engDet))
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
        backPhotonVx[pBackMult] = detectors[numDet].X*LIGHT_C;
        backPhotonVy[pBackMult] = detectors[numDet].Y*LIGHT_C;
        backPhotonVz[pBackMult] = detectors[numDet].Z*LIGHT_C;
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
  fissionTree->Write();
}


//deltapsd, save covariances,
//comment
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
    int numDet;
    int engDet;

    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
       // load tree
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = tree->GetEntry(jentry);   nbytes += nb;

       // allocating the fission info
       f_fisTime = tTime;
       f_fisErg = tDep;

       // reset the neutron and photon multiplicities
       nMult = 0;
       pMult = 0;
       nBackMult = 0;
       pBackMult = 0;

       for(int j = 0; j < tMult; j++)
       {
         // find the number of the detector
        numDet = isDetector(totChan[j]);

        // detection time corrected for delay
        timeDet = totToF[j] -  detectors[numDet].timeDelay;

        engDet = totDep[j]/detectors[numDet].calibration;

        //cout << endl << detectors[numDet].discPSDPoint->Eval(totDep[j]) << endl;


        // cuts for neutrons
        if(
          (totPSP[j] > detectors[numDet].discPSDPoint->Eval(engDet) + deltapsd)  //+deltapsd
          &
          (timeDet > MINTIMEN)
          &
          (timeDet < MAXTIMEN)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             nMult++;
        }

        // cuts for gammas
        else if(
          (totPSP[j] < detectors[numDet].discPSDPoint->Eval(engDet) + deltapsd)  //^^ -0.03 +0.03
          &
          (timeDet > MINTIMEP)
          &
          (timeDet < MAXTIMEP)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             pMult++;
        }

        // cuts for background neutrons
        else if(
          (totPSP[j] > detectors[numDet].discPSDPoint->Eval(engDet) + deltapsd)
          &
          (timeDet > BACKSHIFT+MINTIMEN)
          &
          (timeDet < BACKSHIFT+MAXTIMEN)
          &
          (engDet > DETECTOR_THRESHOLD)
          )
        {
             nBackMult++;
        }

        // cuts for background photons
        else if(
          (totPSP[j] < detectors[numDet].discPSDPoint->Eval(engDet) + deltapsd)
          &
          (timeDet> BACKSHIFT+MINTIMEP)
          &
          (timeDet< BACKSHIFT+MAXTIMEP)
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
  //fissionTree->Write();
}
