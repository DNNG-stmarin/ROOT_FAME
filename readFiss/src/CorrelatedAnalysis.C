#include "readFiss.h"
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TCanvas.h>

using namespace std;


void readFiss::Slice()
{
  cout << "\n\n\n In Slice \n\n\n";
  TF1* myfunc = new TF1("myfunc", "gaus");




  int projectionNum = 40;
  int startBin = 11;
  int numProjectionBins = 250;


  TH1D** Projections = new TH1D*[projectionNum];
  TH1D** Derivatives = new TH1D*[projectionNum];
  for(int i = 0; i < projectionNum; ++i)
  {
    Projections[i] = neutronEnergyLOExp->ProjectionY((TString)"projection" + (TString)to_string(i), startBin + i, startBin + i);
    Derivatives[i] = new TH1D((TString)"derivative"+ (TString)to_string(i), "Derivative of Projection;Neutron Energy [MeV];Rate of Change",
                                      Projections[i]->GetNbinsX(), Projections[i]->GetXaxis()->GetXmin(), Projections[i]->GetXaxis()->GetXmax());

    for(int j = 1; j < Projections[i]->GetNbinsX(); ++j)
    {
      //cout << Projections[i]->GetBinContent(j+1) << " " << Projections[i]->GetBinContent(j) << endl;
      Derivatives[i]->SetBinContent(j, (Projections[i]->GetBinContent(j+1) - Projections[i]->GetBinContent(j)) /
                            (Projections[i]->GetBinCenter(j+1) - Projections[i]->GetBinCenter(j)));
    }
    myfunc->SetParameters(); // set parameters based on derivative - try to find peak numerically
    Derivatives[i]->Fit("myfunc"); // dont fit the whole thing want to fit in the right part
    // use TFitResultPtr
  }

  writeFile->cd();
  cd_correlated->cd();
  TCanvas** c_Proj = new TCanvas*[projectionNum];
  TCanvas** c_Fit = new TCanvas*[projectionNum];
  for(int r = 0; r < projectionNum; ++r)
  {
    c_Proj[r] = new TCanvas((TString)"Proj" + (TString)to_string(r), "Projection", 800, 400);
    c_Proj[r]->cd();
    Projections[r]->Draw();
    c_Proj[r]->Write();

    c_Fit[r] = new TCanvas((TString)"Fit" + (TString)to_string(r), "Derivative Fit", 800, 400);
    c_Fit[r]->cd();
    Derivatives[r]->Draw();
    c_Fit[r]->Write();
  }
  TGraph* final = new TGraph(numProjectionBins);



  TF1* myfunc = new TF1("myfunc", "gaus");
  // myfunc->SetParameters(1.5e+08, -4.73, 1.05);
  // myfunc->SetParLimits(0, 1e+08, 2e+08);
  // myfunc->SetParLimits(1, -5, -4);
  // myfunc->SetParLimits(2, 1, 1.1);
  TH1D* Projection = neutronEnergyLOExp->ProjectionY((TString)"Projection " + "1", 11, 11);
  Projection->Fit("myfunc","R");
  //TF1* myfunc = Projection->GetFunction("gaus");

  //TFitResultPtr r = neutronEnergyLOExp->ProjectionY((TString)"Projection " + "1", 1, 2)->Fit("gaus", "S");

  cout << "\n\n";

  //r->Print();

  writeFile->cd();
  cd_correlated->cd();

  TCanvas* c_Slice = new TCanvas("cSlice", "Fit?", 800, 400);
  c_Slice->cd();

  myfunc->Draw();
  c_Slice->Write();
  Projection->Write();

  cout << myfunc->GetParameter(0) << " "  << myfunc->GetParameter(1) << " " << myfunc->GetParameter(2) << endl;

  cout << "\n\n\n Out Of Slice \n\n\n";
}
