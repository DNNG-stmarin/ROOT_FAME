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

#include "InfoSystem.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"

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
	fitPSD_np->SetParameters(100, 0.12, 0.02, 27, 0.28, 0.04);
	fitPSD_np->SetParLimits(4, 0.2, 0.3);

	expFile->cd();


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
	  _  _ _    _                              _  _
	 | || (_)__| |_ ___  __ _ _ _ __ _ _ __   | \| |__ _ _ __  ___ ___
	 | __ | (_-<  _/ _ \/ _` | '_/ _` | '  \  | .` / _` | '  \/ -_|_-<
	 |_||_|_/__/\__\___/\__, |_| \__,_|_|_|_| |_|\_\__,_|_|_|_\___/__/
	                    |___/
	*/

	// name generators
	TString numDet;

	TString psdName = "PSD";
	TString ergName = "Erg";
	TString psdErgName = "PSD_Erg";
	TString tofName = "TOF";
	TString kinematicName = "Kin";

	TString neutronName = "n";
	TString photonName = "p";

	// initialize the strings containing the names of the detectors
	TString psdHistNameT, ergHistNameT, psdErgHistNameT;
	TString tofNHistNameT;
	TString tofPHistNameT;

	// name of kinematic
	TString kinematicNHistNameT, kinematicPHistNameT;

	// initialize the pointers to the fits
	TFitResultPtr tofDelPFit;
	TFitResultPtr psdFit;

	double psdPhotMean, psdPhotStd, psdNeutMean, psdNeutStd;

	/*
	  ___             _ _
	 | _ \___ ____  _| | |_ ___
	 |   / -_|_-< || | |  _(_-<
	 |_|_\___/__/\_,_|_|\__/__/
	*/

	// psp parameters
	double psd_disc[NUM_DETS] = {0};

	// time parameters
	double time_delay[NUM_DETS] = {0};
	double time_sigma[NUM_DETS] = {0};

	// psd histograms
	TH1F *psdhists[NUM_DETS];
	TH1F *erghists[NUM_DETS];
	TH2F *psdErgHists[NUM_DETS];

	// tof histograms
	TH1F *tofDelPhists[NUM_DETS];
	TH1F *tofNhists[NUM_DETS];
	TH1F *tofPhists[NUM_DETS];

	// kinematic histograms
	TH2F *kinematicN[NUM_DETS];
	TH2F *kinematicP[NUM_DETS];


	/*
	  __  __      _        _
	 |  \/  |__ _(_)_ _   | |   ___  ___ _ __
	 | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
	 |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
	                                    |_|
	*/

	cout << endl;
	for(int i=0; i<NUM_DETS; i++)
	{

		// find the string name of the detector
		numDet = to_string(DETECTORS[i]);

		cout << "Now analyzing detector at channel " << numDet << endl;
		// psd histograms
		psdHistNameT = psdName + numDet;
		psdErgHistNameT = psdErgName + numDet;
		ergHistNameT = ergName + numDet;

		// time of flight histograms
		tofNHistNameT = tofName + neutronName +  numDet;
		tofPHistNameT = tofName + photonName + numDet;

		// kinematic histograms
		kinematicNHistNameT = kinematicName + neutronName + numDet;
		kinematicPHistNameT = kinematicName + photonName + numDet;

		//create histograms to fill
		psdhists[i] = new TH1F(psdHistNameT, psdHistNameT, 200, 0, 1);
		psdErgHists[i] = new TH2F(psdErgHistNameT, psdErgHistNameT, 10000, 0, 10, 500, 0, 1);

		// time of flight histograms, before and after delays
		tofDelPhists[i] = new TH1F(tofPHistNameT + "del", tofPHistNameT + "del", 2*(int)COINC_WINDOW, -COINC_WINDOW, +COINC_WINDOW);

		tofNhists[i] = new TH1F(tofNHistNameT, tofNHistNameT, 2*(int)COINC_WINDOW, -COINC_WINDOW, +COINC_WINDOW);
		tofPhists[i] = new TH1F(tofPHistNameT, tofPHistNameT, 2*(int)COINC_WINDOW, -COINC_WINDOW, +COINC_WINDOW);

		kinematicN[i] = new TH2F(kinematicNHistNameT, kinematicNHistNameT + ";Time (ns); Integral (MeVee); Counts", 2*(int)COINC_WINDOW, -COINC_WINDOW, +COINC_WINDOW, 10000, 0, 10);
		kinematicP[i] = new TH2F(kinematicPHistNameT, kinematicPHistNameT + ";Time (ns); Integral (MeVee); Counts", 2*(int)COINC_WINDOW, -COINC_WINDOW, +COINC_WINDOW, 10000, 0, 10);
		kinematicN[i]->SetOption("COLZ");

		// find the appropriate cuts
		selectChan = chanS + numDet;

		// draw histogram
		tree->Draw("totPSP>>" + psdHistNameT, selectChan, "EGOFF");
		tree->Draw("totPSP:totDep >>" + psdErgHistNameT, upPSP&&selectChan, "EGOFF");
		psdErgHists[i]->SetOption("COLZ");

		// find the psp parameter
		psdFit = psdhists[i]->Fit(fitPSD_np, "SQ");

		psdPhotMean = psdFit->Parameter(1);
		psdPhotStd = psdFit->Parameter(2);
		psdNeutMean = psdFit->Parameter(4);
		psdNeutStd = psdFit->Parameter(5);

		// find the appropriate psp parameters here
		cout << psdPhotMean << " " << psdNeutMean << endl;
		psd_disc[i] = (psdPhotMean + psdNeutMean)/2;

		detectors[i].discPSD = psd_disc[i];

		cout << "PSD discrimination is: " << psd_disc[i] << endl;

		// ok now we have found the appropriate PSP, find the strictest photon cut

		photonCut = photPSP + to_string(psd_disc[i] - 0.05); // change this to number of sigmas

		// fill the tof histograms
		tree->Draw("totToF>>" + tofPHistNameT + "del", selectChan&&photonCut, "EGOFF");

		// fit the photon peak and find delay
		tofDelPFit = tofDelPhists[i]->Fit("gaus", "SQ");

		// find the dealy of each detector
		time_delay[i] = tofDelPFit->Parameter(1);
		time_sigma[i] = tofDelPFit->Parameter(2);

		cout << "Time delay is: " << time_delay[i] << endl;

		// makes the new cuts
		neutronCut = neutPSP + to_string(psd_disc[i]);
		photonCut = photPSP + to_string(psd_disc[i]);


		TString correctedTimes = "totToF - "  + to_string(time_delay[i]);

		// fill the neutron ToF histograms
		tree->Draw(correctedTimes + ">>" + tofNHistNameT, selectChan&&neutronCut, "EGOFF");
		tree->Draw(correctedTimes + ">>" + tofPHistNameT, selectChan&&photonCut, "EGOFF");

		tofNhists[i]->SetLineColor(kBlue);
		tofPhists[i]->SetLineColor(kRed);

		// fill the neutron ToF histograms
		tree->Draw("totDep:" + correctedTimes + ">>" + kinematicNHistNameT, selectChan&&neutronCut, "EGOFF");
		tree->Draw("totDep:" + correctedTimes + ">>" + kinematicPHistNameT, selectChan&&photonCut, "EGOFF");


		// save the results
		psdhists[i]->Write();
		psdErgHists[i]->Write();
		tofNhists[i]->Write();
		tofPhists[i]->Write();
		kinematicN[i]->Write();
		kinematicP[i]->Write();
	}

	// save the stacked histograms as a root file
	TCanvas* tofCanvas = new TCanvas("ToF", "ToF", 800, 1500);
	tofCanvas->Divide(3,2);
	TString titleCanvas;

	for(int i=0; i<NUM_DETS; i++)
	{
		tofCanvas->cd(i+1);

		titleCanvas = "ToF detector " + to_string(DETECTORS[i]) + "; Time (ns); Counts";

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
