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
	double tracktof[numDetectors];
	double trackpsd[numDetectors];

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

	int stepSizepsd = 20;
	int stepSizetof = 20;
	long int minEntries = 150;
	long int minEntriestof = 200;
	// initialize the pointers to the fits
	TFitResultPtr psdFit;
	TFitResultPtr tofDelPFit;
	TFitResultPtr psdDiscFit;
	TFitResultPtr psdDiscFittof;
	double psdPhotCounts, psdPhotMean, psdPhotStd;
	double psdNeutCounts, psdNeutMean, psdNeutStd;
	double *energySliceInd, *discLinePoint;
	double *energySliceIndtof, *discLinePointtof;

	// initialize the discrimination line graphs
	TGraph** discLines;
	TGraph** discLinestof;
	discLines = new TGraph* [numDetectors];
	discLinestof = new TGraph* [numDetectors];

	TF1* fitDisc = new TF1("FitDisc", "pol1", minErg_fit, maxErg_fit);
	fitDisc->SetLineStyle(kDashed);
	fitDisc->SetLineColor(kOrange);
	fitDisc->SetLineWidth(4);
	TF1* fitDisctof = new TF1("FitDisctof", "pol1", minErg_fit, maxErg_fit);
	fitDisctof->SetLineStyle(kDashed);
	fitDisctof->SetLineColor(kOrange);
	fitDisctof->SetLineWidth(4);
	double p0Disc, p1Disc, p2Disc;
	// initialize the tracker of good slices
	int numGoodSlices;
	int energyBins;


	// psd slice discrimination
	for(int det = 0; det < numDetectors; det++)
	{
		cdPsd->cd();
		cout << "\nFor detector " << det << ":" << endl;

		energyBins = psdErgHists[det]->GetNbinsX(); // get the number of slices
		energySliceInd = new double [energyBins]; //energy slice
		discLinePoint = new double [energyBins]; //discrimination
		numGoodSlices = -1; // reset the number of good slices

		// find the profile to get the center
		TProfile* histErgProfile = psdErgHists[det]->ProfileX("profileEnergy");

		cdPsdSlices->cd();
		// loop over the slices
		for(int energySlice = 0; energySlice < energyBins; energySlice += stepSizepsd)
		{

			// define the slice
			TH1D* psdErgSlice = psdErgHists[det]->ProjectionY("psdErgSlice", energySlice, energySlice+stepSizepsd);
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

			// find the optimal psd line
			tempPSD = intersection->GetMinimumX(psdPhotMean, psdNeutMean);
			tempSlice = (histErgProfile->GetBinCenter(energySlice) + histErgProfile->GetBinCenter(energySlice + stepSizepsd))/2.0;
			cout << psdPhotMean << "    " << tempPSD << "    " << psdNeutMean;

			if(tempPSD < psdPhotMean || tempPSD > psdNeutMean || tempPSD >= 1) {
				cout << "     throw" << endl;
				continue;
			}
			// else if (tempPSD < psdPhotMean + psdPhotStd/2 || tempPSD > psdNeutMean - psdNeutStd/2) {
			// 	cout << "     throw" << endl;
			// 	continue;
			// }
			// else if(tempPSD-psdPhotMean < 0.005 || psdNeutMean-tempPSD < 0.005) {
			// 	cout << "     throw" << endl;
			// 	continue;
			// }
			else {
				numGoodSlices++;
				cout << endl;
			}
			// find the points for the graph
			energySliceInd[numGoodSlices] = tempSlice;
			discLinePoint[numGoodSlices] = tempPSD;

			TString canSliceName = "psd" + to_string(det) +  "Proj" + to_string(numGoodSlices);

			// canvas with the psd
			TCanvas* canvasSlice = new TCanvas(canSliceName, canSliceName, 800, 500);

			psdErgSlice->GetYaxis()->SetRangeUser(0, 1.5*psdPhotCounts);

			canvasSlice->cd();

			psdErgSlice->Draw();
			fitPSD->Draw("SAME");
			intersection->Draw("SAME");
			fitPSD_p->SetLineColor(kYellow);
			fitPSD_n->SetLineColor(kOrange);
			fitPSD_p->Draw("SAME");
			fitPSD_n->Draw("SAME");

			// discrimination line
			TLine* line = new TLine(tempPSD, 0, tempPSD, 1.5*psdPhotCounts);
			line->SetLineColor(kBlack);
			line->Draw("SAME");

			canvasSlice->Write();
		}
		cdPsd->cd();

		cout << "Finished looping through slices" << endl;

		cout << "all disc: ";
		for(int i=0; i<numGoodSlices+1; i++) {
			cout << discLinePoint[i] << " ";
		}
		cout << endl << endl;

		// create a canvas with the psd discrimination
		TString canDiscErgName = "psdErg" + to_string(det);
		TCanvas* canvasDiscErg = new TCanvas(canDiscErgName, canDiscErgName, 800, 500);

		// create a new psd discrimination line
	  discLines[det] = new TGraph(numGoodSlices+1, energySliceInd, discLinePoint);
		discLines[det]->SetLineColor(kRed);
		discLines[det]->SetLineWidth(3);

		// fit the psd discriminations
		psdDiscFit = discLines[det]->Fit(fitDisc, "SQ", 0, 0.4); // change the bounds here

		// extract fit information
		p0Disc = psdDiscFit->Parameter(0);
		p1Disc = psdDiscFit->Parameter(1);
		//p2Disc = psdDiscFit->Parameter(2);

		// set discrimination line
		fitDisc->SetParameters(p0Disc, p1Disc, p2Disc);
		fitDisc->SetParameters(p0Disc, p1Disc);	//whats the purpose of this?

		TString nameDisc = "discDet" + to_string(det);
		detectors[det].discPSD = new TF1(nameDisc, "FitDisc", minErg_fit, maxErg_fit);
		detectors[det].discPSD->SetParameters(p0Disc, p1Disc, p2Disc);

		// draw the line on top of the histogram
		canvasDiscErg->cd();
		psdErgHists[det]->Draw();
		discLines[det]->Draw("SAME");
		fitDisc->Draw("SAME");
		cdPsdErg->cd();
		canvasDiscErg->Write();
	}

	/*
	_____ _       _
 |_   _(_)_ __ (_)_ _  __ _
   | | | | '  \| | ' \/ _` |
   |_| |_|_|_|_|_|_||_\__, |
                      |___/
	*/

	cdToF->cd();

	//individual discrimination for each detector loop
	for(int i=0; i<numDetectors; i++)
	{
		TString tofpsdname = "TOF_PSD" + to_string(i);
		TCanvas* tofpsdcanvas = new TCanvas(tofpsdname, tofpsdname, 800, 500);

		int totbin = tofDelPhists[i]->GetNbinsX();
		int binlim = tofDelPhists[i]->FindLastBinAbove(0);
		int binmin = tofDelPhists[i]->FindFirstBinAbove(0);
		double xlim = tofDelPhists[i]->GetXaxis()->GetBinCenter(binlim);
		double xmin = tofDelPhists[i]->GetXaxis()->GetBinCenter(binmin);

		TF1* gausFitTime = new TF1("peakFit", "[0]*e^(-(x - [1])^2/(2*[2]^2))", xmin, xlim);
		TF1* neutFitTime = new TF1("npeakFit", "[0]/(1 + ((x - [1])/([2]))^2)", xmin, xlim);
		TF1* TOFintersection = new TF1("TOFintersect", "abs(npeakFit - peakFit)", xmin, xlim);


		//initial parameters photon peak
		int ctMax = tofDelPhists[i]->GetMaximum();
		int binMax = tofDelPhists[i]->GetMaximumBin();
		double timePeak = tofDelPhists[i]->GetBinCenter(binMax);
		cout << "Detector " << i << endl;
		cout << "	Peak location at: " << timePeak << endl;

		//fit and optimize photon peak
		gausFitTime->SetParameter(0, ctMax);
		gausFitTime->SetParameter(1, timePeak);
		gausFitTime->SetParameter(2, 1);
		tofDelPFit = tofDelPhists[i]->Fit(gausFitTime, "SQ");
		gausFitTime->SetParameter(0, tofDelPFit->Parameter(0));
		gausFitTime->SetParameter(1, tofDelPFit->Parameter(1));
		gausFitTime->SetParameter(2, tofDelPFit->Parameter(2));

		tofDelPhists[i]->GetXaxis()->SetRangeUser(timePeak+10, xlim);

		//initial parameters neutron peak
		double neuMax = tofDelPhists[i]->GetMaximum();
		double nbinMax = tofDelPhists[i]->GetMaximumBin();
		double ntimePeak = tofDelPhists[i]->GetBinCenter(nbinMax);

		//fit and optimize neutron peak
		neutFitTime->SetParameter(0, neuMax);
		neutFitTime->SetParameter(1, ntimePeak);
		neutFitTime->SetParameter(2, tofDelPhists[i]->GetRMS());
		TFitResultPtr tofDelNFit = tofDelPhists[i]->Fit(neutFitTime, "+SQ");
		neutFitTime->SetParameter(0, tofDelNFit->Parameter(0));
		neutFitTime->SetParameter(1, tofDelNFit->Parameter(1));
		neutFitTime->SetParameter(2, tofDelNFit->Parameter(2));

		tofDelPhists[i]->GetXaxis()->SetRangeUser(xmin, xlim);

		//intersection
		TOFintersection->SetParameters(gausFitTime->GetParameter(0), gausFitTime->GetParameter(1), gausFitTime->GetParameter(2), neutFitTime->GetParameter(0), neutFitTime->GetParameter(1), neutFitTime->GetParameter(2));
		TOFintersection->SetLineColor(kGreen);

		double toftempPSD = TOFintersection->GetMinimumX(tofDelPhists[i]->GetXaxis()->GetBinCenter(binMax), tofDelPhists[i]->GetXaxis()->GetBinCenter(nbinMax));
		cout << tofDelPhists[i]->GetXaxis()->GetBinCenter(binMax) << " " << toftempPSD << " " << tofDelPhists[i]->GetXaxis()->GetBinCenter(nbinMax) << endl;

		TString canDiscTOFName = "ToFDiscrimination" + to_string(i);
		TCanvas* canvasDiscTOF = new TCanvas(canDiscTOFName, canDiscTOFName, 800, 500);

		TLine* TOFline = new TLine(toftempPSD, 0, toftempPSD, ctMax);
		TOFline->SetLineColor(kBlack);
		TLine* TOFPSDlinetof = new TLine(0, toftempPSD, 1, toftempPSD);
		TOFPSDlinetof->SetLineColor(kRed);

		canvasDiscTOF->cd();
		tofDelPhists[i]->Draw();
		TOFline->Draw("SAME");
		gausFitTime->Draw("SAME");
		neutFitTime->SetLineColor(kOrange);
		neutFitTime->Draw("SAME");
		TOFintersection->Draw("SAME");
		cdTofIndividual->cd();
		canvasDiscTOF->Write();

		// find the delay of photon peak for each detector
		detectors[i].timeDelay = tofDelPFit->Parameter(1);
		detectors[i].timeResolution = tofDelPFit->Parameter(2);

		cout << "	Time delay is: " << detectors[i].timeDelay  << endl << endl;


		/////////////////////////individual psd ////////////////////////////////////////////////////////

		TF1* psdphotpeak = new TF1("psdphotpeak", "[0]*e^(-(x - [1])^2/(2*[2]^2))");
		TF1* psdneutpeak = new TF1("psdneutpeak", "[0]/(1 + ((x - [1])/([2]))^2)");
		TF1* psdcombined = new TF1("psdcombined", "psdphotpeak + psdneutpeak");
		TF1* psdintersection = new TF1("psdintersection", "abs(psdneutpeak - psdphotpeak)");
		TFitResultPtr optimized;

		TH1* smoothpsd = (TH1F*) psdhists[i]->Clone();
		smoothpsd->SetLineColor(kRed);
		smoothpsd->Smooth(1);

		smoothpsd->GetXaxis()->SetRangeUser(minPSD_fit, divPSD_fit);
		Double_t psdmaxP = smoothpsd->GetMaximum();
		Double_t binpsdmaxP = smoothpsd->GetMaximumBin();
		Double_t xpsdmaxP = smoothpsd->GetXaxis()->GetBinCenter(binpsdmaxP);
		Double_t meanP = smoothpsd->GetMean();
		Double_t rmsP = smoothpsd->GetRMS();

		smoothpsd->GetXaxis()->SetRangeUser(divPSD_fit, maxPSD_fit);
		Double_t psdmaxN = smoothpsd->GetMaximum();
		Double_t binpsdmaxN = smoothpsd->GetMaximumBin();
		Double_t xpsdmaxN = smoothpsd->GetXaxis()->GetBinCenter(binpsdmaxN);
		Double_t meanN = smoothpsd->GetMean();
		Double_t rmsN = smoothpsd->GetRMS();

		smoothpsd->GetXaxis()->SetRangeUser(minPSD_fit, maxPSD_fit);

		psdcombined->SetParameter(0, psdmaxP);
		psdcombined->SetParameter(1, xpsdmaxP);
		psdcombined->SetParameter(2, rmsP);

		psdcombined->SetParameter(3, psdmaxN);
		psdcombined->SetParameter(4, xpsdmaxN);
		psdcombined->SetParameter(5, rmsN);

		psdhists[i]->GetXaxis()->SetRangeUser(minPSD_fit, maxPSD_fit);
		optimized = psdhists[i]->Fit(psdcombined, "SQ");

		psdcombined->SetParameter(0, optimized->Parameter(0));
		psdcombined->SetParameter(1, optimized->Parameter(1));
		psdcombined->SetParameter(2, optimized->Parameter(2));
		psdcombined->SetParameter(3, optimized->Parameter(3));
		psdcombined->SetParameter(4, optimized->Parameter(4));
		psdcombined->SetParameter(5, optimized->Parameter(5));

		psdintersection->SetParameter(0, optimized->Parameter(0));
		psdintersection->SetParameter(1, optimized->Parameter(1));
		psdintersection->SetParameter(2, optimized->Parameter(2));
		psdintersection->SetParameter(3, optimized->Parameter(3));
		psdintersection->SetParameter(4, optimized->Parameter(4));
		psdintersection->SetParameter(5, optimized->Parameter(5));

		psdphotpeak->SetParameter(0, optimized->Parameter(0));
		psdphotpeak->SetParameter(1, optimized->Parameter(1));
		psdphotpeak->SetParameter(2, optimized->Parameter(2));

		psdneutpeak->SetParameter(0, optimized->Parameter(3));
		psdneutpeak->SetParameter(1, optimized->Parameter(4));
		psdneutpeak->SetParameter(2, optimized->Parameter(5));

		double psdtempPSD = psdintersection->GetMinimumX(optimized->Parameter(1), optimized->Parameter(4));
		//cout << optimized->Parameter(1) << " " << psdtempPSD << " " << optimized->Parameter(4) << endl;

		if(psdtempPSD < optimized->Parameter(1) || psdtempPSD > optimized->Parameter(4) || psdtempPSD >= 1) {
			psdtempPSD = divPSD_fit;
		}

		TLine* psdline = new TLine(psdtempPSD, 0, psdtempPSD, optimized->Parameter(0));
		psdline->SetLineColor(kBlack);
		TLine* TOFPSDlinepsd = new TLine(psdtempPSD, -200, psdtempPSD, 200);
		TOFPSDlinepsd->SetLineColor(kRed);


		TString tempName = "PSD" + to_string(i);
		TCanvas* canvaspsd = new TCanvas(tempName, tempName, 800, 500);

		trackpsd[i] = psdtempPSD;
		tracktof[i] = toftempPSD;

		canvaspsd->cd();
		psdhists[i]->Draw();
		psdcombined->Draw("SAME");
		psdintersection->SetLineColor(kViolet+1);
		psdintersection->Draw("SAME");
		psdphotpeak->SetLineColor(kYellow);
		psdneutpeak->SetLineColor(kOrange);
		psdphotpeak->Draw("SAME");
		psdneutpeak->Draw("SAME");
		psdline->Draw("SAME");
		cdPsdIndividual->cd();
		canvaspsd->Write();

		cdPsd->cd();
		tofpsdcanvas->cd();
		tofPsdHists[i]->Draw();
		TOFPSDlinetof->Draw("SAME");
		TOFPSDlinepsd->Draw("SAME");
		cdTOFPSD->cd();
		tofpsdcanvas->Write();
	}

	for(int out=0; out<numDetectors; out++) {
		cout << trackpsd[out] << " ";
	}

	//tof slice discrimination
	for(int det = 0; det < numDetectors; det++)
	{
		cdPsd->cd();
		cout << "\nFor detector " << det << ":" << endl;

		energyBins = tofErgHists[det]->GetNbinsX(); // get the number of slices
		energySliceIndtof = new double [energyBins]; //energy slice
		discLinePointtof = new double [energyBins]; //discrimination
		numGoodSlices = -1; // reset the number of good slices

		// find the profile to get the center
		TProfile* histErgProfile = tofErgHists[det]->ProfileX("profileEnergy");

		cdToF->cd();
		cdTofSlices->cd();
		// loop over the slices
		for(int energySlice = 0; energySlice < energyBins; energySlice += stepSizetof)
		{
			TF1* gausSlicetof = new TF1("gausSlicetof", "[0]*e^(-(x - [1])^2/(2*[2]^2))");
			TF1* neutSlicetof = new TF1("neutSlicetof", "[0]/(1 + ((x - [1])/([2]))^2)");
			TF1* intersectionSlicetof = new TF1("TOFintersect", "abs(neutSlicetof - gausSlicetof)");

			// define the slice
			TH1D* tofErgSlice = tofErgHists[det]->ProjectionY("tofErgSlice", energySlice, energySlice+stepSizetof);
			int numEntriesInSlice = tofErgSlice->GetEntries();
			tofErgSlice->SetLineColor(kBlack);

			// close out of the loop if there is not enough info in the slice
			if(numEntriesInSlice < minEntriestof)
			{
			 	continue;
			}

			// find the initial parameters for the fiting of the photon peak
			tofErgSlice->GetXaxis()->SetRangeUser(tracktof[det]-50, tracktof[det]);
			Double_t tofpMax = tofErgSlice->GetMaximum();
			Double_t tofbinpMax = tofErgSlice->GetMaximumBin();
			Double_t tofxpMax = tofErgSlice->GetXaxis()->GetBinCenter(tofbinpMax);
			Double_t tofpMean = tofErgSlice->GetMean();
			Double_t tofpRMS = tofErgSlice->GetRMS();

			// find the initial parameters for the fitting of the neutron peak
			tofErgSlice->GetXaxis()->SetRangeUser(tracktof[det], tracktof[det]+100);
			Double_t tofnMax = tofErgSlice->GetMaximum();
			Double_t tofbinnMax = tofErgSlice->GetMaximumBin();
			Double_t tofxnMax = tofErgSlice->GetXaxis()->GetBinCenter(tofbinnMax);
			Double_t tofnMean = tofErgSlice->GetMean();
			Double_t tofnRMS = tofErgSlice->GetRMS();

			//reset
			tofErgSlice->GetXaxis()->SetRangeUser(-200, 200);

			// set the initial guesses
			gausSlicetof->SetParameter(0, tofpMax);
			gausSlicetof->SetParameter(1, tofxpMax);
			gausSlicetof->SetParameter(2, tofpRMS);
			TFitResultPtr gausSliceOptimize = tofErgSlice->Fit(gausSlicetof, "SQ");
			gausSlicetof->SetParameter(0, gausSliceOptimize->Parameter(0));
			gausSlicetof->SetParameter(1, gausSliceOptimize->Parameter(1));
			gausSlicetof->SetParameter(2, gausSliceOptimize->Parameter(2));

			neutSlicetof->SetParameter(0, tofnMax);
			neutSlicetof->SetParameter(1, tofxnMax);
			neutSlicetof->SetParameter(2, tofnRMS);
			TFitResultPtr neutSliceOptimize = tofErgSlice->Fit(neutSlicetof, "+SQ");
			neutSlicetof->SetParameter(0, neutSliceOptimize->Parameter(0));
			neutSlicetof->SetParameter(1, neutSliceOptimize->Parameter(1));
			neutSlicetof->SetParameter(2, neutSliceOptimize->Parameter(2));

			intersectionSlicetof->SetParameter(0, gausSliceOptimize->Parameter(0));
			intersectionSlicetof->SetParameter(1, gausSliceOptimize->Parameter(1));
			intersectionSlicetof->SetParameter(2, gausSliceOptimize->Parameter(2));
			intersectionSlicetof->SetParameter(3, neutSliceOptimize->Parameter(0));
			intersectionSlicetof->SetParameter(4, neutSliceOptimize->Parameter(1));
			intersectionSlicetof->SetParameter(5, neutSliceOptimize->Parameter(2));

			// find the optimal psd line
			double temptofslice = intersectionSlicetof->GetMinimumX(gausSliceOptimize->Parameter(1), neutSliceOptimize->Parameter(1));
			double tempergSlice = (histErgProfile->GetBinCenter(energySlice) + histErgProfile->GetBinCenter(energySlice + stepSizetof))/2.0;
			cout << gausSliceOptimize->Parameter(1) << "    " << temptofslice << "," << tempergSlice << "    " << neutSliceOptimize->Parameter(1);

			if(temptofslice < gausSliceOptimize->Parameter(1) || temptofslice > neutSliceOptimize->Parameter(1) || temptofslice >= 200) {
				cout << "     throw" << endl;
				continue;
			}
			else {
				numGoodSlices++;
				cout << endl << numGoodSlices << endl;
			}
			// find the points for the graph
			energySliceIndtof[numGoodSlices] = tempergSlice;
			discLinePointtof[numGoodSlices] = temptofslice;

			TString cantofSliceName = "tof" + to_string(det) +  "Proj" + to_string(numGoodSlices);

			// canvas with the psd
			TCanvas* canvastofSlice = new TCanvas(cantofSliceName, cantofSliceName, 800, 500);

			tofErgSlice->GetYaxis()->SetRangeUser(0, 1.5*gausSliceOptimize->Parameter(0));

			canvastofSlice->cd();

			tofErgSlice->Draw();
			intersectionSlicetof->Draw("SAME");
			gausSlicetof->SetLineColor(kYellow);
			neutSlicetof->SetLineColor(kOrange);

			// discrimination line
			TLine* tofline = new TLine(temptofslice, 0, temptofslice, 1.5*gausSliceOptimize->Parameter(0));
			tofline->SetLineColor(kBlack);
			tofline->Draw("SAME");

			cdToF->cd();
			cdTofSlices->cd();
			canvastofSlice->Write();
		}
		cdPsd->cd();

		cout << "Finished looping through TOF slices" << endl;

		cout << "all disc: ";
		for(int i=0; i<numGoodSlices+1; i++) {
			cout << discLinePointtof[i] << " ";
		}
		cout << endl << endl;

		// create a canvas with the psd discrimination
		TString canDiscErgNametof = "tofErg" + to_string(det);
		TCanvas* canvasDiscErgtof = new TCanvas(canDiscErgNametof, canDiscErgNametof, 800, 500);

		// create a new psd discrimination line
	  discLinestof[det] = new TGraph(numGoodSlices+1, energySliceIndtof, discLinePointtof);
		discLinestof[det]->SetLineColor(kRed);
		discLinestof[det]->SetLineWidth(3);

		// fit the psd discriminations
		psdDiscFittof = discLinestof[det]->Fit(fitDisctof, "SQ", 0, 0.4); // change the bounds here

		// extract fit information
		p0Disc = psdDiscFittof->Parameter(0);
		p1Disc = psdDiscFittof->Parameter(1);
		//p2Disc = psdDiscFit->Parameter(2);

		// set discrimination line
		fitDisctof->SetParameters(p0Disc, p1Disc, p2Disc);
		fitDisctof->SetParameters(p0Disc, p1Disc);	//whats the purpose of this?

		// TString nameDisctof = "discDet" + to_string(det);
		// detectors[det].discPSD = new TF1(nameDisctof, "FitDisc", minErg_fit, maxErg_fit);
		// detectors[det].discPSD->SetParameters(p0Disc, p1Disc, p2Disc);

		// draw the line on top of the histogram
		canvasDiscErgtof->cd();
		tofErgHists[det]->Draw();
		discLinestof[det]->Draw("SAME");
		fitDisctof->Draw("SAME");
		cdTofErg->cd();
		canvasDiscErgtof->Write();
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
		 if(totPSP[part] < detectors[channelDet].discPSD->Eval(totDep[part]))
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
