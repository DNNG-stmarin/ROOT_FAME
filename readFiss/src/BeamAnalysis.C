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

  // h_fisDepSelect = // integrate the whole distributioon betwenn
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
    scaleAlpha = 1*intWindowAlpha;
		h_fisDep[r]->Scale(1.0/scaleFiss);		//Changing counts into count rate in the fission chamber
		h_alphaDep[r]->Scale(1.0/scaleAlpha);		//Changing counts into count rate for alpha background

	 //Subtract alphas from fisDep
    h_fisSubtract[r] = (TH1D*)h_fisDep[r]->Clone("h_fisSubtract");						//Clone fission chamber histogram for future isolation of fission products
		h_fisSubtract[r]->Add(h_alphaDep[r], -1);													        //cloned fisDep histogram

    cout << "subtracted histograms" << endl;

  // fit the alpha background This could be its own function
    double maxCountBin = h_alphaDep[r]->GetMaximumBin();									//Use GetMaximumBin to find candidate for peak (most events/counts)
   // JAMES & NATHAN: make an array of functions and FitResultsPtr in readFiss.h, declare them in a new file initializeFunctions.h, and save the fits for each
   f_alpha[r]->SetRange(h_alphaDep[r]->GetBinCenter(maxCountBin), DEP_MAX);
   h_alphaDep[r]->Fit((TString)"f_alpha" + (TString)to_string(r), "RS");
   f_expo[r]->SetParameters(f_alpha[r]->GetParameter(0), f_alpha[r]->GetParameter(1));

   h_fisSubtract[r]->Fit((TString)"f_fisProducts" + (TString)to_string(r));
   f_gauss[r]->SetParameters(f_fisProducts[r]->GetParameter(0),
                             f_fisProducts[r]->GetParameter(1),
                             f_fisProducts[r]->GetParameter(2));

   cout << "performing fits" << endl;


   //  f_alphaBackground->SetRange(h_alphaSpec->GetBinCenter(maxCountBin),BIN_ERG_MAX);	//Set alpha fit range to start at middle of peak of data
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


		double minDepBin = 200;									 	//Max Energy Threshold
		double maxDepBin = -1;											//Final bin in histogram

    // cout << h_fisSubtract[0]->GetBinCenter(30) << endl;

		//cout << "Bin #; Energy Threshold; Ratio" << endl;
    double minDepErg, numFis, numTot;
    // cout << "start" << endl;
		for (int i = 1; i < minDepBin; i++)
    {						//for loop to get all threshold ratios until max threshold bin
			minDepErg = h_fisSubtract[r]->GetBinCenter(i);		//Get threshold value
			numFis = h_fisSubtract[r]->Integral(i, maxDepBin);	//Get area under fission products curve from threshold
			numTot = h_fisDep[r]->Integral(i, maxDepBin);		//Get area under total histogram from threshold
			g_fisRatioThreshold[r]->SetPoint(i, minDepErg, numFis/numTot);		//Put data points into graph
			//cout << i << " " << minDepErg << " " << numFis/numTot << endl;
		}

    g_fisRatioThreshold[r]->SetName("fissRatioThreshold_" + s_TRIG_NUM);
		g_fisRatioThreshold[r]->SetTitle("Ratio of Fissions to Alphas due to Energy Threshold; Energy Threshold (V us); Fraction of Fissions");

    cout << "finished threshold" << endl;

		//Energy Segments (between two energies)
		int binSeg = 10;									//Energy segment width (10 = width of 0.001; 20 = 0.002)
		for (int j = 1; j < minDepBin/binSeg; j++){																			//Get all segments before energy threshold
			minDepErg = 0.5*(h_fisSubtract[r]->GetBinCenter(j*binSeg) + h_fisSubtract[r]->GetBinCenter((j+1)*binSeg));	//Get middle of threshold to plot on graph so that
			numFis = h_fisSubtract[r]->Integral(j*binSeg,(j+1)*binSeg);												//Integrating over energy segment for fission products curve
			numTot = h_fisDep[r]->Integral(j*binSeg,(j+1)*binSeg);													//Integrating over energy segment for total hisogram
			g_fisRatioSelect[r]->SetPoint(j, minDepErg, numFis/numTot);														//Set points on graph
			//cout << minDepErgSeg << " " << numFisSeg << " " << numTotSeg << " " << numFisSeg/numTotSeg << endl;
		}
    g_fisRatioSelect[r]->SetName("fissRatioSelect_" + s_TRIG_NUM);
    g_fisRatioSelect[r]->SetTitle("Ratio of Fissions to Alphas due to Energy Selection; Energy Selection (V us); Fraction of Fissions");

    cout << "finished selection" << endl;

		// create ratio of multiplicity to fission
		double binWidth = 0.0001;
    double nMult, gMult, nbMult, gbMult, ergPt;
    TGraph *pg_neutronMult = new TGraph(minDepBin);
    TGraph *pg_gammaMult = new TGraph(minDepBin);
		for (int k = 0; k <= minDepBin; k++){

			nMult = p_neutronMultDep[r]->GetBinContent(k);
			gMult = p_gammaMultDep[r]->GetBinContent(k);

			nbMult = p_backNeutronMultDep[r]->GetBinContent(k); // get the background n
			gbMult = p_backGammaMultDep[r]->GetBinContent(k); // get the background g

			ergPt = p_neutronMultDep[r]->GetBinCenter(k+1);

			pg_neutronMult->SetPoint(k+1, ergPt, nMult - nbMult);
			pg_gammaMult->SetPoint(k+1, ergPt, gMult - gbMult);

			g_neutronMultRatioDep[r]->SetPoint(k+1, ergPt, (nMult - nbMult)/g_fisRatioSelect[r]->Eval(ergPt));
			g_gammaMultRatioDep[r]->SetPoint(k+1, ergPt, (gMult - gbMult)/g_fisRatioSelect[r]->Eval(ergPt));
		}

    g_neutronMultRatioDep[r]->SetName("g_neutronMultRatioDep" + s_TRIG_NUM);
    g_neutronMultRatioDep[r]->SetTitle("Fission Neutron Multiplicity; Energy Selection (V us); Average Multiplicity");

    g_gammaMultRatioDep[r]->SetName("g_gammaMultRatioDep" + s_TRIG_NUM);
    g_gammaMultRatioDep[r]->SetTitle("Fission Gamma Multiplicity; Energy Selection (V us); Average Multiplicity");

    cout << "taking ratios" << endl;

  }

}

void readFiss::BeamErgAnalysis()
{

}
