/*
Author: Stefano Marin
Purpose: Initialize all the histograms that are used for the analysis of the
         detectors histogram
Date: Ann Arbor, May 14th 2020
*/

#include "DetectorSystemClass.h"

void DetectorSystemClass::InitializeDetectorHistograms()
{
  cout << endl;


  // detector histograms


  // important axes to have
  TString timeAxis = "time (ns)";
  TString energyAxis = "energy (MeVee)";
  TString countAxis = "counts";

  /*
    __  __      _ _   _      _ _    _ _
   |  \/  |_  _| | |_(_)_ __| (_)__(_) |_ _  _
   | |\/| | || | |  _| | '_ \ | / _| |  _| || |
   |_|  |_|\_,_|_|\__|_| .__/_|_\__|_|\__|\_, |
                       |_|                |__/
  */

  // multiplicity histogram
  hMult = new TH2I("mult", "mult", 10, 0, 10, 10, 0, 10);

  // bicorrelation histogram
  TString bicorrT = "bicorr_det_";
  TString bicorrHistName, bicorrHistTitle;

  for(int det1 = 0; det1 < NUM_DETS; det1++)
  {
     for(int det2 = 0; det2 < NUM_DETS; det2++)
     {
        bicorrHistName = bicorrT + to_string(det1) + "_" + to_string(det2);
        bicorrHistTitle = bicorrHistName + ";" + timeAxis + ";" + timeAxis;

        hBicorr[det1][det2] = new TH2F(bicorrHistName, bicorrHistTitle, 100, -10, 100, 100, -10, 100);
        // hBicorr[det1][det2]->SetOption("COLZ");
     }
  }

  /*
    ___ _                    _      _   _
   | _ |_)__ ___ _ _ _ _ ___| |__ _| |_(_)___ _ _
   | _ \ / _/ _ \ '_| '_/ -_) / _` |  _| / _ \ ' \
   |___/_\__\___/_| |_| \___|_\__,_|\__|_\___/_||_|

  */
  cout << "Bicorrelation histograms have been created" << endl;

  // singles and doubles histograms
  hSingles = new TH1I("singles", "singles; detector; counts", NUM_DETS + 1, 0, NUM_DETS);
  hDoubles = new TH2I("doubles", "doubles; detector; detector", NUM_DETS + 1, 0, NUM_DETS, NUM_DETS + 1, 0, NUM_DETS);

  cout << "Detection histograms have been created" << endl;

  /*
           _         _    _
    __ ___(_)_ _  __(_)__| |___ _ _  __ ___ ___
   / _/ _ \ | ' \/ _| / _` / -_) ' \/ _/ -_|_-<
   \__\___/_|_||_\__|_\__,_\___|_||_\__\___/__/

  */

  // create titles
  TString nnCoincT = "nn_det_";
  TString ngCoincT = "ng_det_";
  TString gnCoincT = "gn_det_";
  TString ggCoincT = "gg_det_";
  TString allCoincT = "all_det_";

  double minTimeCrossCorr, maxTimeCrossCorr, numBinsCrossCorr;
  minTimeCrossCorr = -50;
  maxTimeCrossCorr = +50;
  numBinsCrossCorr = 800;


  TString nnHistName, ngHistName, gnHistName, ggHistName, allCoincName;
  TString nnHistTitle, ngHistTitle, gnHistTitle, ggHistTitle, allCoincTitle;

  // populate histograms with pointers
  for(int det1 = 0; det1 < NUM_DETS; det1++)
  {
     for(int det2 = 0; det2 < NUM_DETS; det2++)
     {
        allCoincName = allCoincT + to_string(det1) + "_" + to_string(det2);
        allCoincTitle = allCoincName + ";" + timeAxis + ";" + countAxis;
        allCoinc[det1][det2] = new THStack(allCoincName, "");

        nnHistName = nnCoincT + to_string(det1) + "_" + to_string(det2);
        nnHistTitle = nnHistName + ";" + timeAxis + ";" + countAxis;
        nnMult[det1][det2] = new TH1F(nnHistName, nnHistTitle, numBinsCrossCorr, minTimeCrossCorr, maxTimeCrossCorr);
        nnMult[det1][det2]->SetLineColor(kBlue);

        allCoinc[det1][det2]->Add(nnMult[det1][det2]);

        ngHistName = ngCoincT + to_string(det1) + "_" + to_string(det2);
        ngHistTitle = ngHistName + ";" + timeAxis + ";" + countAxis;
        ngMult[det1][det2] = new TH1F(ngHistName, ngHistTitle, numBinsCrossCorr, minTimeCrossCorr, maxTimeCrossCorr);
        ngMult[det1][det2]->SetLineColor(kOrange);

        allCoinc[det1][det2]->Add(ngMult[det1][det2]);

        gnHistName = gnCoincT + to_string(det1) + "_" + to_string(det2);
        gnHistTitle = gnHistName + ";" + timeAxis + ";" + countAxis;
        gnMult[det1][det2] = new TH1F(gnHistName, gnHistTitle, numBinsCrossCorr, minTimeCrossCorr, maxTimeCrossCorr);
        gnMult[det1][det2]->SetLineColor(kOrange);

        allCoinc[det1][det2]->Add(gnMult[det1][det2]);

        ggHistName = ggCoincT + to_string(det1) + "_" + to_string(det2);
        ggHistTitle = ggHistName + ";" + timeAxis + ";" + countAxis;
        ggMult[det1][det2] = new TH1F(ggHistName, ggHistTitle, numBinsCrossCorr, minTimeCrossCorr, maxTimeCrossCorr);
        ggMult[det1][det2]->SetLineColor(kRed);

        allCoinc[det1][det2]->Add(ggMult[det1][det2]);
        allCoinc[det1][det2]->SetDrawOption("nostack");

     }
  }

  cout << "Cross-correlations histograms have been created" << endl;

  /*
    ___      __ _        _   _
   | _ \___ / _| |___ __| |_(_)___ _ _  ___
   |   / -_)  _| / -_) _|  _| / _ \ ' \(_-<
   |_|_\___|_| |_\___\__|\__|_\___/_||_/__/
  */

  // create the reflections
  TString refT = "ref_det_";
  TString refHistName, refHistTitle;

  for(int det1 = 0; det1 < NUM_DETS; det1++)
  {
     for(int det2 = 0; det2 < NUM_DETS; det2++)
     {
        refHistName = refT + to_string(det1) + "_" + to_string(det2);
        refHistTitle = refHistName + ";" + timeAxis + ";" + energyAxis + ";" + countAxis;
        reflections[det1][det2] = new TH2F(refHistName, refHistTitle, 100, -50, 50, 10000, 0, 10);
     }
  }

  cout << "Reflection histograms have been created" << endl;

  cout << "All histograms have been initialized" << endl;
}
