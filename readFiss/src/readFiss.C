#include "readFiss.h"
#include <TLegend.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFitResult.h>
#include <THStack.h>
#include <TFolder.h>
#include <TSystem.h>

#include <iostream>
#include <fstream>
#include <sstream>



using namespace std;

//Destructor
readFiss::~readFiss()
{
    if (!expTree) return;
    delete expFile;
    if(mode == 1)
    {
      delete simFile;
    }
    delete writeFile;
}
//Get entry at integer <entry>
Int_t readFiss::GetExpEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!expTree) return 0;
    return expTree->GetEntry(entry);
}
Int_t readFiss::GetSimEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!simTree) return 0;
    return simTree->GetEntry(entry);
}
//Load root Trees
Long64_t readFiss::LoadExpTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!expTree) return -5;
    Long64_t centry = expTree->LoadTree(entry);
    if (centry < 0) return centry;
    if (expTree->GetTreeNumber() != fCurrent) {
        fCurrent = expTree->GetTreeNumber();
        Notify();
    }
    return centry;
}
Long64_t readFiss::LoadSimTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!simTree) return -5;
    Long64_t centry = simTree->LoadTree(entry);
    if (centry < 0) return centry;
    if (simTree->GetTreeNumber() != fCurrent) {
        fCurrent = simTree->GetTreeNumber();
        Notify();
    }
    return centry;
}

void readFiss::InitExp(TChain* tree)
{

    //INITIALIZE EXPERIMENT TREE
    if (!tree) return;
    expTree = tree;
    fCurrent = -1;
    expTree->SetMakeClass(1);

    expTree->SetBranchAddress("fisTime", &fisTime, &b_fisTime);
    expTree->SetBranchAddress("fisDep", &fisDep, &b_fisDep);
    expTree->SetBranchAddress("fisChan", &fisChan, &b_fisChan);
    expTree->SetBranchAddress("fisPSP", &fisPSP, &b_fisPSP);
    expTree->SetBranchAddress("neutronMult", &neutronMult, &b_neutronMult);
    expTree->SetBranchAddress("gammaMult", &gammaMult, &b_gammaMult);
    expTree->SetBranchAddress("neutronBackMult", &neutronBackMult, &b_neutronBackMult);
    expTree->SetBranchAddress("gammaBackMult", &gammaBackMult, &b_gammaBackMult);
    expTree->SetBranchAddress("neutronDetTimes", neutronDetTimes, &b_neutronDetTimes);
    expTree->SetBranchAddress("neutronLightOut", neutronLightOut, &b_neutronLightOut);
    expTree->SetBranchAddress("neutronPSD", neutronPSD, &b_neutronPSD);
    expTree->SetBranchAddress("neutronToFErg", neutronToFErg, &b_neutronToFErg);
    expTree->SetBranchAddress("neutronDet", neutronDet, &b_neutronDet);
    expTree->SetBranchAddress("neutronVx", neutronVx, &b_neutronVx);
    expTree->SetBranchAddress("neutronVy", neutronVy, &b_neutronVy);
    expTree->SetBranchAddress("neutronVz", neutronVz, &b_neutronVz);
    expTree->SetBranchAddress("photonDetTimes", photonDetTimes, &b_photonDetTimes);
    expTree->SetBranchAddress("photonLightOut", photonLightOut, &b_photonLightOut);
    expTree->SetBranchAddress("photonPSD", photonPSD, &b_photonPSD);
    expTree->SetBranchAddress("photonDet", photonDet, &b_photonDet);
    expTree->SetBranchAddress("photonVx", photonVx, &b_photonVx);
    expTree->SetBranchAddress("photonVy", photonVy, &b_photonVy);
    expTree->SetBranchAddress("photonVz", photonVz, &b_photonVz);
    expTree->SetBranchAddress("backNeutronDetTimes", backNeutronDetTimes, &b_backNeutronDetTimes);
    expTree->SetBranchAddress("backNeutronLightOut", backNeutronLightOut, &b_backNeutronLightOut);
    expTree->SetBranchAddress("backNeutronPSD", backNeutronPSD, &b_backNeutronPSD);
    expTree->SetBranchAddress("backNeutronToFErg", backNeutronToFErg, &b_backNeutronToFErg);
    expTree->SetBranchAddress("backNeutronDet", backNeutronDet, &b_backNeutronDet);
    expTree->SetBranchAddress("backNeutronVx", backNeutronVx, &b_backNeutronVx);
    expTree->SetBranchAddress("backNeutronVy", backNeutronVy, &b_backNeutronVy);
    expTree->SetBranchAddress("backNeutronVz", backNeutronVz, &b_backNeutronVz);
    expTree->SetBranchAddress("backPhotonDetTimes", backPhotonDetTimes, &b_backPhotonDetTimes);
    expTree->SetBranchAddress("backPhotonLightOut", backPhotonLightOut, &b_backPhotonLightOut);
    expTree->SetBranchAddress("backPhotonPSD", backPhotonPSD, &b_backPhotonPSD);
    expTree->SetBranchAddress("backPhotonDet", backPhotonDet, &b_backPhotonDet);
    expTree->SetBranchAddress("backPhotonVx", backPhotonVx, &b_backPhotonVx);
    expTree->SetBranchAddress("backPhotonVy", backPhotonVy, &b_backPhotonVy);
    expTree->SetBranchAddress("backPhotonVz", backPhotonVz, &b_backPhotonVz);
    Notify();
}

void readFiss::InitSim(TChain* tree)
{
    //INTITIALZE SIM TREE
    if (!tree) return;
    simTree = tree;
    fCurrent = -1;
    simTree->SetMakeClass(1);

    simTree->SetBranchAddress("neutronMult", &neutronMult, &b_neutronMultSim);//already defined no need to change
    simTree->SetBranchAddress("gammaMult", &gammaMult, &b_gammaMultSim);
    simTree->SetBranchAddress("neutronChannel", neutronChannel, &b_neutronChannelSim);
    simTree->SetBranchAddress("neutronDetTimes", neutronDetTimes, &b_neutronDetTimesSim);
    simTree->SetBranchAddress("neutronEnergy", neutronEnergy, &b_neutronEnergySim);
    simTree->SetBranchAddress("neutronIntegral", neutronIntegral, &b_neutronIntegralSim);
    simTree->SetBranchAddress("neutronHeight", neutronHeight, &b_neutronHeightSim);
    simTree->SetBranchAddress("neutronPSD", neutronPSD, &b_neutronPSDSim);
    simTree->SetBranchAddress("photonChannel", photonChannel, &b_photonChannelSim);
    simTree->SetBranchAddress("photonDetTimes", photonDetTimes, &b_photonDetTimesSim);
    simTree->SetBranchAddress("photonEnergy", photonEnergy, &b_photonEnergySim);
    simTree->SetBranchAddress("photonIntegral", photonIntegral, &b_photonIntegralSim);
    simTree->SetBranchAddress("photonHeight", photonHeight, &b_photonHeightSim);
    simTree->SetBranchAddress("photonPSD", photonPSD, &b_photonPSDSim);
    simTree->SetBranchAddress("neutronFlag", neutronFlag, &b_neutronFlagSim);
    simTree->SetBranchAddress("photonFlag", photonFlag, &b_photonFlagSim);
    Notify();
}
Bool_t readFiss::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void readFiss::ShowExp(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!expTree) return;
    expTree->Show(entry);
}
void readFiss::ShowSim(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!simTree) return;
    simTree->Show(entry);
}
Int_t readFiss::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
