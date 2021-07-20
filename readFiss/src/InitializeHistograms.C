#include "readFiss.h"
using namespace std;
void readFiss::InitializeHistograms()
{
  cout << "initializing histograms" << endl;

  const int numTofBins = 240;
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

  const int numDets = NUM_DETECTORS;

  const int numCosBins = 100;

  const int numfisDepBins = 500; //JJ import these numbers from the beamInfo.root file
  const double minDep = 0;
  const double maxDep = 0.05;

  const int numfisBeamTimeBins= 2000;
  const double minBeamTime = -200;
  const double maxBeamTime = 1800;

  const int numDeltaT = 200;
  const double minDeltaT = 0;
  const double maxDeltaT = 10000;




  /*
   _   _                         _      _          _
  | | | |_ _  __ ___ _ _ _ _ ___| |__ _| |_ ___ __| |
  | |_| | ' \/ _/ _ \ '_| '_/ -_) / _` |  _/ -_) _` |
   \___/|_||_\__\___/_| |_| \___|_\__,_|\__\___\__,_|

   */

  fissRej = new TH1I("fissRej", "Composition of Fission Rejection; Fission Type; Counts", 10, -0.5, 9.5);
  h_timeDiffTrig = new TH1D* [NUM_TRIGGERS];
  for(int j = 0; j < NUM_TRIGGERS; j++)
  {
    h_timeDiffTrig[j] = new TH1D("h_timeDiffTrig_" + TString(to_string(j)), "Time difference Triggers; Delta Time (ns); Counts", numDeltaT, minDeltaT, maxDeltaT);
  }

  neutronLightOutputExp = new TH1D("neutronLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  photonLightOutputExp = new TH1D("photonLightOutputExp", "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  neutronTofExp = new TH1D("neutronTofExp", "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
  photonTofExp = new TH1D("photonTofExp", "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
  neutronEnergyExp = new TH1D("neutronEnergyExp", "Neutron Energy Experiment;Energy [MeV];Counts", numErgBins, minErg, maxErg);
  neutronMultExp = new TH1I("neutronMultExp", "Neutron Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  photonMultExp = new TH1I("photonMultExp", "Photon Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  neutronPSDExp = new TH1D("neutronPSDExp", "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
  photonPSDExp = new TH1D("photonPSDExp", "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);
  neutronSinglesExp = new TH1D("neutronSinglesExp", "Neutron Singles; Detector1; counts", numDets, 0, numDets);
  photonSinglesExp = new TH1D("photonSinglesExp", "Photon Singles; Detector1; counts", numDets, 0, numDets);

  neutronLightOutputBack = new TH1D("neutronLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  photonLightOutputBack = new TH1D("photonLightOutputBack", "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
  neutronTofBack = new TH1D("neutronTofBack", "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);
  photonTofBack = new TH1D("photonTofBack", "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);
  neutronEnergyBack = new TH1D("neutronEnergyBack", "Neutron Energy Background;Energy [MeV];Counts", numErgBins, minErg, maxErg);
  neutronMultBack = new TH1I("neutronMultBack", "Neutron Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  photonMultBack = new TH1I("photonMultBack", "Photon Multiplicity; multiplicity; counts", maxMult, minMult, maxMult);
  neutronPSDBack = new TH1D("neutronPSDBack", "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
  photonPSDBack = new TH1D("photonPSDBack", "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);
  neutronSinglesBack = new TH1D("neutronSinglesBack", "Neutron Singles; Detector1; counts", numDets, 0, numDets);
  photonSinglesBack = new TH1D("photonSinglesBack", "Photon Singles; Detector1; counts", numDets, 0, numDets);



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
    neutronSinglesSim = new TH1D("neutronSinglesSim", "Neutron Singles; Detector1; counts", numDets, 0, numDets);
    photonSinglesSim = new TH1D("photonSinglesSim", "Photon Singles; Detector1; counts", numDets, 0, numDets);
  }

/*
  ___                 _      _          _
 / __|___ _ _ _ _ ___| |__ _| |_ ___ __| |
| (__/ _ \ '_| '_/ -_) / _` |  _/ -_) _` |
 \___\___/_| |_| \___|_\__,_|\__\___\__,_|

 */
  // correlated multiplicity
  neutronGammaMultExp = new TH2I("neutronGammaMultExp", "Neutron-Gamma Multiplicity;Neutron Multiplicity; Gamma Multiplicity; Counts",maxMult, minMult, maxMult, maxMult, minMult, maxMult);
  neutronGammaMultBack = new TH2I("neutronGammaMultBack", "Neutron-Gamma Back Multiplicity;Neutron Multiplicity; Gamma Multiplicity; Counts",maxMult, minMult, maxMult, maxMult, minMult, maxMult);

// neutron mult vs photon light output
  neutronMultPhotonLOExp = new TH2D("neutronMultPhotonLOExp", "Neutron Multiplicity vs. Photon Light Output; Neutron Multiplicity; Photon Light Output [MeVee]; Counts", maxMult, minMult-0.5, maxMult-0.5, numLObins, minLO, maxLO);

// neutron ToFErg vs neutron Light Ouput
  neutronEnergyLOExp = new TH2D("neutronEnergyLOExp", "Neutron Energy vs. Neutron Light Output; Neutron Energy [MeV]; Neutron Light Output [MeVee]; Counts", numErgBins, minErg, maxErg, numLObins, minLO, maxLO);

// neutron Light Output vs neutron PSD
  neutronLightOutPSDExp = new TH2D("neutronLightOutPSDExp", "Neutron Light Output vs. Neutron PSD; Neutron Light Output [MeVee]; Neutron PSP [tail/total]; Counts", numLObins, minLO, maxLO, numPSDBins, minPSP, maxPSP);

// photon Light Output vs neutron PSD
  photonLightOutPSDExp = new TH2D("photonLightOutPSDExp", "Photon Light Output vs. Photon PSD; Photon Light Output [MeVee]; Photon PSP [tail/total]; Counts", numLObins, minLO, maxLO, numPSDBins, minPSP, maxPSP);

// angular coorrelations
  neutronDoublesMat = new TH2D("neutronDoublesExp", "Neutron Doubles; Detector1; Detector2; counts", numDets, 0, numDets, numDets, 0, numDets);
  neutronSinglesMat = new TH2D("neutronSinglesMatExp", "Neutron Singles; Detector1; Detector2; counts", numDets, 0, numDets, numDets, 0, numDets);
  // neutronAngleCorr = new TH1D("neutronAngleCorr", "Neutron Angular Correlations; Cos T; counts", numCosBins, -1, 1);



  //
  /*
  ___         _ _     _    _           _
 |_ _|_ _  __| (_)_ _(_)__| |_  _ __ _| |
  | || ' \/ _` | \ V / / _` | || / _` | |
 |___|_||_\__,_|_|\_/|_\__,_|\_,_\__,_|_|

 */

 // basic
 IndivNeutronLightOutputExp = new TH1D*[NUM_DETECTORS];
 IndivNeutronLightOutputBack = new TH1D*[NUM_DETECTORS];

 IndivPhotonLightOutputExp = new TH1D*[NUM_DETECTORS];
 IndivPhotonLightOutputBack = new TH1D*[NUM_DETECTORS];

 IndivNeutronTofExp = new TH1D*[NUM_DETECTORS];
 IndivNeutronTofBack = new TH1D*[NUM_DETECTORS];

 IndivPhotonTofExp = new TH1D*[NUM_DETECTORS];
 IndivPhotonTofBack = new TH1D*[NUM_DETECTORS];

 IndivNeutronEnergyExp = new TH1D*[NUM_DETECTORS];
 IndivNeutronEnergyBack = new TH1D*[NUM_DETECTORS];

 IndivNeutronPSDExp = new TH1D*[NUM_DETECTORS];
 IndivNeutronPSDBack = new TH1D*[NUM_DETECTORS];

 IndivPhotonPSDExp = new TH1D*[NUM_DETECTORS];
 IndivPhotonPSDBack = new TH1D*[NUM_DETECTORS];

 // simulated
 if(mode == 1)
 {
   IndivNeutronLightOutputSim = new TH1D*[NUM_DETECTORS];
   IndivPhotonLightOutputSim = new TH1D*[NUM_DETECTORS];
   IndivNeutronTofSim = new TH1D*[NUM_DETECTORS];
   IndivPhotonTofSim = new TH1D*[NUM_DETECTORS];
   IndivNeutronEnergySim = new TH1D*[NUM_DETECTORS];
   IndivNeutronPSDSim = new TH1D*[NUM_DETECTORS];
   IndivPhotonPSDSim = new TH1D*[NUM_DETECTORS];
 }

 // correlated
 IndivNeutronEnergyLOExp = new TH2D*[NUM_DETECTORS];
 IndivNeutronLightOutPSDExp = new TH2D*[NUM_DETECTORS];
 IndivPhotonLightOutPSDExp = new TH2D*[NUM_DETECTORS];

 for(int i = 0; i < NUM_DETECTORS; ++i)
 {
   // basic
   IndivNeutronLightOutputExp[i] = new TH1D((TString)"IndivNeutronLightOutputExp" + (TString)to_string(i), "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
   IndivNeutronLightOutputBack[i] = new TH1D((TString)"IndivNeutronLightOutputBack" + (TString)to_string(i), "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);

   IndivPhotonLightOutputExp[i] = new TH1D((TString)"IndivPhotonLightOutputExp" + (TString)to_string(i), "Light Output Experiment;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
   IndivPhotonLightOutputBack[i] = new TH1D((TString)"IndivPhotonLightOutputBack" + (TString)to_string(i), "Light Output Background;Light Output [MeVee];Counts", numLObins, minLO, maxLO);

   IndivNeutronTofExp[i] = new TH1D((TString)"IndivNeutronTofExp" + (TString)to_string(i), "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
   IndivNeutronTofBack[i] = new TH1D((TString)"IndivNeutronTofBack" + (TString)to_string(i), "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);

   IndivPhotonTofExp[i] = new TH1D((TString)"IndivPhotonTofExp" + (TString)to_string(i), "Time-of-Flight Experiment;Time [ns];Counts", numTofBins, minTof, maxTof);
   IndivPhotonTofBack[i] = new TH1D((TString)"IndivPhotonTofBack" + (TString)to_string(i), "Time-of-Flight Background;Time [ns];Counts", numTofBins, minTof, maxTof);

   IndivNeutronEnergyExp[i] = new TH1D((TString)"IndivNeutronEnergyExp" + (TString)to_string(i), "Neutron Energy Experiment;Energy [MeV];Counts", numErgBins, minErg, maxErg);
   IndivNeutronEnergyBack[i] = new TH1D((TString)"IndivNeutronEnergyBack" + (TString)to_string(i), "Neutron Energy Background;Energy [MeV];Counts", numErgBins, minErg, maxErg);

   IndivNeutronPSDExp[i] = new TH1D((TString)"IndivNeutronPSDExp" + (TString)to_string(i), "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
   IndivNeutronPSDBack[i] = new TH1D((TString)"IndivNeutronPSDBack" + (TString)to_string(i), "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);

   IndivPhotonPSDExp[i] = new TH1D((TString)"IndivPhotonPSDExp" + (TString)to_string(i), "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);
   IndivPhotonPSDBack[i] = new TH1D((TString)"IndivPhotonPSDBack" + (TString)to_string(i), "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);

   // simulated
   if(mode == 2)
   {
     IndivNeutronLightOutputSim[i] = new TH1D((TString)"IndivNeutronLightOutputSim" + (TString)to_string(i), "Light Output Simulation;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
     IndivPhotonLightOutputSim[i] = new TH1D((TString)"IndivPhotonLightOutputSim" + (TString)to_string(i), "Light Output Simulation;Light Output [MeVee];Counts", numLObins, minLO, maxLO);
     IndivNeutronTofSim[i] = new TH1D((TString)"IndivNeutronTofSim" + (TString)to_string(i), "Time-of-Flight Simulation;Time [ns];Counts", numTofBins, minTof, maxTof);
     IndivPhotonTofSim[i] = new TH1D((TString)"IndivPhotonTofSim" + (TString)to_string(i), "Time-of-Flight Simulation;Time [ns];Counts", numTofBins, minTof, maxTof);
     IndivNeutronEnergySim[i] = new TH1D((TString)"IndivNeutronEnergySim" + (TString)to_string(i), "Neutron Energy Simulation;Energy [MeV];Counts", numErgBins, minErg, maxErg);
     IndivNeutronPSDSim[i] = new TH1D((TString)"IndivNeutronPSDSim" + (TString)to_string(i), "Neutron PSP; PSP (tail/total); counts",  numPSDBins, minPSP, maxPSP);
     IndivPhotonPSDSim[i] = new TH1D((TString)"IndivPhotonPSDSim" + (TString)to_string(i), "Photon PSP; PSP (tail/total); counts", numPSDBins, minPSP, maxPSP);
   }

   // correlated
   IndivNeutronEnergyLOExp[i] = new TH2D((TString)"IndivNeutronEnergyLOExp" + (TString)to_string(i), "Neutron Energy vs. Neutron Light Output; Neutron Energy [MeV]; Neutron Light Output [MeVee]; Counts", numErgBins, minErg, maxErg, numLObins, minLO, maxLO);
   IndivNeutronLightOutPSDExp[i] = new TH2D((TString)"IndivNeutronLightOutPSDExp" + (TString)to_string(i), "Neutron Light Output vs. Neutron PSD; Neutron Light Output [MeVee]; Neutron PSP [tail/total]; Counts", numLObins, minLO, maxLO, numPSDBins, minPSP, maxPSP);
   IndivPhotonLightOutPSDExp[i] = new TH2D((TString)"IndivPhotonLightOutPSDExp" + (TString)to_string(i), "Photon Light Output vs. Photon PSD; Photon Light Output [MeVee]; Photon PSP [tail/total]; Counts", numLObins, minLO, maxLO, numPSDBins, minPSP, maxPSP);
 }


 /*
  ___
 | _ ) ___ __ _ _ __
 | _ \/ -_) _` | '  \
 |___/\___\__,_|_|_|_|

 */

  // alphaFile histograms
  h_alphaDep = new TH1D* [NUM_TRIGGERS];

  //Projection histograms
  pj_pLightOutErg = new TH1D** [(int)BEAM_ERG_BINNUM];
  pj_nLightOutErg = new TH1D** [(int)BEAM_ERG_BINNUM];

  // beam histograms
  h_fisDep = new TH1D* [NUM_TRIGGERS];
  h_fisSubtract = new TH1D* [NUM_TRIGGERS];
  h2_fisDepErg = new TH2D* [NUM_TRIGGERS];
  h_beamTime = new TH1D* [NUM_TRIGGERS];

  h2_neutronMultDep = new TH2D* [NUM_TRIGGERS];
  h2_gammaMultDep = new TH2D* [NUM_TRIGGERS];
  h2_backNeutronMultDep = new TH2D* [NUM_TRIGGERS];
  h2_backGammaMultDep = new TH2D* [NUM_TRIGGERS];

  h2_neutronMultErg = new TH2D* [NUM_TRIGGERS];
  h2_gammaMultErg = new TH2D* [NUM_TRIGGERS];
  h2_backNeutronMultErg = new TH2D* [NUM_TRIGGERS];
  h2_backGammaMultErg = new TH2D* [NUM_TRIGGERS];

  h2_photonLightOutErg = new TH2D* [NUM_TRIGGERS];
  h2_nLightOutErg = new TH2D* [NUM_TRIGGERS];
  h2_nToFErg = new TH2D* [NUM_TRIGGERS];

  h2_nBackToFErg = new TH2D* [NUM_TRIGGERS];
  h2_photonBackLightOutErg = new TH2D* [NUM_TRIGGERS];
  h2_nBackLightOutErg = new TH2D* [NUM_TRIGGERS];

  for(int indexChannel = 0; indexChannel < NUM_TRIGGERS; indexChannel++)
  {
    // h_alphaDep[indexChannel] = new TH1D("h_alphaDep","Total PPAC events; Event energy (V us); counts", numfisDepBins, minDep, maxDep);
    // beam histograms
    h_fisDep[indexChannel]  = new TH1D((TString)"h_fisDep" + (TString)to_string(indexChannel), "Total Fission Spectrum; Event Energy (V us); counts",  numfisDepBins, minDep, maxDep);
    h_fisSubtract[indexChannel]  = new TH1D((TString)"h_fisSubtract" + (TString)to_string(indexChannel), "Total Fission Spectrum; Event Energy (V us); counts",  numfisDepBins, minDep, maxDep);
    h2_fisDepErg[indexChannel]  = new TH2D((TString)"h2_fisDepErg" + (TString)to_string(indexChannel), "Total Fission Spectrum vs Ei; Event Energy (V us); Incident Neutron Energy (MeV); counts", numfisDepBins, minDep, maxDep, BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX);
    h_beamTime[indexChannel]  = new TH1D((TString)"h_beamTime" + (TString)to_string(indexChannel), "Fission rate in Beam Window; Time within micro beam Index (ns); counts",  numfisBeamTimeBins, minBeamTime, maxBeamTime);

    h2_neutronMultDep[indexChannel]  = new TH2D((TString)"h2_neutronMultDep" + (TString)to_string(indexChannel), "Energy Dependent Neutron Multiplicity; Event Energy (V us); Neutron Multiplicity; counts", numfisDepBins, minDep, maxDep, maxMult, minMult-0.5, maxMult-0.5);
    h2_gammaMultDep[indexChannel]  =  new TH2D((TString)"h2_gammaMultDep" + (TString)to_string(indexChannel), "Energy Dependent Gamma Multiplicity; Event Energy (V us); Gamma Multiplicity; counts", numfisDepBins, minDep, maxDep, maxMult, minMult-0.5, maxMult-0.5);
    h2_backNeutronMultDep[indexChannel]  = new TH2D((TString)"h2_backNeutronMultDep"+ (TString)to_string(indexChannel), "Energy Dependent Background Neutron Multiplicity; Event Energy (V us); Neutron Multiplicity; counts", numfisDepBins, minDep, maxDep, maxMult, minMult-0.5, maxMult-0.5);
    h2_backGammaMultDep[indexChannel]  =  new TH2D((TString)"h2_backGammaMultDep"+ (TString)to_string(indexChannel), "Energy Dependent Background Gamma Multiplicity; Event Energy (V us); Gamma Multiplicity; counts", numfisDepBins, minDep, maxDep, maxMult, minMult-0.5, maxMult-0.5);

    h2_neutronMultErg[indexChannel]  = new TH2D((TString)"h2_neutronMultErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Neutron Multiplicity; Incident Neutron Energy (MeV); Neutron Multiplicity; counts", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, maxMult, minMult-0.5, maxMult-0.5);
    h2_gammaMultErg[indexChannel]  = new TH2D((TString)"h2_gammaMultErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Gamma Multiplicity; Event Energy (V us); Gamma Multiplicity; counts", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, maxMult, minMult-0.5, maxMult-0.5);
    h2_backNeutronMultErg[indexChannel] =  new TH2D((TString)"h2_backNeutronMultErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Neutron Multiplicity; Incident Neutron Energy (MeV); Neutron Multiplicity; counts", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, maxMult, minMult-0.5, maxMult-0.5);
    h2_backGammaMultErg[indexChannel]  = new TH2D((TString)"h2_backGammaMultErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Gamma Multiplicity; Event Energy (V us); Gamma Multiplicity; counts", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, maxMult, minMult-0.5, maxMult-0.5);

    pj_pLightOutErg[indexChannel] = new TH1D* [(int)BEAM_ERG_BINNUM];
    pj_nLightOutErg[indexChannel] = new TH1D* [(int)BEAM_ERG_BINNUM];

    h2_photonLightOutErg[indexChannel] = new TH2D((TString)"h2_photonLightOutErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Photon Light Output; Incident Neutron Energy (MeV); Photon Light Output (MeVee)", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numLObins, minLO, maxLO);
    h2_nLightOutErg[indexChannel] = new TH2D((TString)"h2_nLightOutErg"+ (TString)to_string(indexChannel), "Incident Energy Depnedent Neutron Light Output; Incident Neutron Energy (MeV); Neutron Light Output (MeVee)", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numLObins, minLO, maxLO);
    h2_nToFErg[indexChannel] = new TH2D((TString)"h2_nToFErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Neutron Time of Flight Energy; Incident Neutron Energy (MeV); Neutron Time of Flight Energy (MeV)", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numErgBins, minErg, maxErg);

    h2_nBackToFErg[indexChannel] = new TH2D((TString)"h2_nBackToFErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Background Neutron Time of Flight Energy; Incident Neutron Energy (MeV); Background Neutron Time of Flight Energy (MeV)", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numErgBins, minErg, maxErg);
    h2_photonBackLightOutErg[indexChannel] = new TH2D((TString)"h2_photonBackLightOutErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Background Photon Light Output; Incident Neutron Energy (MeV); Background Photon Light Output", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numLObins, minLO, maxLO);
    h2_nBackLightOutErg[indexChannel] = new TH2D((TString)"h2_nBackLightOutErg"+ (TString)to_string(indexChannel), "Incident Energy Dependent Background Neutron Light Output; Incident Neutron Energy (MeV); Background Neutron Light Output", BEAM_ERG_BINNUM, BEAM_ERG_MIN, BEAM_ERG_MAX, numLObins, minLO, maxLO);

  }

  //beam stack
  stack = new THStack* [NUM_TRIGGERS];

  // declare the profiles for the analysis
  p_neutronMultDep = new TProfile* [NUM_TRIGGERS];
  p_gammaMultDep = new TProfile* [NUM_TRIGGERS];
  p_backNeutronMultDep = new TProfile* [NUM_TRIGGERS];
  p_backGammaMultDep = new TProfile* [NUM_TRIGGERS];

  p_nToFErg = new TProfile* [NUM_TRIGGERS];
  p_photonLightOutErg = new TProfile* [NUM_TRIGGERS];
  p_nLightOutErg = new TProfile* [NUM_TRIGGERS];


  g_fisRatioThreshold = new TGraph* [NUM_TRIGGERS];
  g_fisRatioSelect = new TGraph* [NUM_TRIGGERS];

  g_neutronMultRatioDep = new TGraph* [NUM_TRIGGERS];
  g_gammaMultRatioDep = new TGraph* [NUM_TRIGGERS];

  for(int indexChannel = 0; indexChannel < NUM_TRIGGERS; indexChannel++)
  {
    g_fisRatioThreshold[indexChannel] = new TGraph(numfisDepBins);
    g_fisRatioSelect[indexChannel] = new TGraph(numfisDepBins);

    g_neutronMultRatioDep[indexChannel] = new TGraph(numfisDepBins);
    g_gammaMultRatioDep[indexChannel] = new TGraph(numfisDepBins);
  }

  // BeamErgAnalysis profiles and graphs
  p_neutronMultErg =     new TProfile* [NUM_TRIGGERS];
  p_gammaMultErg =       new TProfile* [NUM_TRIGGERS];
  p_backNeutronMultErg = new TProfile* [NUM_TRIGGERS];
  p_backGammaMultErg =   new TProfile* [NUM_TRIGGERS];

  g_fisRatioErg =  new TGraph* [NUM_TRIGGERS];
  g_nMultErg =     new TGraph* [NUM_TRIGGERS];
  g_gMultErg =     new TGraph* [NUM_TRIGGERS];
  g_nMultBackErg = new TGraph* [NUM_TRIGGERS];
  g_gMultBackErg = new TGraph* [NUM_TRIGGERS];

  g_gMultnMult =   new TGraph* [NUM_TRIGGERS];

  g_gnRatio = new TGraphErrors(NUM_TRIGGERS);
  g_nRatioSlopeInt = new TGraphErrors(NUM_TRIGGERS);
  g_gRatioSlopeInt = new TGraphErrors(NUM_TRIGGERS);

  for(int indexChannel = 0; indexChannel < NUM_TRIGGERS; indexChannel++)
  {
    g_fisRatioErg[indexChannel] =  new TGraph(BEAM_ERG_BINNUM);
    g_nMultErg[indexChannel] =     new TGraph(BEAM_ERG_BINNUM);
    g_gMultErg[indexChannel] =     new TGraph(BEAM_ERG_BINNUM);
    g_nMultBackErg[indexChannel] = new TGraph(BEAM_ERG_BINNUM);
    g_gMultBackErg[indexChannel] = new TGraph(BEAM_ERG_BINNUM);

    g_gMultnMult[indexChannel] =   new TGraph(BEAM_ERG_BINNUM);
  }

}
