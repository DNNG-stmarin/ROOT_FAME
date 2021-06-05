#include "readFiss.h"
#include <TFitResultPtr.h>
#include <Constants.h>

using namespace std;


void readFiss::BeamDepAnalysis()
{
  cd_beam->cd();
  // set the correct initial parameters for the beam
  f_TimeFromErg->SetParameters(BEAM_PATH, LIGHT_C, N_MASS);

  double minTimeFiss = f_TimeFromErg->Eval(BEAM_ERG_MAX);
  double maxTimeFiss = f_TimeFromErg->Eval(BEAM_ERG_MIN);
  intWindowFiss = maxTimeFiss - minTimeFiss;
  cout << "Integrating fission over " << intWindowFiss << " (ns)" << endl;

  // loop through the ppac plates

  // h_fisDepSelect = // integrate the whole distributioon betwenn
  double scaleFiss;
  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    cout << r << endl;

    // compute the profiles
    p_neutronMultDep[r] = h2_neutronMultDep[r]->ProfileX("p_neutronMult" + s_TRIG_NUM);
		p_gammaMultDep[r] = h2_gammaMultDep[r]->ProfileX("p_gammaMult"  + s_TRIG_NUM);
    p_backNeutronMultDep[r] = h2_backNeutronMultDep[r]->ProfileX("p_backNeutronMult" + s_TRIG_NUM);
		p_backGammaMultDep[r] = h2_backGammaMultDep[r]->ProfileX("p_backGammaMult" + s_TRIG_NUM);

    // h2_neutronMultDep[r]->Write();
    // h2_gammaMultDep[r]->Write();
    // p_neutronMultDep[r]->Write();
    // p_gammaMultDep[r]->Write();

    cout << "declared profiles" << endl;

    // find the scaling factors
		scaleFiss = h_macroPop->GetMean() * intWindowFiss; // times the size in ns of the integration window
		h_fisDep[r]->Scale(1.0 / scaleFiss);		//Changing counts into count rate in the fission chamber

	 //Subtract alphas from fisDep
    h_fisSubtract[r] = (TH1D*)h_fisDep[r]->Clone("h_fisSubtract");						//Clone fission chamber histogram for future isolation of fission products
		h_fisSubtract[r]->Add(h_alphaDep[r], -1);													        //cloned fisDep histogram

    cout << "subtracted histograms" << endl;

    // Fit alpha background and subtracted fissions
    h_alphaDep[r]->Fit((TString)"f_alpha" + (TString)to_string(r),
        "", "", h_alphaDep[r]->GetBinCenter(h_alphaDep[r]->GetMaximumBin()), DEP_MAX);
    f_expo[r]->SetParameters(f_alpha[r]->GetParameter(0), f_alpha[r]->GetParameter(1));

    h_fisSubtract[r]->Fit((TString)"f_fisProducts" + (TString)to_string(r));
    f_gauss[r]->SetParameters(f_fisProducts[r]->GetParameter(0),
                              f_fisProducts[r]->GetParameter(1),
                              f_fisProducts[r]->GetParameter(2));
    f_gauss[r]->SetLineColor(kGreen);

    cout << "performing fits" << endl;



		double minDepBin = 200;									 	//Max Energy Threshold
		double maxDepBin = -1;											//Final bin in histogram

    // cout << h_fisSubtract[0]->GetBinCenter(30) << endl;

		//cout << "Bin #; Energy Threshold; Ratio" << endl;
    double minDepErg, numFis, numTot;
    int numPoints = 0;
    // cout << "start" << endl;
		for (int i = 1; i < minDepBin; i++)
    {						//for loop to get all threshold ratios until max threshold bin
			minDepErg = h_fisSubtract[r]->GetBinCenter(i);		//Get threshold value
			numFis = h_fisSubtract[r]->Integral(i, maxDepBin);	//Get area under fission products curve from threshold
			numTot = h_fisDep[r]->Integral(i, maxDepBin);		//Get area under total histogram from threshold
			g_fisRatioThreshold[r]->SetPoint(i, minDepErg, numFis/numTot);		//Put data points into graph
			//cout << i << " " << minDepErg << " " << numFis/numTot << endl;
		}

    g_fisRatioThreshold[r]->SetName("fissRatioThreshold" + s_TRIG_NUM);
		g_fisRatioThreshold[r]->SetTitle("Ratio of Fissions to Alphas due to Energy Threshold; Energy Threshold (V us); Fraction of Fissions");
    g_fisRatioThreshold[r]->Set(numPoints);
    cout << "finished threshold" << endl;

		//Energy Segments (between two energies)
		int binSeg = 10;									//Energy segment width (10 = width of 0.001; 20 = 0.002)
    numPoints = 0;
		for (int j = 0; j < minDepBin/binSeg; j++)
    {																			//Get all segments before energy threshold
      numPoints++;
			minDepErg = 0.5*(h_fisSubtract[r]->GetBinCenter(j*binSeg) + h_fisSubtract[r]->GetBinCenter((j+1)*binSeg));	//Get middle of threshold to plot on graph so that
			numFis = h_fisSubtract[r]->Integral(j*binSeg,(j+1)*binSeg);												//Integrating over energy segment for fission products curve
			numTot = h_fisDep[r]->Integral(j*binSeg,(j+1)*binSeg);													//Integrating over energy segment for total hisogram
			g_fisRatioSelect[r]->SetPoint(j, minDepErg, numFis/numTot);														//Set points on graph
			//cout << minDepErgSeg << " " << numFisSeg << " " << numTotSeg << " " << numFisSeg/numTotSeg << endl;
		}
    g_fisRatioSelect[r]->SetName("fissRatioSelect" + s_TRIG_NUM);
    g_fisRatioSelect[r]->SetTitle("Ratio of Fissions to Alphas due to Energy Selection; Energy Selection (V us); Fraction of Fissions");
    g_fisRatioSelect[r]->Set(numPoints);

    cout << "finished selection" << endl;

		// create ratio of multiplicity to fission
		double binWidth = 0.0001;
    double nMult, gMult, nbMult, gbMult, ergPt;
    TGraph *pg_neutronMult = new TGraph(minDepBin);
    TGraph *pg_gammaMult = new TGraph(minDepBin);
    numPoints = 0;
		for (int k = 0; k <= minDepBin; k++)
    {
      numPoints++;
			nMult = p_neutronMultDep[r]->GetBinContent(k);
			gMult = p_gammaMultDep[r]->GetBinContent(k);

			nbMult = p_backNeutronMultDep[r]->GetBinContent(k); // get the background n
			gbMult = p_backGammaMultDep[r]->GetBinContent(k); // get the background g

			ergPt = p_neutronMultDep[r]->GetBinCenter(k+1);

			pg_neutronMult->SetPoint(k, ergPt, nMult - nbMult);
			pg_gammaMult->SetPoint(k, ergPt, gMult - gbMult);

			g_neutronMultRatioDep[r]->SetPoint(k, ergPt, (nMult - nbMult)/g_fisRatioSelect[r]->Eval(ergPt));
			g_gammaMultRatioDep[r]->SetPoint(k, ergPt, (gMult - gbMult)/g_fisRatioSelect[r]->Eval(ergPt));
		}

    g_neutronMultRatioDep[r]->SetName("g_neutronMultRatioDep" + s_TRIG_NUM);
    g_neutronMultRatioDep[r]->SetTitle("Fission Neutron Multiplicity; Energy Selection (V us); Average Multiplicity");
    g_neutronMultRatioDep[r]->Set(numPoints);

    g_gammaMultRatioDep[r]->SetName("g_gammaMultRatioDep" + s_TRIG_NUM);
    g_gammaMultRatioDep[r]->SetTitle("Fission Gamma Multiplicity; Energy Selection (V us); Average Multiplicity");
    g_gammaMultRatioDep[r]->Set(numPoints);

    //cout << "taking ratios" << endl;



  }

}

void readFiss::BeamErgAnalysis()
{
  cd_beam->cd();
  // set the correct initial parameters for the beam
  f_TimeFromErg->SetParameters(BEAM_PATH, LIGHT_C, N_MASS);

  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    cout << "Trigger " << r + 1 << " of " << NUM_TRIGGERS << endl;

    // Populate profiles
    p_neutronMultErg[r] = h2_neutronMultErg[r]->ProfileX("p_neutronMultErg" + s_TRIG_NUM);
    p_gammaMultErg[r] = h2_gammaMultErg[r]->ProfileX("p_gammaMultErg" + s_TRIG_NUM);
    p_backNeutronMultErg[r] = h2_backNeutronMultErg[r]->ProfileX("p_backNeutronMultErg" + s_TRIG_NUM);
    p_backGammaMultErg[r] = h2_backGammaMultErg[r]->ProfileX("p_backGammaMultErg" + s_TRIG_NUM);

    for (int i = 0; i < BEAM_ERG_BINNUM; i++)
    {
      double ergBinLow = BEAM_ERG_MIN + i * (BEAM_ERG_MAX - BEAM_ERG_MIN) / BEAM_ERG_BINNUM;
      double ergBinHigh = ergBinLow + (BEAM_ERG_MAX - BEAM_ERG_MIN) / BEAM_ERG_BINNUM;
      double meanErg = (ergBinLow + ergBinHigh) / 2;

      double timeBinLow = f_TimeFromErg->Eval(ergBinLow);
      double timeBinHigh = f_TimeFromErg->Eval(ergBinHigh);

      TH1D* h_ergDep = h2_fisDepErg[r]->ProjectionX("h_ergDep", i + 1, i + 1);
      // cout << "Erg bins: " << meanErg << " " << h2_fisDepErg[r]->GetYaxis()->GetBinCenter(i + 1) << endl;

      h_ergDep->Scale(1 / (h_macroPop->GetMean() * (timeBinLow - timeBinHigh)), "n");

      TH1D *h_ergSubtract = (TH1D*)h_ergDep->Clone("h_ergSubtract");
      h_ergSubtract->Add(h_alphaDep[r], -1);  // Subtract alpha count rate

      // Take integrals
      int thresholdDepBin = h_ergDep->GetXaxis()->FindBin(THRESHOLD_DEP);

      double numFis = h_ergSubtract->Integral(thresholdDepBin, -1);
      double numTot = h_ergDep->Integral(thresholdDepBin, -1);
      double numAlpha = h_alphaDep[r]->Integral(thresholdDepBin, -1);


      // Add points to graphs
      g_fisRatioErg[r]->SetPoint(i, meanErg, numFis / numTot);

      // double nMultSubt = (p_neutronMultErg[r]->GetBinContent(i + 1)
      //   - p_backNeutronMultErg[r]->GetBinContent(i + 1)) * numTot / numFis;
      // double gMultSubt = (p_gammaMultErg[r]->GetBinContent(i + 1)
      //   - p_backGammaMultErg[r]->GetBinContent(i + 1)) * numTot / numFis;
      double nMultSubt = numTot / numFis * p_neutronMultErg[r]->GetBinContent(i + 1)
        - p_backNeutronMultErg[r]->GetBinContent(i + 1);
      double gMultSubt = numTot / numFis * p_gammaMultErg[r]->GetBinContent(i + 1)
        - p_backGammaMultErg[r]->GetBinContent(i + 1);

      g_nMultErg[r]->SetPoint(i, meanErg, nMultSubt);
      g_gMultErg[r]->SetPoint(i, meanErg, gMultSubt);

      g_gMultnMult[r]->SetPoint(i, nMultSubt, gMultSubt);

      g_nMultBackErg[r]->SetPoint(i, meanErg,
        p_backNeutronMultErg[r]->GetBinContent(i + 1));
      g_gMultBackErg[r]->SetPoint(i, meanErg,
        p_backGammaMultErg[r]->GetBinContent(i + 1));

    }

    // Set graph names
    g_fisRatioErg[r]->SetName("g_fisRatioErg" + s_TRIG_NUM);
    g_fisRatioErg[r]->SetTitle("Fission-to-total ratio; Beam energy [MeV];f / (f + #alpha)");

    g_nMultErg[r]->SetName("g_nMultErg" + s_TRIG_NUM);
    g_nMultErg[r]->SetTitle("Fission Neutron Multiplicity; Beam energy [MeV]; Average Multiplicity");

    g_gMultErg[r]->SetName("g_gMultErg" + s_TRIG_NUM);
    g_gMultErg[r]->SetTitle("Fission Photon Multiplicity; Beam energy [MeV]; Average Multiplicity");

    g_nMultBackErg[r]->SetName("g_nMultBackErg" + s_TRIG_NUM);
    g_nMultBackErg[r]->SetTitle("Background Neutron Multiplicity; Beam energy [MeV]; Average Multiplicity");

    g_gMultBackErg[r]->SetName("g_gMultBackErg" + s_TRIG_NUM);
    g_gMultBackErg[r]->SetTitle("Background Photon Multiplicity; Beam energy [MeV]; Average Multiplicity");

    g_gMultnMult[r]->SetName("g_gMultnMult" + s_TRIG_NUM);
    g_gMultnMult[r]->SetTitle("Photon / Neutron Multiplicity;Average Neutron Multiplicity;Average Photon Multiplicity");
  }
}

void readFiss::FitMult(){ //Doesn't run function so had try code in above function

  TFitResultPtr nMultFit;
  TFitResultPtr gMultFit;
  Int_t n = NUM_TRIGGERS;
  Double_t TriggerAxis[MAX_TRIGGERS];

  Double_t gammaSlope[MAX_TRIGGERS];
  Double_t gammaSlopeError[MAX_TRIGGERS];
  Double_t gammaIntercept[MAX_TRIGGERS];
  Double_t gammaInterceptError[MAX_TRIGGERS];

  Double_t neutronSlope[MAX_TRIGGERS];
  Double_t neutronSlopeError[MAX_TRIGGERS];
  Double_t neutronIntercept[MAX_TRIGGERS];
  Double_t neutronInterceptError[MAX_TRIGGERS];

  Double_t ratioSlope[MAX_TRIGGERS];
  Double_t ratioSlopeError[MAX_TRIGGERS];
  Double_t nRatioSlopeInt[MAX_TRIGGERS];
  Double_t nRatioSlopeIntError[MAX_TRIGGERS];
  Double_t gRatioSlopeInt[MAX_TRIGGERS];
  Double_t gRatioSlopeIntError[MAX_TRIGGERS];


  for (int r = 0; r < NUM_TRIGGERS; r++){
    TriggerAxis[r] = r;
    TString s_TRIG_NUM = (TString)to_string(r);

    //fit (linear) g_nMultErg[r] and g_gMultErg[r] ratio multiplicty lines as function of r (PPAC)
    nMultFit = g_nMultErg[r]->Fit((TString)"f_aveNmult" + s_TRIG_NUM, "SQ");
    gMultFit = g_gMultErg[r]->Fit((TString)"f_aveGmult" + s_TRIG_NUM, "SQ");
    // f_aveNmult[r]->SetLineColor(kBlue);

    //Print out results, slopes, and intercepts
    gammaIntercept[r] = f_aveGmult[r]->GetParameter(0);
    gammaSlope[r] = f_aveGmult[r]->GetParameter(1);
    neutronIntercept[r] = f_aveNmult[r]->GetParameter(0);
    neutronSlope[r] = f_aveNmult[r]->GetParameter(1);

    ratioSlope[r] = gammaSlope[r]/neutronSlope[r];
    nRatioSlopeInt[r] = neutronSlope[r]/neutronIntercept[r];
    gRatioSlopeInt[r] = gammaSlope[r]/gammaIntercept[r];

    gammaInterceptError[r] = f_aveGmult[r]->GetParError(0);
    gammaSlopeError[r] = f_aveGmult[r]->GetParError(1);
    neutronInterceptError[r] = f_aveNmult[r]->GetParError(0);
    neutronSlopeError[r] = f_aveNmult[r]->GetParError(1);

    ratioSlopeError[r] = (gammaSlope[r]/neutronSlope[r])*sqrt(pow(neutronSlopeError[r]/neutronSlope[r], 2) + pow(gammaSlopeError[r]/gammaSlope[r], 2));
    nRatioSlopeIntError[r] = (nRatioSlopeInt[r])*sqrt(pow(neutronSlopeError[r]/neutronSlope[r], 2) + pow(neutronInterceptError[r]/neutronIntercept[r], 2));
    gRatioSlopeIntError[r] = (gRatioSlopeInt[r])*sqrt(pow(gammaSlopeError[r]/gammaSlope[r], 2) + pow(gammaInterceptError[r]/gammaIntercept[r], 2));

    // cout << "Ratio of Error to slope value: " << gammaSlopeError/gammaSlope << endl;
    // cout << "Gamma Intercept: " << f_aveGmult[r]->GetParameter(0) << " Gamma Slope: " << gammaSlope << " +/- " << gammaSlopeError << endl;
    // cout << "Neutron Intercept: " << f_aveNmult[r]->GetParameter(0) << " Neutron Slope: " << neutronSlope << " +/- " << neutronSlopeError << endl;
  }
  g_gnRatio = new TGraphErrors(n, TriggerAxis, ratioSlope, 0, ratioSlopeError);
  g_nRatioSlopeInt = new TGraphErrors(n, TriggerAxis, nRatioSlopeInt, 0, nRatioSlopeIntError);
  g_gRatioSlopeInt = new TGraphErrors(n, TriggerAxis, gRatioSlopeInt, 0, gRatioSlopeIntError);

  g_gnRatio->SetName("g_gnRatio");
  g_nRatioSlopeInt->SetName("g_nRatioSlopeInt");
  g_gRatioSlopeInt->SetName("g_gRatioSlopeInt");

}
