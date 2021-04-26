#include "readFiss.h"

void readFiss::InitializeHistograms()
{

  const int numTofBins = 120;
	const double minTof = -20;
	const double maxTof = 100;

	const int numLObins = 250;
	const double minLO = 0;
	const double maxLO = 5;

	const int numErgBins = 100;
	const double minErg = 0;
	const double maxErg = 10;

  const int numPSDBins = 100;
  const double minPSP = 0;
  const double maxPSP = 1;

  const int minMult = 0;
  const int maxMult = 10;



  neutronLightOutputExp = new TH1D("neutronLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  photonLightOutputExp = new TH1D("photonLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  neutronTofExp = new TH1D("neutronTofExp", "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
  photonTofExp = new TH1D("photonTofExp", "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
  neutronEnergyExp = new TH1D("neutronEnergyExp", "Neutron Energy Experiment;Energy [MeV];Counts", numErgBins, minErg, maxErg);
  neutronMultExp = new TH1I("neutronMultExp", "Neutron Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  photonMultExp = new TH1I("photonMultExp", "Photon Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  neutronPSDExp = new TH1D("neutronPSDExp", "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
  photonPSDExp = new TH1D("photonPSDExp", "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);

  neutronLightOutputBack = new TH1D("neutronLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  photonLightOutputBack = new TH1D("photonLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  neutronTofBack = new TH1D("neutronTofBack", "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);
  photonTofBack = new TH1D("photonTofBack", "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);
  neutronEnergyBack = new TH1D("neutronEnergyBack", "Neutron Energy Background;Energy [MeV];Counts", numErgBins, minErg, maxErg);
  neutronMultBack = new TH1I("neutronMultBack", "Neutron Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  photonMultBack = new TH1I("photonMultBack", "Photon Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  neutronPSDBack = new TH1D("neutronPSDBack", "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
  photonPSDBack = new TH1D("photonPSDBack", "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);



  if(simTree != 0)
  {
    neutronLightOutputSim = new TH1D("neutronLightOutputSim", "Light Output Simulation;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
    photonLightOutputSim = new TH1D("photonLightOutputSim", "Light Output Simulation;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
    neutronTofSim = new TH1D("neutronTofSim", "Time-of-Flight Simulation;Time [ns];Counts", numTofBins, minTof, maxTof);
    photonTofSim = new TH1D("photonTofSim", "Time-of-Flight Simulation;Time [ns];Counts", numTofBins, minTof, maxTof);
    neutronEnergySim = new TH1D("neutronEnergySim", "Neutron Energy Simulation;Energy [MeV];Counts", numErgBins, minErg, maxErg);
    neutronMultSim = new TH1I("neutronMultSim", "Neutron Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
    photonMultSim = new TH1I("photonMultSim", "Photon Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
    neutronPSDSim = new TH1D("neutronPSDSim", "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
    photonPSDSim = new TH1D("photonPSDSim", "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);
  }

}
