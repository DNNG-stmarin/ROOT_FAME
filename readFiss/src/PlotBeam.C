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

    //Draw both fits on general fisDep hisgram
     c_Alpha[r]->cd(1);
     gPad->SetLogy(1);
     h_fisDep[r]->Draw();											//General plot of fisDep
     h_fisSubtract[r]->Draw("SAME");
     h_fisSubtract[r]->SetLineColor(kBlack);
     f_gauss[r]->Draw("SAME");								//Gaussian fis of fissions
     f_expo[r]->Draw("SAME");								//Exponential fit of alphas

     // TLegend *leg = new TLegend(0.9,0.6,0.5,0.75);
     // leg->AddEntry("h_fisDep","fisDep","l");
     // leg->AddEntry("f_gausProducts","Fission Products","l");
     // leg->AddEntry("f_expoBackground","Alpha Background","l");
     // leg->Draw();

    //Draw Vertical Line at intersection
     //TLine *intersection = new TLine(0.00623,0,0.00623,100000);	//Define intersections line of gaussian (fissions) and exponential (alphas)
     //intersection->Draw("SAME");

    //Draw other canvas entries
     c_Alpha[r]->cd(2);												//Second canvas assignment
     h_fisSubtract[r]->Draw("HIST");							//Isolated fissions plot
     h_fisSubtract[r]->SetTitle("Fissions without 'Alphas';Pulse Integral (V us); Count Rate");	//Title
     f_gauss[r]->Draw("SAME");							//Fissions fit

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
  // c_depRatio = new TCanvas("MultiplicityRatio_Channel_"+s_TRIGGER_CHANNEL, "Neutron/Gamma Multiplicity Comparision to Fission Alpha Ratios", 600, 800);
  // c_depRatio->Divide(1,2);

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
