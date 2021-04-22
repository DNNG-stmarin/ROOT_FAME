#include "readFiss.h"

void readFiss::InitializeHistograms()
{

	neutronLightOutputExp = new TH1D("neutronLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", 100, 0, 5);
    photonLightOutputExp = new TH1D("photonLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", 100, 0, 5);
    neutronTofExp = new TH1D("neutronTofExp", "Time-of-Flight Experiment;Time [ns];Counts", 100, 0, 100);
    photonTofExp = new TH1D("photonTofExp", "Time-of-Flight Experiment;Time [ns];Counts", 100, 0, 100);
    neutronEnergyExp = new TH1D("neutronEnergyExp", "Neutron Energy Experiment;Energy [MeV];Counts", 140, 0, 14);  

    neutronLightOutputBack = new TH1D("neutronLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", 100, 0, 5);
    photonLightOutputBack = new TH1D("photonLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", 100, 0, 5);
    neutronTofBack = new TH1D("neutronTofBack", "Time-of-Flight Background;Time [ns];Counts", 50, 0, 100);
    photonTofBack = new TH1D("photonTofBack", "Time-of-Flight Background;Time [ns];Counts", 50, 0, 100);
    neutronEnergyBack = new TH1D("neutronEnergyBack", "Neutron Energy Background;Energy [MeV];Counts", 140, 0, 14);  


    if(simTree != 0)
    {
	    neutronLightOutputSim = new TH1D("neutronLightOutputSim", "Light Output Simulation;Light Output [MeVee];Counts", 100, 0, 5);
	    photonLightOutputSim = new TH1D("photonLightOutputSim", "Light Output Simulation;Light Output [MeVee];Counts", 100, 0, 5);
	    neutronTofSim = new TH1D("neutronTofSim", "Time-of-Flight Simulation;Time [ns];Counts", 50, 0, 100);
	    photonTofSim = new TH1D("photonTofSim", "Time-of-Flight Simulation;Time [ns];Counts", 50, 0, 100);
	    neutronEnergySim = new TH1D("neutronEnergySim", "Neutron Energy Simulation;Energy [MeV];Counts", 140, 0, 14);
	    neutronEnergyExp = new TH1D("neutronEnergyExp", "Neutron Energy Experiment;Energy [MeV];Counts", 140, 0, 14); 
    }

}
 
 