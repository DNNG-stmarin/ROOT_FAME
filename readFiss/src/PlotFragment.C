#include "readFiss.h"

#include <TLegend.h>
#include <TCanvas.h>
#include <TRatioPlot.h>
#include <TPaveStats.h>
#include <TProfile2D.h>

using namespace std;

void readFiss::PlotFragmentEmission()
{
  cd_fragment->cd();

  TCanvas* c_FragYield = new TCanvas("c_fragYield","c_fragYield", 1200, 800);
  c_FragYield->Divide(3,1);

  c_FragYield->cd(1);
  TProfile2D* h2_gMassTKE = h3_gMassTKE->Project3DProfile("xy");
  h2_gMassTKE->SetName("h2_gMassTKE");
  h2_gMassTKE->Draw("COLZ");

  c_FragYield->cd(2);
  TProfile2D* h2_nMassTKE = h3_nMassTKE->Project3DProfile("xy");
  h2_nMassTKE->SetName("h2_nMassTKE");
  h2_nMassTKE->Draw("COLZ");

  c_FragYield->cd(3);
  h2_MassTKE->Draw("COLZ");

  c_FragYield->Write();

  TCanvas* c_FragSpec = new TCanvas("c_fragSpec","c_fragSpec", 1200, 800);
  c_FragSpec->Divide(3,1);

  c_FragSpec->cd(1);
  h3_gSpecMassTKE->Draw("COLZ");

  c_FragSpec->cd(2);
  h3_nSpecMassTKE->Draw("COLZ");

  c_FragSpec->cd(3);
  h2_MassTKE->Draw("COLZ");

  c_FragSpec->Write();


}
