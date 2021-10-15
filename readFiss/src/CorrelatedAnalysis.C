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

    // gamma-gamma
  for(int i = 1; i <= photonSinglesExp->GetNbinsX(); ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      photonSinglesMat->SetBinContent(i, j, (photonSinglesExp->GetBinContent(i) * photonSinglesExp->GetBinContent(j)) / photonSinglesExp->Integral());
    }
  }
    // n-gamma
  for(int i = 1; i <= neutronSinglesExp->GetNbinsX(); ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      neutronPhotonSinglesMat->SetBinContent(i, j, (neutronSinglesExp->GetBinContent(i) * photonSinglesExp->GetBinContent(j)) / neutronSinglesExp->Integral());
    }
  }
    // n-n background
    for(int i = 1; i <= neutronSinglesBack->GetNbinsX(); ++i)
    {
      for(int j = 1; j < i; ++j)
      {
        neutronBackSinglesMat->SetBinContent(i, j, (neutronSinglesBack->GetBinContent(i) * neutronSinglesBack->GetBinContent(j)) / neutronSinglesBack->Integral());
      }
    }
    // gamma-gamma background
  for(int i = 1; i <= photonSinglesBack->GetNbinsX(); ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      photonBackSinglesMat->SetBinContent(i, j, (photonSinglesBack->GetBinContent(i) * photonSinglesBack->GetBinContent(j)) / photonSinglesBack->Integral());
    }
  }
    // n-gamma background
  for(int i = 1; i <= neutronSinglesBack->GetNbinsX(); ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      neutronPhotonBackSinglesMat->SetBinContent(i, j, (neutronSinglesBack->GetBinContent(i) * photonSinglesBack->GetBinContent(j)) / neutronSinglesBack->Integral());
    }
  }


  // scale doubles matrix by eff matrix
  neutronScaledDoubles = (TH2I*)neutronDoublesMat->Clone();
  neutronScaledDoubles->SetName("neutronScaledDoubles");
  neutronScaledDoubles->Divide(neutronSinglesMat);

  photonScaledDoubles = (TH2I*)photonDoublesMat->Clone();
  photonScaledDoubles->SetName("photonScaledDoubles");
  photonScaledDoubles->Divide(photonSinglesMat);

  neutronPhotonScaledDoubles = (TH2I*)neutronPhotonDoublesMat->Clone();
  neutronPhotonScaledDoubles->SetName("neutronPhotonScaledDoubles");
  neutronPhotonScaledDoubles->Divide(neutronPhotonSinglesMat);

    // Background
  neutronBackScaledDoubles = (TH2I*)neutronBackDoublesMat->Clone();
  neutronBackScaledDoubles->SetName("neutronBackScaledDoubles");
  neutronBackScaledDoubles->Divide(neutronBackSinglesMat);

  photonBackScaledDoubles = (TH2I*)photonBackDoublesMat->Clone();
  photonBackScaledDoubles->SetName("photonBackScaledDoubles");
  photonBackScaledDoubles->Divide(photonBackSinglesMat);

  neutronPhotonBackScaledDoubles = (TH2I*)neutronPhotonBackDoublesMat->Clone();
  neutronPhotonBackScaledDoubles->SetName("neutronPhotonBackScaledDoubles");
  neutronPhotonBackScaledDoubles->Divide(neutronPhotonBackSinglesMat);

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
        continue;
      }

      a_angles[n_points] = angles[i-1][j-1];
      a_counts[n_points] = neutronScaledDoubles->GetBinContent(i, j);
      a_errors[n_points] = neutronScaledDoubles->GetBinError(i, j);
      n_points++;
    }
  }

    // gamma-gamma
  double* p_angles = new double[arraySize];
  double* p_counts = new double[arraySize];
  double* p_errors = new double[arraySize];
  int p_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(photonScaledDoubles->GetBinContent(i, j) == 0)
      {
        continue;
      }

      p_angles[p_points] = angles[i-1][j-1];
      p_counts[p_points] = photonScaledDoubles->GetBinContent(i, j);
      p_errors[p_points] = photonScaledDoubles->GetBinError(i, j);
      p_points++;
    }
  }
    // n-gamma
  double* np_angles = new double[arraySize];
  double* np_counts = new double[arraySize];
  double* np_errors = new double[arraySize];
  int np_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(neutronPhotonScaledDoubles->GetBinContent(i, j) == 0)
      {
        continue;
      }

      np_angles[np_points] = angles[i-1][j-1];
      np_counts[np_points] = neutronPhotonScaledDoubles->GetBinContent(i, j);
      np_errors[np_points] = neutronPhotonScaledDoubles->GetBinError(i, j);
      np_points++;
    }
  }
    // n-n background
  double* nB_angles = new double[arraySize];
  double* nB_counts = new double[arraySize];
  double* nB_errors = new double[arraySize];
  int nB_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(neutronBackScaledDoubles->GetBinContent(i, j) == 0)
      {
        continue;
      }

      nB_angles[nB_points] = angles[i-1][j-1];
      nB_counts[nB_points] = neutronBackScaledDoubles->GetBinContent(i, j);
      nB_errors[nB_points] = neutronBackScaledDoubles->GetBinError(i, j);
      nB_points++;
    }
  }
    // gamma-gamma background
  double* pB_angles = new double[arraySize];
  double* pB_counts = new double[arraySize];
  double* pB_errors = new double[arraySize];
  int pB_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(photonBackScaledDoubles->GetBinContent(i, j) == 0)
      {
        continue;
      }

      pB_angles[pB_points] = angles[i-1][j-1];
      pB_counts[pB_points] = photonBackScaledDoubles->GetBinContent(i, j);
      pB_errors[pB_points] = photonBackScaledDoubles->GetBinError(i, j);
      pB_points++;
    }
  }
    // n-gamma background
  double* npB_angles = new double[arraySize];
  double* npB_counts = new double[arraySize];
  double* npB_errors = new double[arraySize];
  int npB_points = 0;
  for(int i = 1; i <= NUM_DETECTORS; ++i)
  {
    for(int j = 1; j < i; ++j)
    {
      if(neutronPhotonBackScaledDoubles->GetBinContent(i, j) == 0)
      {
        continue;
      }

      npB_angles[npB_points] = angles[i-1][j-1];
      npB_counts[npB_points] = neutronPhotonBackScaledDoubles->GetBinContent(i, j);
      npB_errors[npB_points] = neutronPhotonBackScaledDoubles->GetBinError(i, j);
      npB_points++;
    }
  }

  // make graphs
  neutronAngleCorr = new TGraphErrors(n_points, a_angles, a_counts, 0, a_errors);
  neutronAngleCorr->Sort();
  neutronAngleCorr->SetName("neutronAngleCorr");
  neutronAngleCorr->SetTitle("Neutron Double Angle Correlations;Cos T;Counts");
  neutronAngleCorr->GetXaxis()->SetLimits(-1, 1);
  //neutronAngleCorr->GetYaxis()->SetRangeUser(0, ?);

  photonAngleCorr = new TGraphErrors(p_points, p_angles, p_counts, 0, p_errors);
  photonAngleCorr->Sort();
  photonAngleCorr->SetName("photonAngleCorr");
  photonAngleCorr->SetTitle("Photon Double Angle Correlations;Cos T;Counts");
  photonAngleCorr->GetXaxis()->SetLimits(-1, 1);

  neutronPhotonAngleCorr = new TGraphErrors(np_points, np_angles, np_counts, 0, np_errors);
  neutronPhotonAngleCorr->Sort();
  neutronPhotonAngleCorr->SetName("neutronPhotonAngleCorr");
  neutronPhotonAngleCorr->SetTitle("Neutron Photon Angle Correlations;Cos T;Counts");
  neutronPhotonAngleCorr->GetXaxis()->SetLimits(-1, 1);

    // Background
  neutronBackAngleCorr = new TGraphErrors(nB_points, nB_angles, nB_counts, 0, nB_errors);
  neutronBackAngleCorr->Sort();
  neutronBackAngleCorr->SetName("neutronBackAngleCorr");
  neutronBackAngleCorr->SetTitle("Background Neutron Double Angle Correlations;Cos T;Counts");
  neutronBackAngleCorr->GetXaxis()->SetLimits(-1, 1);

  photonBackAngleCorr = new TGraphErrors(pB_points, pB_angles, pB_counts, 0, pB_errors);
  photonBackAngleCorr->Sort();
  photonBackAngleCorr->SetName("photonBackAngleCorr");
  photonBackAngleCorr->SetTitle("Background Photon Double Angle Correlations;Cos T;Counts");
  photonBackAngleCorr->GetXaxis()->SetLimits(-1, 1);

  neutronPhotonBackAngleCorr = new TGraphErrors(npB_points, npB_angles, npB_counts, 0, npB_errors);
  neutronPhotonBackAngleCorr->Sort();
  neutronPhotonBackAngleCorr->SetName("neutronPhotonBackAngleCorr");
  neutronPhotonBackAngleCorr->SetTitle("Background Neutron Photon Angle Correlations;Cos T;Counts");
  neutronPhotonBackAngleCorr->GetXaxis()->SetLimits(-1, 1);

  // make average lines
  int numAvgBins = 20;
  int numBin = 0;
  int numPoints = 0;
  double xAvg[n_points];
  double yAvg[n_points];

  neutronAngleCorrAvg = new TGraphErrors(numAvgBins);
  photonAngleCorrAvg = new TGraphErrors(numAvgBins);
  neutronPhotonAngleCorrAvg = new TGraphErrors(numAvgBins);

  neutronBackAngleCorrAvg = new TGraphErrors(numAvgBins);
  photonBackAngleCorrAvg = new TGraphErrors(numAvgBins);
  neutronPhotonBackAngleCorrAvg = new TGraphErrors(numAvgBins);


  for(int i = 0; i < n_points; ++i)
  {
    double rightBound = (2.0 / numAvgBins) * (numBin + 1);
    if(((neutronAngleCorr->GetPointX(i) + 1) >= rightBound) || i == n_points - 1)
    {
      neutronAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg), TMath::Mean(numPoints, yAvg));
      neutronAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg), TMath::StdDev(numPoints, yAvg));

      for(int j = 0; j < numPoints; ++j)
      {
        xAvg[j] = 0;
        yAvg[j] = 0;
      }
      numPoints = 0;
      ++numBin;
    }
    xAvg[numPoints] = neutronAngleCorr->GetPointX(i);
    yAvg[numPoints] = neutronAngleCorr->GetPointY(i);
    ++numPoints;
  }

    // gamma-gamma
  numBin = 0;
  numPoints = 0;
  double xAvg_p[p_points];
  double yAvg_p[p_points];
  for(int i = 0; i < p_points; ++i)
  {
    double rightBound = (2.0 / numAvgBins) * (numBin + 1);
    if(((photonAngleCorr->GetPointX(i) + 1) >= rightBound) || i == p_points - 1)
    {
      photonAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg_p), TMath::Mean(numPoints, yAvg_p));
      photonAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg_p), TMath::StdDev(numPoints, yAvg_p));

      for(int j = 0; j < numPoints; ++j)
      {
        xAvg_p[j] = 0;
        yAvg_p[j] = 0;
      }
      numPoints = 0;
      ++numBin;
    }
    xAvg_p[numPoints] = photonAngleCorr->GetPointX(i);
    yAvg_p[numPoints] = photonAngleCorr->GetPointY(i);
    ++numPoints;
  }
    // n-gamma
    numBin = 0;
    numPoints = 0;
    double xAvg_np[np_points];
    double yAvg_np[np_points];
    for(int i = 0; i < np_points; ++i)
    {
      double rightBound = (2.0 / numAvgBins) * (numBin + 1);
      if(((neutronPhotonAngleCorr->GetPointX(i) + 1) >= rightBound) || i == np_points - 1)
      {
        neutronPhotonAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg_np), TMath::Mean(numPoints, yAvg_np));
        neutronPhotonAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg_np), TMath::StdDev(numPoints, yAvg_np));

        for(int j = 0; j < numPoints; ++j)
        {
          xAvg_np[j] = 0;
          yAvg_np[j] = 0;
        }
        numPoints = 0;
        ++numBin;
      }
      xAvg_np[numPoints] = neutronPhotonAngleCorr->GetPointX(i);
      yAvg_np[numPoints] = neutronPhotonAngleCorr->GetPointY(i);
      ++numPoints;
    }
    // n-n background
    numBin = 0;
    numPoints = 0;
    double xAvg_nB[nB_points];
    double yAvg_nB[nB_points];
    for(int i = 0; i < nB_points; ++i)
    {
      double rightBound = (2.0 / numAvgBins) * (numBin + 1);
      if(((neutronBackAngleCorr->GetPointX(i) + 1) >= rightBound) || i == nB_points - 1)
      {
        neutronBackAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg_nB), TMath::Mean(numPoints, yAvg_nB));
        neutronBackAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg_nB), TMath::StdDev(numPoints, yAvg_nB));

        for(int j = 0; j < numPoints; ++j)
        {
          xAvg_nB[j] = 0;
          yAvg_nB[j] = 0;
        }
        numPoints = 0;
        ++numBin;
      }
      xAvg_nB[numPoints] = neutronBackAngleCorr->GetPointX(i);
      yAvg_nB[numPoints] = neutronBackAngleCorr->GetPointY(i);
      ++numPoints;
    }
    // gamma-gamma background
  numBin = 0;
  numPoints = 0;
  double xAvg_pB[pB_points];
  double yAvg_pB[pB_points];
  for(int i = 0; i < pB_points; ++i)
  {
    double rightBound = (2.0 / numAvgBins) * (numBin + 1);
    if(((photonBackAngleCorr->GetPointX(i) + 1) >= rightBound) || i == pB_points - 1)
    {
      photonBackAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg_pB), TMath::Mean(numPoints, yAvg_pB));
      photonBackAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg_pB), TMath::StdDev(numPoints, yAvg_pB));

      for(int j = 0; j < numPoints; ++j)
      {
        xAvg_pB[j] = 0;
        yAvg_pB[j] = 0;
      }
      numPoints = 0;
      ++numBin;
    }
    xAvg_pB[numPoints] = photonBackAngleCorr->GetPointX(i);
    yAvg_pB[numPoints] = photonBackAngleCorr->GetPointY(i);
    ++numPoints;
  }
    // n-gamma background
  numBin = 0;
  numPoints = 0;
  double xAvg_npB[npB_points];
  double yAvg_npB[npB_points];
  for(int i = 0; i < npB_points; ++i)
  {
    double rightBound = (2.0 / numAvgBins) * (numBin + 1);
    if(((neutronPhotonBackAngleCorr->GetPointX(i) + 1) >= rightBound) || i == npB_points - 1)
    {
      neutronPhotonBackAngleCorrAvg->SetPoint(numBin, TMath::Mean(numPoints, xAvg_npB), TMath::Mean(numPoints, yAvg_npB));
      neutronPhotonBackAngleCorrAvg->SetPointError(numBin, TMath::StdDev(numPoints, xAvg_npB), TMath::StdDev(numPoints, yAvg_npB));

      for(int j = 0; j < numPoints; ++j)
      {
        xAvg_npB[j] = 0;
        yAvg_npB[j] = 0;
      }
      numPoints = 0;
      ++numBin;
    }
    xAvg_npB[numPoints] = neutronPhotonBackAngleCorr->GetPointX(i);
    yAvg_npB[numPoints] = neutronPhotonBackAngleCorr->GetPointY(i);
    ++numPoints;
  }

  cd_AngleCorr = cd_correlated->mkdir("AngleCorr");
}
