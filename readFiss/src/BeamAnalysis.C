#include "readFiss.h"

using namespace std;

void readFiss::BeamDepAnalysis()
{
  cd_beam->cd();
  // set the correct initial parameters for the beam
  f_TimeFromErg->SetParameters(BEAM_PATH, LIGHT_C, N_MASS);

  double minTimeFiss = f_TimeFromErg->Eval(MAX_ERG_BEAM);
  double maxTimeFiss = f_TimeFromErg->Eval(MIN_ERG_BEAM);
  intWindowFiss = maxTimeFiss - minTimeFiss;
  cout << "Integrating fission over " << intWindowFiss << " (ns)" << endl;

  // loop through the ppac plates
  double scaleAlpha, scaleFiss;
  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    cout << r << endl;

    // compute the profiles
    p_neutronMultDep[r] = h2_neutronMultDep[r]->ProfileX("p_neutronMult" + s_TRIG_NUM);
		p_gammaMultDep[r] = h2_gammaMultDep[r]->ProfileX("p_gammaMult"  + s_TRIG_NUM);
    p_backNeutronMultDep[r] = h2_backNeutronMultDep[r]->ProfileX("p_backNeutronMult" + s_TRIG_NUM);
		p_backGammaMultDep[r] = h2_backGammaMultDep[r]->ProfileX("p_backGammaMult" + s_TRIG_NUM);

    cout << "declared profiles" << endl;

    // find the scaling factors
		scaleFiss = h_macroPop->GetMean() * intWindowFiss; // times the size in ns of the integration window
		scaleAlpha = intWindowAlpha;
		h_fisDep[r]->Scale(1 / scaleFiss);		//Changing counts into count rate in the fission chamber
		h_alphaDep[r]->Scale(1 / scaleAlpha);		//Changing counts into count rate for alpha background


	 //Subtract alphas from fisDep
    h_fisSubtract[r] = (TH1D*)h_fisDep[r]->Clone("h_fisSubtract");						//Clone fission chamber histogram for future isolation of fission products
		h_fisSubtract[r]->Add(h_alphaDep[r], -1);													//cloned fisDep histogram

    cout << "subtracted histograms" << endl;

  // fit the alpha background This could be its own function
    double MaxBinCount = h_alphaDep[r]->GetMaximumBin();									//Use GetMaximumBin to find candidate for peak (most events/counts)

   // JAMES & NATHAN: make an array of functions and FitResultsPtr in readFiss.h, declare them in a new file initializeFunctions.h, and save the fits for each
   //  f_alphaBackground->SetRange(h_alphaSpec->GetBinCenter(MaxBinCount),BIN_ERG_MAX);	//Set alpha fit range to start at middle of peak of data
   //  fitAlphas = h_alphaSpec->Fit("f_alphaBackground","RS");				//Initialize fit of alphaBackground in specified range (after peak) to ptr allowing for extraction of the variables of the fit
   //  expoConst = fitAlphas->Value(0);												//Get constant of exponential fit
   //  expoSlope = fitAlphas->Value(1);												//Get slope of exponential fit
   //
   //
	 // //Fit fission products
		// fitFis = h_fisSubtract->Fit("f_fisProducts","RS");					//Initialize fit of fissions to ptr for value extraction later
		// gausAmp = fitFis->Value(0);													//Get amplitude of fit gaussian
		// gausMean = fitFis->Value(1);													//Get mean of fit gaussian
		// gausSTD = fitFis->Value(2);													//Get standard deviation of fit gaussian
   //
	 // //put fission product fit into gaussian and alphaBackground fit into exponential
		// f_gausProducts->SetParameter(0,gausAmp);			//Set amplitude of created gaussian to that of the gaussian fit of fissions
		// f_gausProducts->SetParameter(1,gausMean);			//Set mean of created gaussian to that of the gaussian fit of fissions
		// f_gausProducts->SetParameter(2,gausSTD);			//Set standard deviation of created gaussian to that of the gaussian fit of fissions
		// f_gausProducts->SetLineColor(kGreen);
   //
		// f_expoBackground->SetParameter(0,expoConst);		//Set exponential constant of created exponential to that of the exponential fit of alphas
		// f_expoBackground->SetParameter(1,expoSlope);		//Set exponential slope of created exponential to that of the exponential fit of alphas
		// f_expoBackground->SetLineColor(kRed);


	//Calculate ratio of fisions to alphas
		//Energy Threshold (threshold and above)
    // JAMES & NATHAN: maybe put this in the Constants.h

		double minDepBin = 200;									 	//Max Energy Threshold
		double maxDepBin = -1;											//Final bin in histogram

    // JAMES & NATHAN: make an array of TGraphs for the fisDep dependent ratio
		// //cout << "Bin #; Energy Threshold; Ratio" << endl;
		// for (i = 1; i < minDepBin; i++){						//for loop to get all threshold ratios until max threshold bin
		// 	minDepErg = h_fisSubtract[r]->GetBinCenter(i);		//Get threshold value
		// 	numFis = h_fisSubtract[r]->Integral(i,maxDepBin);	//Get area under fission products curve from threshold
		// 	numTot = h_fisDep[r]->Integral(i,maxDepBin);		//Get area under total histogram from threshold
		// 	fisRatio->SetPoint(i, minDepErg, numFis/numTot);		//Put data points into graph
		// 	//cout << i << " " << minDepErg << " " << numFis/numTot << endl;
		// }
		// fisRatio->Draw();											//Draw fission fraction to canvas
		// fisRatio->SetTitle("Ratio of Fissions to Alphas due to Energy Threshold; Energy Threshold (V us); Fraction of Fissions");
    //
		// //Energy Segments (between two energies)
		// binSeg = 10;									//Energy segment width (10 = width of 0.001; 20 = 0.002)
		// for (j = 1; j < minDepBin/binSeg; j++){																			//Get all segments before energy threshold
		// 	minDepErgSeg = 0.5*(h_fisSubtract->GetBinCenter(j*binSeg) + h_fisSubtract->GetBinCenter((j+1)*binSeg));	//Get middle of threshold to plot on graph so that
		// 	numFisSeg = h_fisSubtract->Integral(j*binSeg,(j+1)*binSeg);												//Integrating over energy segment for fission products curve
		// 	numTotSeg = h_fisDep->Integral(j*binSeg,(j+1)*binSeg);													//Integrating over energy segment for total hisogram
		// 	fisSegRatio->SetPoint(j,minDepErgSeg,numFisSeg/numTotSeg);														//Set points on graph
		// 	//cout << minDepErgSeg << " " << numFisSeg << " " << numTotSeg << " " << numFisSeg/numTotSeg << endl;
		// }
		// //fisSegRatio->Draw();					//*UNCOMMENT* if desire energy segment ratios on main canvas
		// //fisSegRatio->SetTitle("Ratio of Fissions to Alphas in Energy Segments (0.001); Energy Segment Center (V us); Fraction of Fissions");
    //
    //
		// // create ratio of multiplicity to fission
		// binWidth = 0.0001;
		// for (k = 0; k <= minDepBin; k++){
    //
		// 	nMult = p_neutronMult->GetBinContent(k);
		// 	gMult = p_gammaMult->GetBinContent(k);
    //
		// 	nbMult = p_backNeutronMult->GetBinContent(k); // get the background n
		// 	gbMult = p_backGammaMult->GetBinContent(k); // get the background g
    //
		// 	ergPt = p_neutronMult->GetBinCenter(k+1);
    //
		// 	pg_neutronMult->SetPoint(k+1, ergPt, nMult - nbMult);
		// 	pg_gammaMult->SetPoint(k+1, ergPt, gMult - gbMult);
    //
		// 	pg_neutronMultRatio->SetPoint(k+1, ergPt, (nMult - nbMult)/fisSegRatio->Eval(ergPt));
		// 	pg_gammaMultRatio->SetPoint(k+1, ergPt, (gMult - gbMult)/fisSegRatio->Eval(ergPt));
		// }

    // JAMES & NATHAN: see comments above about plotting
		// //Compare Energy Segments with Profiles of Neutron and Gamma Multiplicity
		// c2->cd(1);
		// fisSegRatio->Draw();										//Draw segmented energy fission alpha ratios
		// p_neutronMult->Draw("SAME");								//Draw profile of neutronMult with ratio of fissions to alphas
		// p_gammaMult->Draw("SAME");									//Draw profile of gammaMult with ratio of fissions to alphas
   //
		// fisSegRatio->SetLineColor(black);		//**
		// p_neutronMult->SetLineColor(blue);	//**
		// p_gammaMult->SetLineColor(red);		//**
		// fisSegRatio->SetTitle("Neutron and Gamma Multiplicity Comparision to Energy Segment Ratios of 0.001; Pulse Integral (V us); Fractional Value");
   //
		// TLegend *segLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
		// segLeg->AddEntry("fisSegRatio","Fission Fraction","l");
		// segLeg->AddEntry("p_neutronMult","Mean Neutron Mult","l");
		// segLeg->AddEntry("p_gammaMult","Mean Gamma Mult","l");
		// segLeg->Draw();												//Draw Legend
   //
		// c2->cd(2);
		// pg_neutronMultRatio->Draw();
		// pg_gammaMultRatio->Draw("SAME");
   //
		// pg_neutronMultRatio->SetLineColor(blue);	//**
		// pg_gammaMultRatio->SetLineColor(red);	//**
		// pg_neutronMultRatio->SetTitle("Fission Particle Multiplicities;PPAC Deposited Energy (V us);Mean Multiplicity");
   //
		// TLegend *ratLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
		// ratLeg->AddEntry("pg_neutronMultRatio","Mean Neutron Mult","l");
		// ratLeg->AddEntry("pg_gammaMultRatio","Mean Gamma Mult","l");
		// ratLeg->Draw();
   //
	 // //Write canvas in TBrowser
		// writeFile->cd();
		// c1->Write();			//write c1 to TBrowser
		// c1->Update();			//Update c1
		// c2->Write();			//Write c2 to TBrowser
		// c2->Update();			//Update c2

  }

}

void readFiss::BeamErgAnalysis()
{

}
