#include "readFiss.h"
#include <TLegend.h>
#include <TCanvas.h>
#include <TRatioPlot.h>
#include <TPaveStats.h>
#include <TText.h>

#include <iostream>
#include <fstream>
#include <sstream>

/*
  ___                   _               _
 | __|_ ___ __  ___ _ _(_)_ __  ___ _ _| |_
 | _|\ \ / '_ \/ -_) '_| | '  \/ -_) ' \  _|
 |___/_\_\ .__/\___|_| |_|_|_|_\___|_||_\__|
         |_|
*/
using namespace std;

void readFiss::PlotAll()
{
  PlotLightOut();
  PlotTof();
  PlotErg();
  PlotMult();
  PlotPSD();
  PlotSingles();
  PlotMultCor();
  PlotMultLO();
}

void readFiss::CompareAll()
{
  cout << "Beginning comparison with simulation. Exp/Sim = " << ((double)expEntries)/((double)simEntries) << ". " << endl;

  CompareLightOut();
  CompareTof();
  CompareErg();
  CompareMult();
  ComparePSD();
  CompareSingles();
}

void readFiss::PlotLightOut()
{
    writeFile->cd();
    cd_basics->cd();
    cout << "Plotting Light Output." << endl;

    //create a canvas
    TCanvas* c_LO = new TCanvas("cLO", "Light Output", 800, 400);
    // cLO->Divide(1,2);

    c_LO->cd();

    photonLightOutputExp->SetLineColor(kRed);
    photonLightOutputExp->SetStats(0);
    photonLightOutputExp->Draw();

    photonLightOutputBack->SetLineColor(kRed);
    photonLightOutputBack->SetLineStyle(kDashed);
    photonLightOutputBack->SetStats(0);
    photonLightOutputBack->Draw("SAME");

    neutronLightOutputExp->SetLineColor(kBlue);
    neutronLightOutputExp->SetStats(0);
    neutronLightOutputExp->Draw("SAME");

    neutronLightOutputBack->SetLineColor(kBlue);
    neutronLightOutputBack->SetLineStyle(kDashed);
    neutronLightOutputBack->SetStats(0);
    neutronLightOutputBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronLightOutputExp", "Experimental Neutron");
    legend->AddEntry("photonLightOutputExp", "Experimental Photon");
    legend->AddEntry("neutronLightOutputBack", "Background Neutron");
    legend->AddEntry("photonLightOutputBack", "Background Photon");
    legend->Draw("SAME");

    c_LO->Write();
    //c_LO->SaveAs("fig/LightOutput.eps");
}

void readFiss::PlotTof()
{

    writeFile->cd();
    cd_basics->cd();
    cout << "Plotting Time of Flight." << endl;

    //create a canvas
    TCanvas* c_ToF = new TCanvas("cToF", "Time of Flight", 800, 400);
    // cLO->Divide(1,2);

    c_ToF->cd();

    photonTofExp->SetLineColor(kRed);
    photonTofExp->SetStats(0);
    photonTofExp->Draw();

    photonTofBack->SetLineColor(kRed);
    photonTofBack->SetLineStyle(kDashed);
    photonTofBack->SetStats(0);
    photonTofBack->Draw("SAME");

    neutronTofExp->SetLineColor(kBlue);
    neutronTofExp->SetStats(0);
    neutronTofExp->Draw("SAME");

    neutronTofBack->SetLineColor(kBlue);
    neutronTofBack->SetLineStyle(kDashed);
    neutronTofBack->SetStats(0);
    neutronTofBack->Draw("SAME");



    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronTofExp", "Experimental Neutron");
    legend->AddEntry("photonTofExp", "Experimental Photon");
    legend->AddEntry("neutronTofBack", "Background Neutron");
    legend->AddEntry("photonTofBack", "Background Photon");
    legend->Draw("SAME");

    c_ToF->Write();
    //c_ToF->SaveAs("fig/TimeOfFlight.eps");
}

void readFiss::PlotErg()
{

    writeFile->cd();
    cd_basics->cd();
    cout << "Plotting Neutron Energy." << endl;

    //create a canvas
    TCanvas* c_Erg = new TCanvas("cErg", "ToF Energy", 800, 400);
    // cLO->Divide(1,2);

    c_Erg->cd();
    neutronEnergyExp->SetLineColor(kBlue);
    neutronEnergyExp->SetStats(0);
    neutronEnergyExp->Draw();

    neutronEnergyBack->SetLineColor(kBlue);
    neutronEnergyBack->SetLineStyle(kDashed);
    neutronEnergyBack->SetStats(0);
    neutronEnergyBack->Draw("SAME");


    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronEnergyExp", "Experimental Neutron");
    legend->AddEntry("neutronEnergyBack", "Background Neutron");
    legend->Draw("SAME");

    c_Erg->Write();
    //c_Erg->SaveAs("fig/NeutronErg.eps");
}

void readFiss::PlotMult()
{
    writeFile->cd();
    cd_basics->cd();

    cout << "Plotting Multiplicities." << endl;

    //create a canvas
    TCanvas* c_Mult = new TCanvas("cMult", "Multiplicity", 800, 400);
    // cLO->Divide(1,2);

    c_Mult->cd();

    photonMultExp->SetLineColor(kRed);
    photonMultExp->SetStats(0);
    photonMultExp->Draw();

    photonMultBack->SetLineColor(kRed);
    photonMultBack->SetLineStyle(kDashed);
    photonMultBack->SetStats(0);
    photonMultBack->Draw("SAME");

    neutronMultExp->SetLineColor(kBlue);
    neutronMultExp->SetStats(0);
    neutronMultExp->Draw("SAME");

    neutronMultBack->SetLineColor(kBlue);
    neutronMultBack->SetLineStyle(kDashed);
    neutronMultBack->SetStats(0);
    neutronMultBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronMultExp", "Experimental Neutron");
    legend->AddEntry("photonMultExp", "Experimental Photon");
    legend->AddEntry("neutronMultBack", "Background Neutron");
    legend->AddEntry("photonMultBack", "Background Photon");
    legend->Draw("SAME");

    c_Mult->Write();
    //c_Mult->SaveAs("fig/Multiplicity.eps");
}

void readFiss::PlotPSD()
{
    writeFile->cd();
    cd_basics->cd();

    cout << "Plotting Particle Discrimination." << endl;

    //create a canvas
    TCanvas* c_PSD = new TCanvas("cPSD", "Particle Dicrimination", 800, 400);
    // cLO->Divide(1,2);

    c_PSD->cd();

    photonPSDExp->SetLineColor(kRed);
    photonPSDExp->SetStats(0);
    photonPSDExp->Draw();

    photonPSDBack->SetLineColor(kRed);
    photonPSDBack->SetLineStyle(kDashed);
    photonPSDBack->SetStats(0);
    photonPSDBack->Draw("SAME");

    neutronPSDExp->SetLineColor(kBlue);
    neutronPSDExp->SetStats(0);
    neutronPSDExp->Draw("SAME");

    neutronPSDBack->SetLineColor(kBlue);
    neutronPSDBack->SetLineStyle(kDashed);
    neutronPSDBack->SetStats(0);
    neutronPSDBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronPSDExp", "Experimental Neutron");
    legend->AddEntry("photonPSDExp", "Experimental Photon");
    legend->AddEntry("neutronPSDBack", "Background Neutron");
    legend->AddEntry("photonPSDBack", "Background Photon");
    legend->Draw("SAME");

    c_PSD->Write();
    //c_PSD->SaveAs("fig/ParticleDiscrimination.eps");
}

void readFiss::PlotSingles()
{
    writeFile->cd();
    cd_basics->cd();
    cout << "Plotting Singles." << endl;

    //create a canvas
    TCanvas* c_Sin = new TCanvas("cSin", "Singles Rates", 800, 400);
    // cLO->Divide(1,2);

    c_Sin->cd();

    photonSinglesExp->SetLineColor(kRed);
    photonSinglesExp->SetStats(0);
    photonSinglesExp->Draw();

    photonSinglesBack->SetLineColor(kRed);
    photonSinglesBack->SetLineStyle(kDashed);
    photonSinglesBack->SetStats(0);
    photonSinglesBack->Draw("SAME");

    neutronSinglesExp->SetLineColor(kBlue);
    neutronSinglesExp->SetStats(0);
    neutronSinglesExp->Draw("SAME");

    neutronSinglesBack->SetLineColor(kBlue);
    neutronSinglesBack->SetLineStyle(kDashed);
    neutronSinglesBack->SetStats(0);
    neutronSinglesBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronSinglesExp", "Experimental Neutron");
    legend->AddEntry("photonSinglesExp", "Experimental Photon");
    legend->AddEntry("neutronSinglesBack", "Background Neutron");
    legend->AddEntry("photonSinglesBack", "Background Photon");
    legend->Draw("SAME");

    c_Sin->Write();
    //c_Sin->SaveAs("fig/Singles.eps");
}

void readFiss::PlotMultCor()
{
  writeFile->cd();
  cd_correlated->cd();
  cout << "Plotting correlated multiplicity." << endl;

  //make canvas
  TCanvas* c_MultCor = new TCanvas("cMultCor", "Neutron-Gamma Multiplicity",
                                                                  800, 400);
  c_MultCor->cd();

  neutronGammaMult->SetLineColor(kRed);
  neutronGammaMult->Draw("COLZ");

  // JONATHAN - change stat box DOESNT WORK YET
  /*c_MultCor->Update();
  TPaveStats *statsBox = (TPaveStats*)c_MultCor->GetPrimitive("stats");
  TList* statsList = statsBox->GetListOfLines();
  TText* tconst = statsBox->GetLineWith("Mean x");
  tconst->Print();
  cout << tconst->GetTitle() << endl;
  //cout << *(const_cast<char*>(reinterpret_cast<const char*>(tconst->GetWcsTitle()))) << endl;*/

  c_MultCor->Write();
  //c_MultCor->SaveAs("MultiplicityCorrelation.eps");
}

void readFiss::PlotMultLO()
{
  writeFile->cd();
  cd_correlated->cd();
  cout << "Plotting correlated mult/LO." << endl;

  //make canvas
  TCanvas* c_MultLO = new TCanvas("cMultLO", "Neutron Mult vs. Photon LO",
                                                                800, 400);
  c_MultLO->cd();

  neutronMultPhotonLO->SetLineColor(kRed);
  neutronMultPhotonLO->SetStats(0);
  neutronMultPhotonLO->Draw("COLZ");

  c_MultLO->Write();
  //c_MultLO->SaveAs("NeutronMultiplicityPhotonLO.eps");
}

/*
   ___                          _                ___ _         _____
  / __|___ _ __  _ __  __ _ _ _(_)___ ___ _ _   / __(_)_ __   / / __|_ ___ __
 | (__/ _ \ '  \| '_ \/ _` | '_| (_-</ _ \ ' \  \__ \ | '  \ / /| _|\ \ / '_ \
  \___\___/_|_|_| .__/\__,_|_| |_/__/\___/_||_| |___/_|_|_|_/_/ |___/_\_\ .__/
                |_|                                                     |_|

*/

void readFiss::CompareLightOut()
{

    writeFile->cd();
    cd_simComparison->cd();

    cout << "Comparing Light Output." << endl;


    photonLightOutputExp->SetLineColor(kRed);

    neutronLightOutputExp->SetLineColor(kBlue);

    neutronLightOutputSim->Scale((double)expEntries/(double)simEntries);
    neutronLightOutputSim->SetLineColor(kBlue);
    neutronLightOutputSim->SetLineStyle(kDashed);

    photonLightOutputSim->Scale((double)expEntries/(double)simEntries);
    photonLightOutputSim->SetLineColor(kRed);
    photonLightOutputSim->SetLineStyle(kDashed);



    //create a canvas
    TCanvas* c_LOs = new TCanvas("cLOs", "Light Output", 800, 400);
    // cLO->Divide(1,2);
    c_LOs->cd();

    photonLightOutputExp->Draw();
    neutronLightOutputExp->Draw("SAME");
    neutronLightOutputSim->Draw("SAME");
    photonLightOutputSim->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronLightOutputExp", "Experimental Neutron");
    legend->AddEntry("photonLightOutputExp", "Experimental Photon");
    legend->AddEntry("neutronLightOutputSim", "Simulated Neutron");
    legend->AddEntry("photonLightOutputSim", "Simulated Photon");
    legend->Draw("SAME");

    c_LOs->Write();
    //c_LOs->SaveAs("fig/LightOutputSim.eps");


    //
    // //neutrons only
    // TCanvas* c_LOns = new TCanvas("cLOns", "Light Output Neutrons", 800,400);
    // // cLO->Divide(1,2);
    // c_LOns->cd();
    //
    // neutronLightOutputExp->Draw();
    // neutronLightOutputSim->Draw("SAME");
    //
    // TLegend *legendN = new TLegend(0.7, 0.8, 0.48, 0.9);
    // legendN->AddEntry("neutronLightOutputExp", "Experimental Neutron");
    // legendN->AddEntry("neutronLightOutputSim", "Simulated Neutron");
    // legendN->Draw("SAME");
    //
    // TRatioPlot *ratN = new TRatioPlot(neutronLightOutputSim, neutronLightOutputExp);
    // ratN->Draw("SAME");
    //
    // c_LOns->Write();
    //
    //
    // //photons only
    // TCanvas* c_LOps = new TCanvas("cLOps", "Light Output Photons", 800,400);
    // // cLO->Divide(1,2);
    // c_LOps->cd();
    //
    // photonLightOutputExp->Draw();
    // photonLightOutputSim->Draw("SAME");
    //
    // TLegend *legendP = new TLegend(0.7, 0.8, 0.48, 0.9);
    // legendP->AddEntry("photonLightOutputExp", "Experimental Photon");
    // legendP->AddEntry("photonLightOutputSim", "Simulated Photon");
    // legendP->Draw("SAME");
    //
    // TRatioPlot *ratP = new TRatioPlot(photonLightOutputSim, photonLightOutputExp);
    // ratP->Draw("SAME");
    //
    // c_LOps->Write();

}

void readFiss::CompareTof()
{

    writeFile->cd();
    cd_simComparison->cd();

    cout << "Comparing Time of Flight." << endl;

    //create a canvas

     //create a canvas
    TCanvas* c_ToFs = new TCanvas("cToFs", "Time of Flight", 800, 400);
    // cLO->Divide(1,2);
    c_ToFs->cd();


    neutronTofExp->SetLineColor(kBlue);
    neutronTofExp->Draw();

    photonTofExp->SetLineColor(kRed);
    photonTofExp->Draw("SAME");

    neutronTofSim->Scale((double)expEntries/(double)simEntries);
    neutronTofSim->SetLineStyle(kDashed);
    neutronTofSim->SetLineColor(kBlue);
    neutronTofSim->Draw("SAME");

    photonTofSim->Scale((double)expEntries/(double)simEntries);
    photonTofSim->SetLineStyle(kDashed);
    photonTofSim->SetLineColor(kRed);
    photonTofSim->Draw("SAME");

    // TRatioPlot* rat = new TRatioPlot(neutronTofExp, neutronTofSim);
    // rat->Draw("SAME");

    TLegend* legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronTofExp", "Experimental Neutron");
    legend->AddEntry("photonTofExp", "Experimental Photon");
    legend->AddEntry("neutronTofSim", "Simulated Neutron");
    legend->AddEntry("photonTofSim", "Simulated Photon");
    legend->Draw("SAME");

    c_ToFs->Write();
    //c_ToFs->SaveAs("fig/TimeOfFlightSim.eps");
}

void readFiss::CompareErg()
{
    writeFile->cd();
    cd_simComparison->cd();

    cout << "Comparing Energy." << endl;

    //create a canvas
    TCanvas* c_Ergs = new TCanvas("cErgs", "Energy", 800, 400);
    // cLO->Divide(1,2);
    c_Ergs->cd();

    neutronEnergyExp->SetLineColor(kBlue);
    neutronEnergyExp->Draw();

    neutronEnergySim->Scale((double)expEntries/(double)simEntries);
    neutronEnergySim->SetLineColor(kBlue);
    neutronEnergySim->SetLineStyle(kDashed);
    neutronEnergySim->Draw("SAME");

    TLegend* legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronEnergyExp", "Experimental Neutron");
    legend->AddEntry("neutronEnergySim", "Simulated Neutron");
    legend->Draw("SAME");

    c_Ergs->Write();
    // c_Ergs->SaveAs("fig/NeutronErgSim.eps");
}

void readFiss::CompareMult()
{
    writeFile->cd();
    cd_simComparison->cd();
    cout << "Comparing Multiplicity." << endl;

    //create a canvas
    TCanvas* c_Mults = new TCanvas("cMults", "Multiplicity", 800, 400);
    // cLO->Divide(1,2);

    c_Mults->cd();

    photonMultExp->SetLineColor(kRed);
    photonMultExp->Draw();

    photonMultSim->Scale((double)expEntries/(double)simEntries);
    photonMultSim->SetLineColor(kRed);
    photonMultSim->SetLineStyle(kDashed);
    photonMultSim->Draw("SAME");

    neutronMultExp->SetLineColor(kBlue);
    neutronMultExp->Draw("SAME");

    neutronMultSim->Scale((double)expEntries/(double)simEntries);
    neutronMultSim->SetLineColor(kBlue);
    neutronMultSim->SetLineStyle(kDashed);
    neutronMultSim->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronMultSim", "Experimental Neutron");
    legend->AddEntry("photonMultSim", "Experimental Photon");
    legend->AddEntry("neutronMultSim", "Simulated Neutron");
    legend->AddEntry("photonMultSim", "Simulated Photon");
    legend->Draw("SAME");

    c_Mults->Write();
    //c_Mults->SaveAs("fig/MultiplicitySim.eps");
}


void readFiss::ComparePSD()
{
    writeFile->cd();
    cd_simComparison->cd();
    cout << "Comparing Discrimination." << endl;

    //create a canvas
    TCanvas* c_PSDs = new TCanvas("cPSDs", "Particle Dicrimination", 800, 400);
    // cLO->Divide(1,2);

    c_PSDs->cd();

    photonPSDExp->SetLineColor(kRed);
    photonPSDExp->Draw();

    photonPSDSim->Scale((double)expEntries/(double)simEntries);
    photonPSDSim->SetLineColor(kRed);
    photonPSDSim->SetLineStyle(kDashed);
    photonPSDSim->Draw("SAME");

    neutronPSDExp->SetLineColor(kBlue);
    neutronPSDExp->Draw("SAME");

    neutronPSDSim->Scale((double)expEntries/(double)simEntries);
    neutronPSDSim->SetLineColor(kBlue);
    neutronPSDSim->SetLineStyle(kDashed);
    neutronPSDSim->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronPSDExp", "Experimental Neutron");
    legend->AddEntry("photonPSDExp", "Experimental Photon");
    legend->AddEntry("neutronPSDSim", "Simulated Neutron");
    legend->AddEntry("photonPSDSim", "Simulated Photon");
    legend->Draw("SAME");

    c_PSDs->Write();
    //c_PSDs->SaveAs("fig/ParticleDiscriminationSim.eps");
}

void readFiss::CompareSingles()
{
    writeFile->cd();
    cd_simComparison->cd();
    cout << "Comparing Singles." << endl;

    //create a canvas
    TCanvas* c_Sins = new TCanvas("cSins", "Singles Rates", 800, 400);
    // cLO->Divide(1,2);

    c_Sins->cd();

    photonSinglesExp->SetLineColor(kRed);
    photonSinglesExp->Draw();

    photonSinglesSim->Scale((double)expEntries/(double)simEntries);
    photonSinglesSim->SetLineColor(kRed);
    photonSinglesSim->SetLineStyle(kDashed);
    photonSinglesSim->Draw("SAME");

    neutronSinglesExp->SetLineColor(kBlue);
    neutronSinglesExp->Draw("SAME");

    neutronSinglesSim->Scale((double)expEntries/(double)simEntries);
    neutronSinglesSim->SetLineColor(kBlue);
    neutronSinglesSim->SetLineStyle(kDashed);
    neutronSinglesSim->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronSinglesExp", "Experimental Neutron");
    legend->AddEntry("photonSinglesExp", "Experimental Photon");
    legend->AddEntry("neutronSinglesSim", "Simulated Neutron");
    legend->AddEntry("photonSinglesSim", "Simulated Photon");
    legend->Draw("SAME");

    c_Sins->Write();
    //c_Sins->SaveAs("fig/SinglesSim.eps");
}
