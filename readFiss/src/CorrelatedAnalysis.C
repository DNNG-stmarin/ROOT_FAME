#include "readFiss.h"
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <TLine.h>

#include <algorithm>
#include <vector>
#include <utility>

using namespace std;


void readFiss::CorrAnalysis()
{
  cout << "Generating correlated neutron Energy/LO fits" << endl;
  Slice();

  cout << "Generating angular correlations" << endl;
  AngCorr();
}

void readFiss::Slice()
{
  int projectionNum = 30;
  int startBin = 1;

  // choose where to start and stop
  bool found = false;
  for(int i = 1; i < neutronEnergyLOExp->GetNbinsX(); ++i)
  {
    for(int j = 1; j < neutronEnergyLOExp->GetNbinsY(); ++j)
    {
      if(!found && (neutronEnergyLOExp->GetBinContent(i, j) > 0) &&
      (neutronEnergyLOExp->GetBinContent(i, j+1) > neutronEnergyLOExp->GetBinContent(i, j)) &&
      (neutronEnergyLOExp->Integral(i, i, 1, neutronEnergyLOExp->GetNbinsY()) > (neutronEnergyLOExp->Integral() / 100)))
      {
        startBin = i;
        found = true;
        break;
      }
    }
    if(found && (neutronEnergyLOExp->Integral(i, i, 1, neutronEnergyLOExp->GetNbinsY()) < (neutronEnergyLOExp->Integral() / 100)))
    {
      projectionNum = i - startBin;
      break;
    }
  }

  TF1** myfunc = new TF1*[projectionNum];
  TFitResultPtr* Results = new TFitResultPtr[projectionNum]; // currently unused

  TH1D** Projections = new TH1D*[projectionNum];
  TH1D** Derivatives = new TH1D*[projectionNum];
  double* left_bound = new double[projectionNum];
  TH1D** Seconds = new TH1D*[projectionNum];
  TGraph* final = new TGraph(projectionNum);

  for(int i = 0; i < projectionNum; ++i)
  {
    Projections[i] = neutronEnergyLOExp->ProjectionY((TString)"projection" + (TString)to_string(i), startBin + i, startBin + i);
    Projections[i]->Rebin(5);
    Derivatives[i] = new TH1D((TString)"derivative"+ (TString)to_string(i), "Derivative of Projection;Neutron Light Output [MeVee];Rate of Change",
                                      Projections[i]->GetNbinsX(), Projections[i]->GetXaxis()->GetXmin(), Projections[i]->GetXaxis()->GetXmax());
    Seconds[i] = new TH1D((TString)"2nd_derivative" + (TString)to_string(i), "2nd Derivative of Projection;Neutron Light Output [MeVee];Rate of Change",
                                      Derivatives[i]->GetNbinsX(), Derivatives[i]->GetXaxis()->GetXmin(), Derivatives[i]->GetXaxis()->GetXmax());
    myfunc[i] = new TF1((TString)"myfunc" + (TString)to_string(i), "gaus");

    for(int j = 1; j <= Projections[i]->GetNbinsX(); ++j)
    {
      Derivatives[i]->SetBinContent(j, abs((Projections[i]->GetBinContent(j+1) - Projections[i]->GetBinContent(j)) /
                            (Projections[i]->GetBinCenter(j+1) - Projections[i]->GetBinCenter(j))));
    }

    for(int k = 1; k <= Derivatives[i]->GetNbinsX(); ++k)
    {
      Seconds[i]->SetBinContent(k, (Derivatives[i]->GetBinContent(k+1) - Derivatives[i]->GetBinContent(k)) /
                              (Derivatives[i]->GetBinCenter(k+1) - Derivatives[i]->GetBinCenter(k)));
    }
    Seconds[i]->Rebin(2);

    //Derivatives[i]->Rebin(5); // makes structure more visible
    left_bound[i] = THRESHOLD;

    for(int k = Seconds[i]->GetNbinsX(); k >= 1; --k)
    {
      if(Seconds[i]->GetBinContent(k - 1) > 0 &&
         Seconds[i]->GetBinContent(k) < 0 &&
        (Projections[i]->GetBinContent(2 * k) > (Projections[i]->GetMaximum() / 10)))
        // (Seconds[i]->GetBinContent(k - 1) > (1000 * (1 - Derivatives[i]->GetMean(1))) &&
        //  Seconds[i]->GetBinContent(k) < (-1000 * (1 - Derivatives[i]->GetMean(1)))))
      {
        left_bound[i] = Seconds[i]->GetBinCenter(k);
        break;
      }
    }


    myfunc[i]->SetParameters(100000, left_bound[i], 0.25);
    double leftLimit;
    if(left_bound[i] - 0.1 > THRESHOLD)
    {
      leftLimit = left_bound[i] - 0.1;
    }
    else
    {
      leftLimit = THRESHOLD - 0.1;
    }
    myfunc[i]->SetParLimits(1, leftLimit, left_bound[i] + 0.1);
    Results[i] = Derivatives[i]->Fit((TString)"myfunc" + (TString)to_string(i), "B0QS", "", left_bound[i], 5);

  }

  writeFile->cd();
  cd_correlated->cd();
  cd_projs = cd_correlated->mkdir("ErgDepLO");
  cd_projs->cd();
  TCanvas** c_Proj = new TCanvas*[projectionNum];

  for(int r = 0; r < projectionNum; ++r)
  {
    // prepare to draw fit
    TF1 fitDraw("fitDraw", "gaus", 0, 5);
    double mean = myfunc[r]->GetParameter(1);
    fitDraw.SetParameters(myfunc[r]->GetParameter(0), mean, myfunc[r]->GetParameter(2));

    final->SetPoint(r, ((startBin + r) * 0.1) - 0.05, mean);

    // draw projection
    c_Proj[r] = new TCanvas((TString)"Proj" + (TString)to_string(r), "Projection", 800, 400);
    c_Proj[r]->Divide(1, 3);

    c_Proj[r]->cd(1);
    Projections[r]->Draw();

    TLine* line = new TLine(mean, 0, mean, Projections[r]->GetMaximum());
    line->SetLineColor(kRed);
    line->Draw("SAME");

    c_Proj[r]->cd(2);
    Derivatives[r]->Draw();

    TLine* line2 = new TLine(left_bound[r], 0, left_bound[r], Derivatives[r]->GetMaximum());
    line2->SetLineColor(kBlack);
    line2->Draw("SAME");

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

    c_Proj[r]->cd(3);
    Seconds[r]->Draw();

    c_Proj[r]->Write();
  }

  TCanvas* c_Final = new TCanvas("ErgDepLO", "Neutron Energy vs. Neutron LO", 800, 400);
  c_Final->cd();

  final->SetTitle("Neutron Energy vs. Neutron Light Out;Neutron Energy [MeV];Neutron Light Out [MeVee]");
  final->GetXaxis()->SetLimits(0, 5);
  final->GetYaxis()->SetRangeUser(0, 3);
  final->Draw();

  TF1* finalfit = new TF1("finalfit", "[0] * x + [1] * (1-exp([2] * pow(x, [3])))", 0, 5);
  finalfit->SetParameters(0.8, -2.5, -0.3, 1.001);
  finalfit->SetParLimits(0, 0.5, 1);
  finalfit->SetParLimits(1, -5, 0);
  // finalfit->FixParameter(0, 0.817);
  // finalfit->FixParameter(1, -2.63);
  // finalfit->FixParameter(2, -0.297);
  finalfit->FixParameter(3, 1.001);
  final->Fit("finalfit", "BQ");

  c_Final->Write();
}

void readFiss::AngCorr()
{
  // computing efficiency matrix
  for(int i = 1; i <= neutronSinglesExp->GetNbinsX(); ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      neutronSinglesMat->SetBinContent(i, j, (neutronSinglesExp->GetBinContent(i) * neutronSinglesExp->GetBinContent(j)) / neutronSinglesExp->Integral());
    }
  }

  // scale doubles matrix by eff matrix
  neutronScaledDoubles = (TH2I*)neutronDoublesMat->Clone();
  neutronScaledDoubles->SetName("neutronScaledDoubles");
  neutronScaledDoubles->Divide(neutronSinglesMat);

  // make arrays of angles and counts
  int arraySize = pow(NUM_DETECTORS, 2);
  double* a_angles = new double[arraySize];
  double* a_counts = new double[arraySize];
  double* a_errors = new double[arraySize];
  int n_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(neutronScaledDoubles->GetBinContent(i, j) == 0)
      {
        cout << "Ignoring point " << i << " " << j << endl;
        continue;
      }
      cout << neutronScaledDoubles->GetBinContent(i, j) << " " << i << " " << j << endl;
      a_angles[n_points] = angles[i-1][j-1];
      a_counts[n_points] = neutronScaledDoubles->GetBinContent(i, j);
      a_errors[n_points] = neutronScaledDoubles->GetBinError(i, j);
      n_points++;
    }
  }

  // make graph
  neutronAngleCorr = new TGraphErrors(n_points, a_angles, a_counts, 0, a_errors);
  neutronAngleCorr->Sort();
  neutronAngleCorr->SetName("neutronAngleCorr");
  neutronAngleCorr->SetTitle("Neutron Double Angle Correlations;Cos T;Counts");
  neutronAngleCorr->GetXaxis()->SetLimits(-1, 1);
  //neutronAngleCorr->GetYaxis()->SetRangeUser(0, ?);

  // make average line
  int numAvgBins = 20;
  int numBin = 0;
  int numPoints = 0;
  double xAvg = 0;
  double yAvg = 0;

  neutronAngleCorrAvg = new TGraph();

  for(int i = 0; i < n_points; ++i)
  {
    double rightBound = (2.0 / numAvgBins) * (numBin + 1);
    if(((neutronAngleCorr->GetPointX(i) + 1) >= rightBound) || i == n_points - 1)
    {
      xAvg /= numPoints;
      yAvg /= numPoints;
      neutronAngleCorrAvg->AddPoint(xAvg, yAvg);

      numPoints = 0;
      xAvg = 0;
      yAvg = 0;
      ++numBin;
    }
    xAvg += neutronAngleCorr->GetPointX(i);
    yAvg += neutronAngleCorr->GetPointY(i);
    ++numPoints;
  }

  cd_AngleCorr = cd_correlated->mkdir("AngleCorr");
}
