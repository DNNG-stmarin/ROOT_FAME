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

  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
     // load tree
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = tree->GetEntry(jentry);   nbytes += nb;

     // allocating the fission info
     f_fisTime = tTime;
     //f_fisErg = tDep;
     f_fisErg = tDep/detectors[numDet].calibration;

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

      // cuts for neutrons
      if(
        (totPSP[j] > detectors[numDet].discPSDPoint->Eval(totDep[j])) //didn't change totdep here because it doesnt matter since discpsdpoitn is constant
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
        (totPSP[j] < detectors[numDet].discPSDPoint->Eval(totDep[j]))  //^^ -0.03 +0.03
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
        (totPSP[j] > detectors[numDet].discPSDPoint->Eval(totDep[j]))
        &
        (timeDet > MIN_TIME_N - BACKGROUND_SHIFT)
        &
        (timeDet < MAX_TIME_N - BACKGROUND_SHIFT)
        &
        (engDet > DETECTOR_THRESHOLD)
        )
      {
           nBackMult++;
      }

      // cuts for background photons
      else if(
        (totPSP[j] < detectors[numDet].discPSDPoint->Eval(totDep[j]))
        &
        (timeDet > MIN_TIME_P - BACKGROUND_SHIFT)
        &
        (timeDet < MAX_TIME_P - BACKGROUND_SHIFT)
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

        //cout << endl << detectors[numDet].discPSDPoint->Eval(totDep[j]) << endl;


        // cuts for neutrons
        if(
          (totPSP[j] > detectors[numDet].discPSDPoint->Eval(totDep[j]) + deltapsd)  //+deltapsd
          &
          (timeDet > MINTIMEN)
          &
          (timeDet < MAXTIMEN)
          &
          (totDep[j] > DETECTOR_THRESHOLD)
          )
        {
             nMult++;
        }

        // cuts for gammas
        else if(
          (totPSP[j] < detectors[numDet].discPSDPoint->Eval(totDep[j]) + deltapsd)  //^^ -0.03 +0.03
          &
          (timeDet > MINTIMEP)
          &
          (timeDet < MAXTIMEP)
          &
          (totDep[j] > DETECTOR_THRESHOLD)
          )
        {
             pMult++;
        }

        // cuts for background neutrons
        else if(
          (totPSP[j] > detectors[numDet].discPSDPoint->Eval(totDep[j]) + deltapsd)
          &
          (timeDet > BACKSHIFT+MINTIMEN)
          &
          (timeDet < BACKSHIFT+MAXTIMEN)
          &
          (totDep[j] > DETECTOR_THRESHOLD)
          )
        {
             nBackMult++;
        }

        // cuts for background photons
        else if(
          (totPSP[j] < detectors[numDet].discPSDPoint->Eval(totDep[j]) + deltapsd)
          &
          (timeDet> BACKSHIFT+MINTIMEP)
          &
          (timeDet< BACKSHIFT+MAXTIMEP)
          &
          (totDep[j] > DETECTOR_THRESHOLD)
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
