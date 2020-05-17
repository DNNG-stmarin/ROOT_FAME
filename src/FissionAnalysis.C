/*
Author: Stefano Marin
Purpose: Loop through the entire the coincidence tree and create the fission
         tree.
Date: May 14th, Ann Arbor
*/

#include "DetectorSystemClass.h"
#include "mappingFunctions.h"

void DetectorSystemClass::FissionAnalysis()
{
  fissionFile->cd();

  Long64_t nentries = tree->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  // neutron and photon multiplicities
  int nMult, pMult;

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

     for(int j = 0; j < tMult; j++)
     {
       // find the number of the detector
      numDet = isDetector(totChan[j]);

      // detection time corrected for delay
      timeDet = totToF[j] -  detectors[numDet].timeDelay;

      if(
        (totPSP[j] > detectors[numDet].discPSD)
        &
        (totToF[j] > -5)
        &
        (totDep[j] > 0.1)
        )
      {
           nMult++;
      }

      else if(
        (totPSP[j] < detectors[numDet].discPSD)
        &
        (totToF[j] > -5)
        &
        (totDep[j] > 0.1)
        )
      {
           pMult++;
      }

     }

     // set branches of final tree
     f_neutronMult = nMult;
     f_gammaMult = pMult;

     fissionTree->Fill();
  }

  fissionTree->Write();
}
