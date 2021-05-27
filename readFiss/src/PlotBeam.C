#include "readFiss.h"

#include <TLegend.h>
#include <TCanvas.h>
#include <TRatioPlot.h>
#include <TPaveStats.h>

using namespace std;

void readFiss::PlotDepSubtraction()
{
  cd_beam->cd();
  cout << "Plotting subtracted spectra " << endl;

  TCanvas** c_Alpha = new TCanvas* [NUM_TRIGGERS];

  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    c_Alpha[r] = new TCanvas("AlphaSubtraction_Channel_"+ s_TRIG_NUM, "Fission Alpha Ratios", 1200, 800);
    c_Alpha[r]->Divide(2,2);

    h_fisDep[r]->SetLineColor(kBlue);
    h_alphaDep[r]->SetLineColor(kGreen);
    h_fisSubtract[r]->SetLineColor(kBlack);


    //Draw both fits on general fisDep hisgram
     c_Alpha[r]->cd(1);
     gPad->SetLogy(1);
     h_fisDep[r]->Draw();											//General plot of fisDep
     h_fisSubtract[r]->Draw("SAME");
     f_gauss[r]->Draw("SAME");								//Gaussian fis of fissions
     f_expo[r]->Draw("SAME");								//Exponential fit of alphas

     TLegend *leg = new TLegend(0.9,0.6,0.5,0.75);
     leg->AddEntry("h_fisDep" + s_TRIG_NUM, "Total", "l");
     leg->AddEntry("f_gaus" +  s_TRIG_NUM, "Fission Products", "l");
     leg->AddEntry("f_expo" + s_TRIG_NUM, "Alpha Background", "l");
     leg->Draw();

    //Draw Vertical Line at intersection
     //TLine *intersection = new TLine(0.00623,0,0.00623,100000);	//Define intersections line of gaussian (fissions) and exponential (alphas)
     //intersection->Draw("SAME");

    //Draw other canvas entries
     c_Alpha[r]->cd(2);												//Second canvas assignment
     h_fisSubtract[r]->Draw("HIST");							//Isolated fissions plot
     f_gauss[r]->Draw("SAME");							//Fissions fit
     h_fisSubtract[r]->SetTitle("Fissions without 'Alphas';Pulse Integral (V us); Count Rate");	//Title

     c_Alpha[r]->cd(3);							//Third canvas assignment
     gPad->SetLogy(3);
     h_alphaDep[r]->Draw("HIST");			//Isolated alpha background plot
     f_expo[r]->Draw("SAME");		//Alpha background fit
     h_alphaDep[r]->SetTitle("Alpha spec vs. chan; Pulse Integral (V us); Count Rate");

     // c_Alpha->cd(4);																			//Fourth canvas assignment
     //
     // TLine *beamRangeFis1 = new TLine(FIS_RANGE_LOW,0,FIS_RANGE_LOW,10000);				//Define line indicating lower limit of fissions interval
     // TLine *beamRangeFis2 = new TLine(FIS_RANGE_HIGH,0,FIS_RANGE_HIGH,10000);			//Define line indicating upper limit of fissions interval
     // beamRangeFis1->Draw("SAME");														//Draw lower limit fissions line
     // beamRangeFis2->Draw("SAME");														//Draw upper limit fissions line
     // beamRangeFis1->SetLineColor(kGreen);
     // beamRangeFis2->SetLineColor(kGreen);

     c_Alpha[r]->Write();
  }
}


void readFiss::PlotRatioMult()
{
  cd_beam->cd();
  cout << "Plotting corrected multiplicities " << endl;

  TCanvas** c_depRatio = new TCanvas* [NUM_TRIGGERS];

  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    c_depRatio[r] = new TCanvas("MultiplicityRatio_Channel_" + s_TRIG_NUM, "Neutron/Gamma Multiplicity Comparision to Fission Fractiom", 600, 800);
    c_depRatio[r]->Divide(1,2);

    c_depRatio[r]->cd(1);
    g_fisRatioSelect[r]->Draw();										//Draw segmented energy fission alpha ratios
    p_neutronMultDep[r]->Draw("SAME");								//Draw profile of neutronMult with ratio of fissions to alphas
    p_gammaMultDep[r]->Draw("SAME");									//Draw profile of gammaMult with ratio of fissions to alphas

    g_fisRatioSelect[r]->SetLineColor(kBlack);		//**
    p_neutronMultDep[r]->SetLineColor(kBlue);	//**
    p_gammaMultDep[r]->SetLineColor(kRed);		//**

    TLegend *segLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
    segLeg->AddEntry("fissRatioSelect" + s_TRIG_NUM,"Fission Fraction","l");
    segLeg->AddEntry("p_neutronMultDep" + s_TRIG_NUM,"Mean Neutron Mult","l");
    segLeg->AddEntry("p_gammaMultDep"+ s_TRIG_NUM,"Mean Gamma Mult","l");
    segLeg->Draw();												//Draw Legend

    c_depRatio[r]->cd(2);
    g_neutronMultRatioDep[r]->Draw();
    g_gammaMultRatioDep[r]->Draw("SAME");

    g_neutronMultRatioDep[r]->SetLineColor(kBlue);	//**
    g_gammaMultRatioDep[r]->SetLineColor(kRed);	//**

    TLegend *ratLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
    ratLeg->AddEntry("g_neutronMultRatioDep"+ s_TRIG_NUM,"Mean Fiss Neutron Mult","l");
    ratLeg->AddEntry("g_gammaMultRatioDep"+ s_TRIG_NUM,"Mean Fiss Gamma Mult","l");
    ratLeg->Draw();


    c_depRatio[r]->Write();
  }
}

void readFiss::PlotMultErg()
{
  cd_beam->cd();
  cout << "Plotting incident-dependent multiplicities " << endl;

  TCanvas** c_incMult = new TCanvas* [NUM_TRIGGERS];

  for (int r = 0; r < NUM_TRIGGERS; r++)
	{
    TString s_TRIG_NUM = (TString)to_string(r);

    c_incMult[r] = new TCanvas("MultiplicityRatioErg_Channel_" + s_TRIG_NUM, "Neutron/Gamma Multiplicity Comparision to Fission Fractiom", 600, 800);
    c_incMult[r]->Divide(1,2);

    c_incMult[r]->cd(1);
    g_fisRatioErg[r]->Draw("ALP");
    p_neutronMultErg[r]->Draw("SAME");
    p_gammaMultErg[r]->Draw("SAME");
    p_backNeutronMultErg[r]->Draw("SAME");
    p_backGammaMultErg[r]->Draw("SAME");

    g_fisRatioErg[r]->SetLineColor(kBlack);
    g_fisRatioErg[r]->SetMarkerColor(kBlack);
    g_fisRatioErg[r]->SetMarkerSize(0.5);
    g_fisRatioErg[r]->SetMarkerStyle(21);
    g_fisRatioErg[r]->GetYaxis()->SetRangeUser(0., 1.);

    p_neutronMultErg[r]->SetLineColor(kBlue);

    p_gammaMultErg[r]->SetLineColor(kRed);
    p_backNeutronMultErg[r]->SetLineColor(kBlue);
    p_backNeutronMultErg[r]->SetLineStyle(kDashed);
    p_backGammaMultErg[r]->SetLineColor(kRed);
    p_backGammaMultErg[r]->SetLineStyle(kDashed);

    TLegend *segLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
    segLeg->AddEntry("g_fisRatioErg" + s_TRIG_NUM, "Fission Fraction","l");
    segLeg->AddEntry("p_neutronMultErg" + s_TRIG_NUM,"Mean Neutron Mult","l");
    segLeg->AddEntry("p_gammaMultErg"+ s_TRIG_NUM,"Mean Gamma Mult","l");
    segLeg->AddEntry("p_backNeutronMultErg" + s_TRIG_NUM,"Mean Background Neutron Mult","l");
    segLeg->AddEntry("p_backGammaMultErg"+ s_TRIG_NUM,"Mean Background Gamma Mult","l");
    segLeg->Draw();												//Draw Legend

    c_incMult[r]->cd(2);
    g_nMultErg[r]->Draw("ALP");
    g_gMultErg[r]->Draw("SAME");
    g_nMultBackErg[r]->Draw("SAME");
    g_gMultBackErg[r]->Draw("SAME");

    g_nMultErg[r]->SetLineColor(kBlue);
    g_nMultErg[r]->SetMarkerColor(kBlue);
    g_nMultErg[r]->SetMarkerSize(0.5);
    g_nMultErg[r]->SetMarkerStyle(21);

    g_gMultErg[r]->SetLineColor(kRed);
    g_gMultErg[r]->SetMarkerColor(kRed);
    g_gMultErg[r]->SetMarkerSize(0.5);
    g_gMultErg[r]->SetMarkerStyle(22);

    g_nMultBackErg[r]->SetLineColor(kBlue);
    g_nMultBackErg[r]->SetLineStyle(kDashed);
    g_nMultBackErg[r]->SetMarkerColor(kBlue);
    g_nMultBackErg[r]->SetMarkerSize(0.5);
    g_nMultBackErg[r]->SetMarkerStyle(21);

    g_gMultBackErg[r]->SetLineColor(kRed);
    g_gMultBackErg[r]->SetLineStyle(kDashed);
    g_gMultBackErg[r]->SetMarkerColor(kRed);
    g_gMultBackErg[r]->SetMarkerSize(0.5);
    g_gMultBackErg[r]->SetMarkerStyle(22);

    TLegend *ratLeg = new TLegend(0.4,0.7,0.15,0.88);			//Define Legend
    ratLeg->AddEntry("g_nMultErg" + s_TRIG_NUM, "Fission Neutrons","l");
    ratLeg->AddEntry("g_gMultErg" + s_TRIG_NUM,"Fission Gammas","l");
    ratLeg->AddEntry("g_nMultBackErg"+ s_TRIG_NUM,"Background Neutrons","l");
    ratLeg->AddEntry("g_gMultBackErg" + s_TRIG_NUM,"Background Gammas","l");
    ratLeg->Draw();												//Draw Legend

    c_incMult[r]->Write();
  }
}
