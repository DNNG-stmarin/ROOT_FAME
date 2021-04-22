#include "readFiss.h"
#include <TLegend.h>
#include <TCanvas.h>

/*
  ___                   _               _   
 | __|_ ___ __  ___ _ _(_)_ __  ___ _ _| |_ 
 | _|\ \ / '_ \/ -_) '_| | '  \/ -_) ' \  _|
 |___/_\_\ .__/\___|_| |_|_|_|_\___|_||_\__|
         |_|                                
*/

void readFiss::PlotLightOut()
{
    analysisFile->cd();

    //create a canvas
    TCanvas* c_LO = new TCanvas("cLO", "Light Output", 800,400);
    // cLO->Divide(1,2);

    c_LO->cd();
    neutronLightOutputExp->SetLineColor(kBlue);
    neutronLightOutputExp->Draw();

    neutronLightOutputBack->SetLineColor(kBlue);
    neutronLightOutputBack->SetLineStyle(kDashed);
    neutronLightOutputBack->Draw("SAME");
    
    photonLightOutputExp->SetLineColor(kRed);
    photonLightOutputExp->Draw("SAME");

    photonLightOutputBack->SetLineColor(kRed);
    photonLightOutputBack->SetLineStyle(kDashed);
    photonLightOutputBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronLightOutputExp", "Experimental Neutron");
    legend->AddEntry("photonLightOutputExp", "Experimental Photon");
    legend->AddEntry("neutronLightOutputBack", "Background Neutron");
    legend->AddEntry("photonLightOutputBack", "Background Photon");
    legend->Draw("SAME");

    c_LO->Write();
}

void readFiss::PlotTof()
{

    //create a canvas
    TCanvas* c_ToF = new TCanvas("cToF", "Time of Flight", 800,400);
    // cLO->Divide(1,2);

    c_ToF->cd();
    neutronTofExp->SetLineColor(kBlue);
    neutronTofExp->Draw();

    neutronTofBack->SetLineColor(kBlue);
    neutronTofBack->SetLineStyle(kDashed);
    neutronTofBack->Draw("SAME");
    
    photonTofExp->SetLineColor(kRed);
    photonTofExp->Draw("SAME");

    photonTofBack->SetLineColor(kRed);
    photonTofBack->SetLineStyle(kDashed);
    photonTofBack->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronTofExp", "Experimental Neutron");
    legend->AddEntry("photonTofExp", "Experimental Photon");
    legend->AddEntry("neutronTofBack", "Background Neutron");
    legend->AddEntry("photonTofBack", "Background Photon");
    legend->Draw("SAME");

    c_ToF->Write();
}

void readFiss::PlotErg()
{

    //create a canvas
    TCanvas* c_Erg = new TCanvas("cErg", "ToF Energy", 800,400);
    // cLO->Divide(1,2);

    c_Erg->cd();
    neutronEnergyExp->SetLineColor(kBlue);
    neutronEnergyExp->Draw();

    neutronEnergyBack->SetLineColor(kBlue);
    neutronEnergyBack->SetLineStyle(kDashed);
    neutronEnergyBack->Draw("SAME");


    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronEnergyExp", "Experimental Neutron");
    legend->AddEntry("neutronEnergyBack", "Background Neutron");
    legend->Draw("SAME");

    c_Erg->Write();
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
    //create a canvas
    neutronLightOutputExp->SetLineColor(kBlue);
    neutronLightOutputExp->Draw();

    photonLightOutputExp->SetLineStyle(kDashed);
    photonLightOutputExp->SetLineColor(kBlue);
    photonLightOutputExp->Draw("SAME");

    neutronLightOutputSim->SetLineColor(kRed);
    //neutronLightOutputSim->Scale(expTree->GetEntriesFast() / simTree->GetEntriesFast());
    neutronLightOutputSim->Draw("SAME");

    //photonLightOutputSim->Scale(expTree->GetEntriesFast() / simTree->GetEntriesFast());
    
    photonLightOutputExp->SetLineStyle(kDashed); 
    photonLightOutputExp->SetLineColor(kRed);
    photonLightOutputExp->Draw("SAME");

    TLegend *legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronLightOutputExp", "Experimental Neutron");
    legend->AddEntry("photonLightOutputExp", "Experimental Photon");
    legend->AddEntry("neutronLightOutputSim", "Simulated Neutron");
    legend->AddEntry("photonLightOutputSim", "Simulated Photon");
    legend->Draw("SAME");
}
void readFiss::CompareTof()
{
    //create a canvas
    neutronTofExp->SetLineColor(kBlue);
    neutronTofExp->Draw();

    photonTofExp->SetLineStyle(kDashed);
    photonTofExp->SetLineColor(kBlue);
    photonTofExp->Draw("SAME");

    neutronTofSim->SetLineColor(kRed);
    //neutronTofSim->Scale(expTree->GetEntriesFast() / simTree->GetEntriesFast());
    neutronTofSim->Draw("SAME");

    //photonTofSim->Scale(expTree->GetEntriesFast() / simTree->GetEntriesFast());
    photonTofSim->SetLineStyle(kDashed);
    photonTofSim->SetLineColor(kRed);
    photonTofSim->Draw("SAME");

    TLegend* legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronTofExp", "Experimental Neutron");
    legend->AddEntry("photonTofExp", "Experimental Photon");
    legend->AddEntry("neutronTofSim", "Simulated Neutron");
    legend->AddEntry("photonTofSim", "Simulated Photon");
    legend->Draw("SAME");
}

void readFiss::CompareErg()
{
    neutronEnergyExp->SetLineColor(kBlue);
    neutronEnergyExp->Draw();

    neutronEnergySim->SetLineColor(kRed);
    //neutronEnergySim->Scale(expTree->GetEntriesFast() / simTree->GetEntriesFast());
    neutronEnergySim->Draw("SAME");

    TLegend* legend = new TLegend(0.7, 0.8, 0.48, 0.9);
    legend->AddEntry("neutronEnergyExp", "Experimental Neutron");
    legend->AddEntry("neutronEnergySim", "Simulated Neutron");
}