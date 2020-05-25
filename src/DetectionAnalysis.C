/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Perform the analyisis of the collected data on a detector-by-detector basis.
The result of this code will be a series of cuts to clean the data.

So far, we look at:
- PSD characterization
- Timing delays
*/

#define DetectionAnalysis_cxx

#include "DetectorSystemClass.h"

#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

//#include "InfoSystem.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"
//#include "InfoSystemTest.h"

using namespace std;

int DetectorSystemClass::DetectionAnalysis()
{
	/*
	    _____      _
	  / ____|    | |
	 | (___   ___| |_ _   _ _ __
	  \___ \ / _ \ __| | | | '_ \
	  ____) |  __/ |_| |_| | |_) |
	 |_____/ \___|\__|\__,_| .__/
	                       | |
	                       |_|
	*/

	// openFile and create tree
	TString filePrefix = "FissionOutput";
	TString fileInName;

	// define the fitting function for the psd discrimination
	double minPSD_fit = 0;
	double maxPSD_fit = 0.3;
	TF1* fitPSD_n = new TF1("fitPSDn", "[0]*e^(-(x - [1])^2/(2*[2]^2))", minPSD_fit, maxPSD_fit);
	TF1* fitPSD_p = new TF1("fitPSDp", "[0]*e^(-(x - [1])^2/(2*[2]^2))", minPSD_fit, maxPSD_fit);

	// the sum of the two fitting functions
	TF1* fitPSD_np = new TF1("fitPSDnp", "fitPSDn + fitPSDp");
	fitPSD_np->SetParNames("AP", "mP", "sP", "AN", "mN", "sN");
	fitPSD_np->SetParameters(100, 0.12, 0.02, 27, 0.24, 0.04);
	fitPSD_np->SetParLimits(4, 0.15, 0.3);

	// initialize the pointers to the fits
	TFitResultPtr tofDelPFit;
	TFitResultPtr psdFit;

	double psdPhotMean, psdPhotStd, psdNeutMean, psdNeutStd;


	/*
		___     _
	  / __|  _| |_ ___
	 | (_| || |  _(_-<
	  \___\_,_|\__/__/

	*/

	// cuts based on detector
	TCut selectChan;
	TString chanS = "totChan==";

	// cuts based on psp
	float minPSP = 0.0;
	float maxPSP = 1.0;

	// max min cuts
	TString upPSP = "totPSP >= " + to_string(minPSP);
	TString downPSP = "totPSP <= " + to_string(maxPSP);
	TCut cutMinPSP;
	TCut cutMaxPSP;
	cutMinPSP = upPSP;
	cutMaxPSP = downPSP;

	// neutron photon cuts
	TCut neutronCut;
	TCut photonCut;
	TString neutPSP = "totPSP >=";
	TString photPSP = "totPSP <=";

	/*
	  __  __      _        _
	 |  \/  |__ _(_)_ _   | |   ___  ___ _ __
	 | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
	 |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
	                                    |_|
	*/

	detFile->cd();

	int channelDet;

	if (tree == 0) return -1;

	Long64_t nentries = tree->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;

	for (Long64_t jentry=0; jentry<nentries;jentry++)
	{
	 // load tree
	 Long64_t ientry = LoadTree(jentry);
	 if (ientry < 0) break;
	 nb = tree->GetEntry(jentry);   nbytes += nb;

	 for(int part = 0; part < tMult; part++)
	 {
		 // store the channel number
		 channelDet = isDetector(totChan[part]);

		 // fill the appropriate histograms
		 psdhists[channelDet]->Fill(totPSP[part]);
		 erghists[channelDet]->Fill(totDep[part]);
		 psdErgHists[channelDet]->Fill(totDep[part], totPSP[part]);
		 tofDelPhists[channelDet]->Fill(totToF[part]);

	 }
  }

	cout << "Finished looping tree and filled histograms" << endl;

	for(int i = 0; i < numDetectors; i++)
	{
		// find the psp parameter
		psdFit = psdhists[i]->Fit(fitPSD_np, "SQ");

		psdPhotMean = psdFit->Parameter(1);
		psdPhotStd = psdFit->Parameter(2);
		psdNeutMean = psdFit->Parameter(4);
		psdNeutStd = psdFit->Parameter(5);

		// find the appropriate psp parameters here
		cout << psdPhotMean << " " << psdNeutMean << endl;

		// compute the psd discrimination parameter
		detectors[i].discPSD = (psdPhotMean + psdNeutMean)/2;

		cout << "PSD discrimination is: " << detectors[i].discPSD << endl;
	}

	for(int i = 0; i < numDetectors; i++)
	{
		// fit the photon peak and find delay
		tofDelPFit = tofDelPhists[i]->Fit("gaus", "SQ");

		// find the dealy of each detector
		detectors[i].timeDelay = tofDelPFit->Parameter(1);
		detectors[i].timeResolution = tofDelPFit->Parameter(2);

		cout << "Time delay is: " << detectors[i].timeDelay  << endl;
	}

	// write the output histograms
	for(int i = 0; i < numDetectors; i++)
	{
			// save the results
			cdPsd->cd();
			psdhists[i]->Write();
			erghists[i]->Write();
			psdErgHists[i]->Write();

			cdToF->cd();
			tofDelPhists[i]->Write();
			tofNhists[i]->Write();
			tofPhists[i]->Write();

			cdKin->cd();
			kinematicN[i]->Write();
			kinematicP[i]->Write();
	}

	cout << "Histograms have been written to file" << endl;

	cdToF->cd();
	// save the stacked histograms as a root file
	TCanvas* tofCanvas = new TCanvas("ToF", "ToF", 800, 1500);
	tofCanvas->Divide(3,2);
	TString titleCanvas;

	for(int i = 0; i < numDetectors; i++)
	{
		tofCanvas->cd(i+1);

		titleCanvas = "ToF detector " + to_string(listDetectorsChan[i]) + "; Time (ns); Counts";

		THStack *tofStack = new THStack("tofStack", titleCanvas);
		tofStack->Add(tofNhists[i]);
		tofStack->Add(tofPhists[i]);
		tofStack->Draw("nostack");

		TLegend* legendTT = new TLegend(0.1,0.7,0.48,0.9);
		legendTT->AddEntry(tofNhists[i], "Neutrons", "l");
		legendTT->AddEntry(tofPhists[i], "Photons", "l");
		legendTT->Draw();
	}
	tofCanvas->Write();

	//outF->Close();
	return 0;
}
