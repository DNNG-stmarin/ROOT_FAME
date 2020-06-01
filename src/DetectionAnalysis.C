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

#include <TH3.h>
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <TProfile.h>
#include <TGraph.h>
#include <TLine.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

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



	/*
		___     _
	  / __|  _| |_ ___
	 | (_| || |  _(_-<
	  \___\_,_|\__/__/

	*/

	// important make cuts

	// // cuts based on detector
	// TCut selectChan;
	// TString chanS = "totChan==";
	//
	// // cuts based on psp
	// float minPSP = 0.0;
	// float maxPSP = 1.0;
	//
	// // max min cuts
	// TString upPSP = "totPSP >= " + to_string(minPSP);
	// TString downPSP = "totPSP <= " + to_string(maxPSP);
	// TCut cutMinPSP;
	// TCut cutMaxPSP;
	// cutMinPSP = upPSP;
	// cutMaxPSP = downPSP;
	//
	// // neutron photon cuts
	// TCut neutronCut;
	// TCut photonCut;
	// TString neutPSP = "totPSP >=";
	// TString photPSP = "totPSP <=";

	/*
	  __  __      _        _
	 |  \/  |__ _(_)_ _   | |   ___  ___ _ __
	 | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
	 |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
	                                    |_|
	*/

	detFile->cd();

	int channelDet;
	double tempPSD, tempSlice;

	if (tree == 0) return -1;

	Long64_t nentries = tree->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;


	// psd loop
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
		 psdhists[channelDet]->Fill(totPSP[part]); // psd histogram
		 erghists[channelDet]->Fill(totDep[part]); // energy histogram
		 tofDelPhists[channelDet]->Fill(totToF[part]); // tof histograms

		 psdErgHists[channelDet]->Fill(totDep[part], totPSP[part]); // energy-psd histogram
		 tofPsdHists[channelDet]->Fill(totPSP[part], totToF[part]); // psd-tof histograms
		 tofErgHists[channelDet]->Fill(totDep[part], totToF[part]); // energy-tof

		 expHists[channelDet]->Fill(totPSP[part], totDep[part], totToF[part]); // complete point
	 }
  }
	cout << "Finished psd filling loop" << endl;

	/*
	___  _            _       _           _   _
 |   \(_)___ __ _ _(_)_ __ (_)_ _  __ _| |_(_)___ _ _
 | |) | (_-</ _| '_| | '  \| | ' \/ _` |  _| / _ \ ' \
 |___/|_/__/\__|_| |_|_|_|_|_|_||_\__,_|\__|_\___/_||_|

	*/

	int stepSize = 10;
	long int minEntries = 400;

	// initialize the pointers to the fits
	TFitResultPtr psdFit;
	TFitResultPtr tofDelPFit;
	TFitResultPtr psdDiscFit;

	double psdPhotCounts, psdPhotMean, psdPhotStd;
	double psdNeutCounts, psdNeutMean, psdNeutStd;

	double *energySliceInd, *discLinePoint;

	// initialize the discrimination line graphs
	TGraph** discLines;
	discLines = new TGraph* [numDetectors];

	TF1* fitDisc = new TF1("FitDisc", "pol1", minErg_fit, maxErg_fit);
	fitDisc->SetLineStyle(kDashed);
	fitDisc->SetLineColor(kOrange);
	fitDisc->SetLineWidth(4);

	double p0Disc, p1Disc, p2Disc;

	// initialize the tracker of good slices
	int numGoodSlices;
	int energyBins;

	// psd discrimination loop delay over detectors
	for(int det = 0; det < numDetectors; det++)
	{
		cdPsd->cd();
		cout << "For detector " << det << ":" << endl;

		energyBins = psdErgHists[det]->GetNbinsX(); // get the number of slices
		energySliceInd = new double [energyBins]; //energy slice
		discLinePoint = new double [energyBins]; //discrimination
		numGoodSlices = 0; // reset the number of good slices

		// find the profile to get the center
		TProfile* histErgProfile = psdErgHists[det]->ProfileX("profileEnergy");

		cdPsdSLices->cd();
		// loop over the slices
		for(int energySlice = 0; energySlice < energyBins; energySlice += stepSize)
		{

			// define the slice
			TH1D* psdErgSlice = psdErgHists[det]->ProjectionY("psdErgSlice", energySlice, energySlice+stepSize);
			int numEntriesInSlice = psdErgSlice->GetEntries();
			psdErgSlice->SetLineColor(kBlack);

			// close out of the loop if there is not enough info in the slice
			if(numEntriesInSlice < minEntries)
			{
				continue;
			}

			//smooth histograms before fitting
			TH1F* smooth = (TH1F*) psdErgSlice->Clone();
			smooth->SetLineColor(kRed);
			smooth->Smooth(1);

			// find the initial parameters for the fiting of the photon peak
			smooth->GetXaxis()->SetRangeUser(minPSD_fit, divPSD_fit);
			Double_t pMax = smooth->GetMaximum();
			Double_t binpMax = smooth->GetMaximumBin();
			Double_t xpMax = smooth->GetXaxis()->GetBinCenter(binpMax);
			Double_t pMean = smooth->GetMean();
			Double_t pRMS = smooth->GetRMS();

			// find the initial parameters for the fitting of the neutron peak
			smooth->GetXaxis()->SetRangeUser(divPSD_fit, maxPSD_fit);
			Double_t nMax = smooth->GetMaximum();
			Double_t binnMax = smooth->GetMaximumBin();
			Double_t xnMax = smooth->GetXaxis()->GetBinCenter(binnMax);
			Double_t nMean = smooth->GetMean();
			Double_t nRMS = smooth->GetRMS();

			//reset
			smooth->GetXaxis()->SetRangeUser(minPSD_fit, maxPSD_fit);

			// set the initial guesses
			fitPSD->SetParameter(0, pMax);
			fitPSD->SetParameter(1, xpMax);
			fitPSD->SetParameter(2, pRMS);

			fitPSD->SetParameter(3, nMax);
			fitPSD->SetParameter(4, xnMax);
			fitPSD->SetParameter(5, nRMS);

			// find the psp parameter through fit and reset
			psdErgSlice->GetXaxis()->SetRangeUser(minPSD_fit, maxPSD_fit);
			psdFit = psdErgSlice->Fit(fitPSD, "SQ");

			// extract the relevant fit parameters
			psdPhotCounts = psdFit->Parameter(0);
			psdPhotMean = psdFit->Parameter(1);
			psdPhotStd = psdFit->Parameter(2);
			psdNeutCounts = psdFit->Parameter(3);
			psdNeutMean = psdFit->Parameter(4);
			psdNeutStd = psdFit->Parameter(5);

			// cout << "Finished fitting " << endl;
			// cout << "[" << psdPhotCounts << " " << psdPhotMean << " " << psdPhotStd;
			// cout << " " << psdNeutCounts << " " << psdNeutMean << " " << psdNeutStd;
			// cout << "]" << endl;

			// set fitted parameters
			fitPSD->SetParameter(0, psdPhotCounts);
			fitPSD->SetParameter(1, psdPhotMean);
			fitPSD->SetParameter(2, psdPhotStd);

			fitPSD->SetParameter(3, psdNeutCounts);
			fitPSD->SetParameter(4, psdNeutMean);
			fitPSD->SetParameter(5, psdNeutStd);

			// set fitted parameters
			intersection->SetParameter(0, psdPhotCounts);
			intersection->SetParameter(1, psdPhotMean);
			intersection->SetParameter(2, psdPhotStd);

			intersection->SetParameter(3, psdNeutCounts);
			intersection->SetParameter(4, psdNeutMean);
			intersection->SetParameter(5, psdNeutStd);

			// set fitted parameters to the individual parameters
			fitPSD_p->SetParameter(0, psdPhotCounts);
			fitPSD_p->SetParameter(1, psdPhotMean);
			fitPSD_p->SetParameter(2, psdPhotStd);

			fitPSD_n->SetParameter(0, psdNeutCounts);
			fitPSD_n->SetParameter(1, psdNeutMean);
			fitPSD_n->SetParameter(2, psdNeutStd);

			// see if we can find the interesect
			if((psdNeutMean < psdPhotMean) &
			   (intersection->Eval(psdNeutMean) > 0) &
			   (intersection->Eval(psdPhotMean) < 0) )
			{
			  continue;
			}
			// is is valid
			else
			{
				numGoodSlices++;
			}

			// find the optimal psd line
			tempPSD = intersection->GetX(0, psdPhotMean, psdNeutMean);
			tempSlice = (histErgProfile->GetBinCenter(energySlice) + histErgProfile->GetBinCenter(energySlice + stepSize))/2.0;

			// find the points for the graph
			energySliceInd[numGoodSlices] = tempSlice;
			discLinePoint[numGoodSlices] = tempPSD;

			TString canSliceName = "psd" + to_string(det) +  "Proj" + to_string(numGoodSlices);

			// canvas with the psd
			TCanvas* canvasSlice = new TCanvas(canSliceName, canSliceName, 800, 500);

			psdErgSlice->GetYaxis()->SetRangeUser(-1.5*psdPhotCounts, 1.5*psdPhotCounts);

			canvasSlice->cd();

			psdErgSlice->Draw();
			fitPSD->Draw("SAME");
			//fitPSD_n->Draw("SAME");
			//fitPSD_p->Draw("SAME");
			intersection->Draw("SAME");

			// discrimination line
			TLine* line = new TLine(tempPSD, 0, tempPSD, pMax);
			line->SetLineColor(kBlack);
			line->Draw("SAME");

			canvasSlice->Write();
		}

		cdPsd->cd();

		cout << "Finished looping through slices" << endl;

		// create a canvas with the psd discrimination
		TString canDiscErgName = "psdErg" + to_string(det);
		TCanvas* canvasDiscErg = new TCanvas(canDiscErgName, canDiscErgName, 800, 500);

		// create a new psd discrimination line
	  discLines[det] = new TGraph(numGoodSlices, energySliceInd, discLinePoint);
		discLines[det]->SetLineColor(kRed);
		discLines[det]->SetLineWidth(3);

		// fit the psd discriminations
		psdDiscFit = discLines[det]->Fit(fitDisc, "SQ", "", 0.2, 1.2); // change the bounds here

		// extract fit information
		p0Disc = psdDiscFit->Parameter(0);
		p1Disc = psdDiscFit->Parameter(1);
		//p2Disc = psdDiscFit->Parameter(2);

		// set discrimination line
		fitDisc->SetParameters(p0Disc, p1Disc, p2Disc);
		fitDisc->SetParameters(p0Disc, p1Disc);

		TString nameDisc = "discDet" + to_string(det);
		detectors[det].discPSD = new TF1(nameDisc, "FitDisc", minErg_fit, maxErg_fit);
		detectors[det].discPSD->SetParameters(p0Disc, p1Disc, p2Disc);

		// draw the line on top of the histogram
		canvasDiscErg->cd();
		psdErgHists[det]->Draw();
		discLines[det]->Draw("SAME");
		fitDisc->Draw("SAME");
		canvasDiscErg->Write();
		
	}


	/*
	_____ _       _
 |_   _(_)_ __ (_)_ _  __ _
   | | | | '  \| | ' \/ _` |
   |_| |_|_|_|_|_|_||_\__, |
                      |___/
	*/

	for(int i = 0; i < numDetectors; i++)
	{
		// fit the photon peak and find delay
		int ctMax = tofDelPhists[i]->GetMaximum();
		int binMax = tofDelPhists[i]->GetMaximumBin();
		double timePeak = tofDelPhists[i]->GetBinCenter(binMax);
		cout << "Peak location at: " << timePeak << endl;

		TF1* gausFitTime = new TF1("peakFit", "[0]*e^(-(x - [1])^2/(2*[2]^2))", timePeak - 10, timePeak + 10);
		gausFitTime->SetParameter(0, ctMax);
		gausFitTime->SetParameter(1, timePeak);
		gausFitTime->SetParameter(2, 1);

		tofDelPFit = tofDelPhists[i]->Fit(gausFitTime, "SQ");

		// find the dealy of each detector
		detectors[i].timeDelay = tofDelPFit->Parameter(1);
		detectors[i].timeResolution = tofDelPFit->Parameter(2);

		cout << "Time delay is: " << detectors[i].timeDelay  << endl;
	}


	// time of flight loop
	double corrTime;
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
		 corrTime = totToF[part] - detectors[channelDet].timeDelay;

		 // discriminate particles here (make it better)
		 if(totPSP[part] < detectors[channelDet].discPSD->Eval(totDep[part]) )
		 {
			 tofPhists[channelDet]->Fill(corrTime);
			 kinematicP[channelDet]->Fill(corrTime, totDep[part]);
		 }

		 if(totPSP[part] > detectors[channelDet].discPSD->Eval(totDep[part]) )
		 {
			 tofNhists[channelDet]->Fill(corrTime);
			 kinematicN[channelDet]->Fill(corrTime, totDep[part]);
		 }

	 }
	}



	// write the output histograms
	for(int i = 0; i < numDetectors; i++)
	{
			// save the results
			cdPsd->cd();
			psdhists[i]->Write();
			erghists[i]->Write();
			psdErgHists[i]->Write();
			tofPsdHists[i]->Write();
			tofErgHists[i]->Write();
			expHists[i]->Write();

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
