#include "readFiss.h"
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <TLine.h>

#include <algorithm>

using namespace std;


void readFiss::Slice()
{
  cout << "\n\n\n In Slice \n\n\n";

  int projectionNum = 40;
  int startBin = 11;
  int numProjectionBins = 250;

  TF1** myfunc = new TF1*[projectionNum];
  TFitResultPtr* Results = new TFitResultPtr[projectionNum];

  TH1D** Projections = new TH1D*[projectionNum];
  TH1D** Derivatives = new TH1D*[projectionNum];
  TGraph* final = new TGraph(projectionNum);

  for(int i = 0; i < projectionNum; ++i)
  {
    Projections[i] = neutronEnergyLOExp->ProjectionY((TString)"projection" + (TString)to_string(i), startBin + i, startBin + i);
    Derivatives[i] = new TH1D((TString)"derivative"+ (TString)to_string(i), "Derivative of Projection;Neutron Light Output [MeVee];Rate of Change",
                                      Projections[i]->GetNbinsX(), Projections[i]->GetXaxis()->GetXmin(), Projections[i]->GetXaxis()->GetXmax());
    myfunc[i] = new TF1((TString)"myfunc" + (TString)to_string(i), "gaus");

    for(int j = 1; j < Projections[i]->GetNbinsX(); ++j)
    {
      Derivatives[i]->SetBinContent(j, abs((Projections[i]->GetBinContent(j+1) - Projections[i]->GetBinContent(j)) /
                            (Projections[i]->GetBinCenter(j+1) - Projections[i]->GetBinCenter(j))));
    }
    Derivatives[i]->Rebin(5); // makes structure more visible
    double left_bound = THRESHOLD;

    for(int k = Derivatives[i]->GetNbinsX(); k > 1; --k)
    {
      if(Derivatives[i]->GetBinContent(k - 1) * 1.2 < Derivatives[i]->GetBinContent(k)
        && Derivatives[i]->GetBinContent(k) > (Derivatives[i]->GetMaximum() / 10))
      {
        left_bound = Derivatives[i]->GetBinCenter(k);
        if(left_bound < THRESHOLD)
        {
          left_bound = THRESHOLD;
        }
        break;
      }
    }

    myfunc[i]->SetParameters(100000, left_bound, 0.25);
    myfunc[i]->SetParLimits(1, max(left_bound - 0.1, 0.1), left_bound + 0.1);
    Results[i] = Derivatives[i]->Fit((TString)"myfunc" + (TString)to_string(i), "B0Q", "", left_bound, 5);
    //cout << i << " " << left_bound << "\n\n";
  }

  writeFile->cd();
  cd_correlated->cd();
  TCanvas** c_Proj = new TCanvas*[projectionNum];
  TCanvas** c_Fit = new TCanvas*[projectionNum];
  for(int r = 0; r < projectionNum; ++r)
  {
    // prepare to draw fit
    TF1 fitDraw("fitDraw", "gaus", 0, 5);
    double mean = myfunc[r]->GetParameter(1);
    fitDraw.SetParameters(myfunc[r]->GetParameter(0), mean, myfunc[r]->GetParameter(2));

    final->SetPoint(r, ((startBin + r) * 0.1) - 0.05, mean);

    // draw projection
    c_Proj[r] = new TCanvas((TString)"Proj" + (TString)to_string(r), "Projection", 800, 400);
    c_Proj[r]->Divide(1, 2);
    c_Proj[r]->cd(1);
    Projections[r]->Draw();

    TLine* line = new TLine(mean, 0, mean, Projections[r]->GetMaximum());
    line->SetLineColor(kRed);
    line->Draw("SAME");

    c_Proj[r]->cd(2);
    Derivatives[r]->Draw();

    c_Proj[r]->Update();
    TPaveStats *stats = (TPaveStats*)Derivatives[r]->GetListOfFunctions()->FindObject("stats");
    stats->SetName("mystats");
    TList* list = stats->GetListOfLines();
    TLatex* mytext = new TLatex(0, 0, (TString)"Fit Mean = " + (TString)to_string(mean));
    mytext->SetTextFont(42);
    mytext->SetTextSize(0.03);
    list->Add(mytext);
    Derivatives[r]->SetStats(0);

    c_Proj[r]->Modified();
    c_Proj[r]->Update();

    fitDraw.Draw("SAME");

    c_Proj[r]->Write();
  }

  TCanvas* c_Final = new TCanvas("Final", "Neutron Energy vs. Neutron LO", 800, 400);
  c_Final->cd();

  final->SetTitle("Neutron Energy vs. Neutron Light Out;Neutron Energy [MeV];Neutron Light Out [MeVee]");
  final->GetXaxis()->SetLimits(0, 5);
  final->GetYaxis()->SetLimits(0, 5);
  final->Draw();

  TF1* finalfit = new TF1("finalfit", "[0]x - [1] * (1-exp([2] * pow(x, [3])))", 0, 5);
  finalfit->FixParameter(3, 1.001);
  final->Fit("finalfit", "B");
  c_Final->Write();

  cout << "\n\n\n Out Of Slice \n\n\n";
}
