#define CoincidenceAnalysis_cxx

#include "CoincidenceAnalysis.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

#include "InfoSystem.h"
#include "ProcessingConstants.h"

#include "CoincidenceEvent.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"
#include "BeamEvent.h"

#include "mappingFunctions.h"

using namespace std;

int CoincidenceAnalysis::CreateCoincidenceTree(Long64_t entriesToProc)
{
	/*
	  _  _ _    _
	 | || (_)__| |_ ___  __ _ _ _ __ _ _ __  ___
	 | __ | (_-<  _/ _ \/ _` | '_/ _` | '  \(_-<
	 |_||_|_/__/\__\___/\__, |_| \__,_|_|_|_/__/
	                    |___/
	*/
	cout << "Initializing coincidence THistogram " << endl;

	//TH1F* h_Dt = new TH1F("delT", "delT", 1000, 0, 5e12);
	TH1D* h_microTimeDiff = new TH1D("microDelta", "Time Interval Micro; Time Difference (ns); Counts", 1000, 1750, 1800);
	TH1D* h_macroTimeDiff = new TH1D("macroDelta", "Time Interval Macro; Time Difference (ns); Counts", 1000, 0, 5e7);
	TH1D* h_microTime = new TH1D("microTime", "Time in Micro; Time (ns); Counts", 10000, 0, 1e6);
	TH1I* h_macroPop = new TH1I("macroPop", "Population in Macro; Number of Micropulses; Counts", 500, 0, 500);

	TH1I* h_alphaCounts = new TH1I("alphaCounts", "Population in Macro; Number of Micropulses; Counts", 500, 0, 500);


	queue<BeamEvent> microStructure;



	/*
	  _______              _____            _                 _   _
	 |__   __|            |  __ \          | |               | | (_)
	    | |_ __ ___  ___  | |  | | ___  ___| | __ _ _ __ __ _| |_ _  ___  _ __
	    | |  __/ _ \/ _ \ | |  | |/ _ \/ __| |/ _  |  __/ _  | __| |/ _ \|  _ \
	    | | | |  __/  __/ | |__| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | |
	    |_|_|  \___|\___| |_____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|
  */

	cout << "Initializing coincidence TTrees " << endl;

	expFile->cd();

	coincTree = new TTree("CoincidenceTree", "Tree of Experimental Coincidences");


	// declatre the variables to store the fission branches
	// fission trigger
	int tMult = 0;
	double tTime = 0;
	double tDep = 0;
	int tChan = 0;
	double tPSP = 0;
	double tTail = 0;

	// fission beam
	double bTime = 0;
	double bErg = 0;
	int bChan = 0;
	int bIndex = 0;
	double bPSP = 0;
	double bTail = 0;

	// particles
	double totToF[MAX_MULTIPLICITY] = {0};
	double totPSP[MAX_MULTIPLICITY] = {0};
	double totDep[MAX_MULTIPLICITY] = {0};
	int totChan[MAX_MULTIPLICITY] = {0};
	int totTail[MAX_MULTIPLICITY] = {0};

	// trigger variables
	coincTree->Branch("tMult", &tMult, "tMult/I");
	coincTree->Branch("tTime", &tTime, "fissionTime/D");
	coincTree->Branch("tDep", &tDep, "fissionErg/D");
	coincTree->Branch("tChan", &tChan, "fissionChan/I");
	coincTree->Branch("tPSP", &tPSP, "fissionPSP/D");
	coincTree->Branch("tTail", &tTail, "fissionTail/D");

	// beam variables
	if(NUM_BEAMS > 0)
	{
		coincTree->Branch("bTime", &bTime, "beamTime/D");
		coincTree->Branch("bErg", &bErg, "beamEnergy/D");
		coincTree->Branch("bChan", &bChan, "beamChan/I");
		coincTree->Branch("bIndex", &bIndex, "beamIndex/I");
		coincTree->Branch("bPSP", &bPSP, "beamPSP/D");
		coincTree->Branch("bTail", &bTail, "beamTail/D");
	}

	// list variables
	coincTree->Branch("totToF", totToF, "totToF[tMult]/D");
	coincTree->Branch("totPSP", totPSP, "totPSP[tMult]/D");
	coincTree->Branch("totDep", totDep, "totDep[tMult]/D");
	coincTree->Branch("totChan", totChan, "totChan[tMult]/I");
	coincTree->Branch("totTail", totTail, "totTail[tMult]/D");

	coincTree->SetMaxTreeSize(100000LL);


	/*
		   _____      _            _     _                       _
		  / ____|    (_)          (_)   | |                     | |
		 | |     ___  _ _ __   ___ _  __| | ___ _ __   ___ ___  | |     ___   ___  _ __
		 | |    / _ \| | '_ \ / __| |/ _` |/ _ \ '_ \ / __/ _ \ | |    / _ \ / _ \| '_ \
		 | |___| (_) | | | | | (__| | (_| |  __/ | | | (_|  __/ | |___| (_) | (_) | |_) |
		  \_____\___/|_|_| |_|\___|_|\__,_|\___|_| |_|\___\___| |______\___/ \___/| .__/
		                                                                          | |
		                                                                          |_|
	*/
		cout << "Initializing coincidence queue " << endl;


		// keep track of the iterator in each of the channels
		Long64_t *indexTrig = new Long64_t[NUM_TRIGGERS];

		// keep track of the earliest times in each of the channels
		double *DetectorLastTime = new double[NUM_DETS];

		// distribution of chamber times and energy
		double *chamberTimes = new double[NUM_TRIGGERS];
		double *chamberErgs = new double[NUM_TRIGGERS];
		double *chamberPSP = new double[NUM_TRIGGERS];
		int chamberChan = 0;

		// initialize the new particle
		TriggerEvent qTrigger = TriggerEvent();
		BeamEvent qBeam = BeamEvent();
		CoincidenceEvent newFission = CoincidenceEvent(0, 0, 0, 0, 0);

		// start at the beginning of the array and also keep track of the the number of coincidence events found.
		ULong64_t numFissEvents = 0;

		// initialize fission tracker
		bool validFiss = true;
		double averageTrigTime = 0;
		double sumTrigErg = 0;
		double averageTrigPSP = 0;

		long double beginTime = 0;

		double firstMicro;
		double lastMicro;
		double timeDiffMacro;
		double timeDiffMicro;

		double microTime;
		double timeHead;

	  int microIndex;

		// variables for validation
		CoincidenceEvent curFis;
		CoincidenceEvent goodFis;
		BeamEvent curBeam;

		double deltaFissBeam;
		double curFisTime, curBeamTime;
		/*
			______ _         _               _
		 |  ____(_)       (_)             | |
		 | |__   _ ___ ___ _  ___  _ __   | |     ___   ___  _ __
		 |  __| | / __/ __| |/ _ \| '_ \  | |    / _ \ / _ \| '_ \
		 | |    | \__ \__ \ | (_) | | | | | |___| (_) | (_) | |_) |
		 |_|    |_|___/___/_|\___/|_| |_| |______\___/ \___/| .__/
																												| |
																												|_|
		*/
		cout << "Initializing fission queue. " << endl;

		// now loop thrugh fission events to find valid fission events
		CoincidenceEvent qFission = CoincidenceEvent(0, 0, 0, 0, 0);

		// trigger variables
		double fissionTime = 0;
		double fissionEnergy = 0;
		int fissionChan = 0;
		double fissionPSP = 0;
		double fissionTail = 0;

		// same but for beam variables
		double beamPSP, beamTime, beamEnergy, beamTail;
		int beamChan, beamIndex;

		// dynamical variables
		double detTime = 0;
		double detPSP = 0;

		// store multiplicities
		int totMult = 0;

		// find coincidences option
		ParticleEvent qParticle = ParticleEvent();

		// time to compare to fission
		double compDetTime;
		double deltaT;

		// keep track of the fission index
		long int fisTracker = 0;



	/*
	  __  __       _         _
	 |  \/  |     (_)       | |
	 | \  / | __ _ _ _ __   | |     ___   ___  _ __
	 | |\/| |/ _` | | '_ \  | |    / _ \ / _ \| '_ \
	 | |  | | (_| | | | | | | |___| (_) | (_) | |_) |
	 |_|  |_|\__,_|_|_| |_| |______\___/ \___/| .__/
	                                          | |
	                                          |_|
	*/

	// get the number of entries
	Long64_t nentries = fChain->GetEntries();
	//nentries = 1000000; // for debug
	cout << "For this file there are: " << nentries << " entries." << endl;

	if(entriesToProc > 0)
	{
		cout << "Reading the first " << entriesToProc << " events." << endl;
		nentries = entriesToProc;
	}

	// keep track of the number of bytes in the chain
	Long64_t nbytes = 0, nb = 0;

	// initialize the particle
	ParticleEvent newParticle = ParticleEvent();
	TriggerEvent newTrigger = TriggerEvent();
	BeamEvent newBeam = BeamEvent();

	// initialize the detector channel
	int detChannel = 0;
	double timeDet = 0;
	double energyDep = 0;
	double energyTail = 0;
	int entryChannel = 0;

	int refTrig = 0;
	double refTrigTime = 0;
	double compTrigTime = 0;

	bool pastRef, trigReady;

	double timeDel = 0;
	int fileInd = 0;

	cout << "Beam delay set to: " << BEAM_DELAY << endl;

	// cout << TRIGGER_THRESHOLD << " " << TRIGGER_CLIP << endl;

	// loop through the raw data
	for (Long64_t jentry = 0; jentry < nentries; jentry++)
	{

		// if(jentry%100000 == 0)
		// {
		// 	cout << jentry << endl;
		// }

		Long64_t ientry = LoadTree(jentry);
	  	if (ientry < 0) break;



		// load current entry
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		//correct for time delay
		if(ientry == 0)
		{
			cout << "Loading file number " << fileInd++ << endl;
			timeDel = timeDet;
			// cout << timeDel << endl;
		}

		//  ___ _ _ _ _
		// | __(_) | (_)_ _  __ _
		// | _|| | | | | ' \/ _` |
		// |_| |_|_|_|_|_||_\__, |
		// 							   	|___/

		// choose the correct data format
		if(DATA_TYPE == 0) //compass type digitizer
		{
			detChannel = cp->getDetector();
			timeDet = cp->getTime() + timeDel;
			energyDep = cp->getEnergy();
			energyTail = cp->getTail();

		}
		else if(DATA_TYPE == 1) //midas type digitizer
		{
			detChannel = md->getDetector();
			timeDet = md->getTime() + timeDel;
			energyDep = md->getEnergy();
			energyTail = md->getTail();
		}

		// new events is in a detector
		if(isDetector(detChannel, NUM_DETS, DETECTORS) >= 0)
		{
			entryChannel = isDetector(detChannel, NUM_DETS, DETECTORS);
			newParticle = ParticleEvent(detChannel, timeDet - BEAM_DELAY, energyDep, energyTail);
			DetectorBuffer[entryChannel].push(newParticle);
		}

		// new event is in a trigger
		else if(isTrigger(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS) >= 0)
		{
			entryChannel = isTrigger(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS);
			newTrigger = TriggerEvent(detChannel, timeDet - BEAM_DELAY, energyDep, energyTail);

			// energy discrimination of fission
			if((newTrigger.getEnergy() > TRIGGER_THRESHOLD) & (newTrigger.getEnergy() < TRIGGER_CLIP) & (newTrigger.getPsp() > TRIGGER_MIN_PSP) & (newTrigger.getPsp() < TRIGGER_MAX_PSP) )
			{
				// cout << newTrigger.getEnergy() << endl;
				TriggerBuffer[entryChannel].push(newTrigger);
				// cout << TriggerBuffer[entryChannel].size() << endl;
			}
		}

		else if(isBeam(detChannel, NUM_BEAMS, BEAM) >= 0)
		{
			entryChannel = isBeam(detChannel, NUM_BEAMS, BEAM);
			newBeam = BeamEvent(detChannel, timeDet, energyDep, energyTail);
			BeamBuffer[entryChannel].push(newBeam);
			// cout << newBeam.getEnergy() << endl;
		}

		// else
		// {
		// 	cout << detChannel << " found in the data, and ";
		// 	cout << "channel not recognized, stopping the processing and check your input file" << endl;
		// 	exit(3);
		// }


		/*
		 ____                         _
		|  _ \                       | |
		| |_) | ___  __ _ _ __ ___   | |     ___   ___  _ __
		|  _ < / _ \/ _` | '_ ` _ \  | |    / _ \ / _ \| '_ \
		| |_) |  __/ (_| | | | | | | | |___| (_) | (_) | |_) |
		|____/ \___|\__,_|_| |_| |_| |______\___/ \___/| .__/
																									| |
																									|_|
		*/
		// cout << microStructure.size() << endl;

		// loop through the beams (usually only one)
		for(int beamIndex = 0; beamIndex < NUM_BEAMS; beamIndex++)
		{

			// if there is information stored in the beam queue, proceed to analyze
			if(!BeamBuffer[beamIndex].empty())
			{
				curBeam = BeamBuffer[beamIndex].front();
				curBeamTime = curBeam.getTime();

				// micropulse structure is empty, assume new beam is part of a macro
				if(microStructure.size() == 0)
				{
					microStructure.push(curBeam);
					BeamBuffer[beamIndex].pop();
				}

				// a macro already exists, compare current beam to the existing one
				else
				{
					firstMicro = microStructure.front().getTime();
					lastMicro = microStructure.back().getTime();
					timeDiffMacro = 0;
					timeDiffMicro = 0;

					while(!BeamBuffer[beamIndex].empty())
					{
						timeDiffMacro =  curBeamTime - firstMicro;

						if(timeDiffMacro < MACRO_SEP) // within macro pulse
						{
							timeDiffMicro = curBeamTime - lastMicro;
							h_microTimeDiff->Fill(timeDiffMicro);
							microStructure.push(curBeam);
						}

						else // next macropulse, now analyze the present microstructure
						{

							h_macroTimeDiff->Fill(timeDiffMacro);

							timeHead = microStructure.front().getTime(); // this is the last micro in the macropulse

							h_macroPop->Fill(microStructure.size()); // population of macropulse

							// if(microStructure.size() > MICRO_NUM)
							// {
							// 	microStructureCoinc = microStructure;
							// }
							microIndex = 0;
							while(!microStructure.empty())
							{
								qBeam = microStructure.front();
								qBeam.microIndex = microIndex; microIndex++;

								microTime = qBeam.getTime() - timeHead;
								h_microTime->Fill(microTime);

								ValidBeamBuffer.push(qBeam);
								microStructure.pop();

							}

							microStructure.push(curBeam);
						}

						BeamBuffer[beamIndex].pop();
						// cout << "B" << endl;
					}
				}

			}

		}


		//  _____    _
		// |_   _| _(_)__ _ __ _ ___ _ _
		//  | || '_| / _` / _` / -_) '_|
		//  |_||_| |_\__, \__, \___|_|
		// 					 |___/|___/

		// if trigger is not split, check if you can store in a fission.
		// This is the standard operational mode
		if(!TRIGGER_SPLIT)
		{
			// check whether all trigs queues are populated
			bool allFill = true;

			while(allFill)
			{
				for(int trigIndex = 0; trigIndex < NUM_TRIGGERS; trigIndex++)
				{
					if(TriggerBuffer[trigIndex].empty())
					{
						allFill = false;
						// cout << trigIndex << " " << TriggerBuffer[trigIndex].empty() <<  endl;
					}
				}

				// find minimum trigger
				if(allFill)
				{
					// cout << "oh hello" << endl;
					int recentIndex = 0;
					for(int trigIndex=0; trigIndex<NUM_TRIGGERS; trigIndex++)
					{
						if(TriggerBuffer[trigIndex].front().getTime() < TriggerBuffer[recentIndex].front().getTime())
						{
						   recentIndex = trigIndex;
						}
					}

					newFission = CoincidenceEvent(TriggerBuffer[recentIndex].front().getTime(),
					 															TriggerBuffer[recentIndex].front().getEnergy(),
																				TriggerBuffer[recentIndex].front().getDetector(),
																				TriggerBuffer[recentIndex].front().getPsp(),
																		   	TriggerBuffer[recentIndex].front().getTail());


					// we have found the first fission event, check whether this can be put in coincidence with beam

					ValidTriggerBuffer.push(newFission);

					TriggerBuffer[recentIndex].pop();
					// cout << "popped, now " << recentIndex << ": " << TriggerBuffer[recentIndex].size() << endl;
				}

				// cout << "T" << endl;
			}
		}

		// if trigger split, check if you can form a coincidence between the triggers
		else if(TRIGGER_SPLIT)
		{

			trigReady = true;
			pastRef = false;

			if(!TriggerBuffer[refTrig].empty())
			{
				refTrigTime = TriggerBuffer[refTrig].front().getTime();

				// cycle through the other triggers and make sure that they are empty
				for(int trigIndex = 1; trigIndex < NUM_TRIGGERS; trigIndex++)
				{
					// check whether more data needs to be read in
					if(!TriggerBuffer[trigIndex].empty())
					{
						compTrigTime = TriggerBuffer[trigIndex].front().getTime();
					}

					// eliminate bad coincidence events
					while( (!TriggerBuffer[trigIndex].empty() ) &  (refTrigTime - compTrigTime > MAX_TRIGGER_DRIFT) )
					{
						TriggerBuffer[trigIndex].pop();
						compTrigTime = TriggerBuffer[trigIndex].front().getTime();
					}

					// check whether more data needs to be read in
					if(TriggerBuffer[trigIndex].empty())
					{
						// split fission not reached yet
						trigReady = false;
					}

					// coincidence can be formed
					else if(abs(compTrigTime - refTrigTime) < MAX_TRIGGER_DRIFT)
					{
						// h_Dt->Fill(compTrigTime - refTrigTime);
						trigReady = true; // does this work, or does it cancel 2/3
					}

					// reference channel is lagging
					else if(compTrigTime - refTrigTime > MAX_TRIGGER_DRIFT)
					{
						trigReady = false;
						pastRef = true;
					}
				}
			}

			// if the time is right for coincidence, create a new fission
			if(trigReady)
			{
				newFission = CoincidenceEvent(TriggerBuffer[refTrig].front().getTime(),
				 															TriggerBuffer[refTrig].front().getEnergy(),
																			TriggerBuffer[refTrig].front().getDetector(),
																			TriggerBuffer[refTrig].front().getPsp(),
																		  TriggerBuffer[refTrig].front().getTail());
				ValidTriggerBuffer.push(newFission);
				TriggerBuffer[refTrig].pop();
			}

			// if the reference channel is outdated, pop and proceed
			else if(pastRef)
			{
				TriggerBuffer[refTrig].pop();
			}

		}

//  ___ _           ___                  __   __    _ _    _      _   _
// | __(_)_________| _ ) ___ __ _ _ __   \ \ / /_ _| (_)__| |__ _| |_(_)___ _ _
// | _|| (_-<_-<___| _ \/ -_) _` | '  \   \ V / _` | | / _` / _` |  _| / _ \ ' \
// |_| |_/__/__/   |___/\___\__,_|_|_|_|   \_/\__,_|_|_\__,_\__,_|\__|_\___/_||_|


		// At the end of the trigger loop we have found a valid, chronological fission event

		// if no beam are present, just set fission events to loop events
		if(NUM_BEAMS == 0)
		{
			// depopulate the valid trigger and populate the fission trigger
			while(ValidTriggerBuffer.size() > 0)
			{
				curFis = ValidTriggerBuffer.front();
				FissionBuffer.push(curFis);
				ValidTriggerBuffer.pop();
			}
		}

		// if beams are present, compare the earliest events
		else
		{
			// cout << ValidBeamBuffer.size() << " " << ValidTriggerBuffer.size() << endl;

			while(!ValidBeamBuffer.empty() & !ValidTriggerBuffer.empty())
			{
				// cout << ValidBeamBuffer.size() << " " << ValidTriggerBuffer.size() << endl;

				// get the front of the queue
				curFis = ValidTriggerBuffer.front();
				curBeam = ValidBeamBuffer.front();;

				// get the times
				curFisTime = curFis.triggerTime;
				curBeamTime = curBeam.getTime();

				deltaFissBeam = curFisTime - curBeamTime;

				// cout << deltaFissBeam << endl;

				if(deltaFissBeam >= BEAM_WINDOW)
				{
						// the beam store in memory did not trigger a fission reaction
						ValidBeamBuffer.pop();
				}

				else if(deltaFissBeam <= -1*BEAM_WINDOW)
				{
					// the fission stored in memory does not belong to a beam event
					ValidTriggerBuffer.pop();
				}

				else if(abs(deltaFissBeam) < BEAM_WINDOW)
				{
						// the fission reaction is associated with the current beam and a new
						// accepted fission is created

						goodFis = CoincidenceEvent(curFis.triggerTime,
						 													 curFis.triggerEnergy,
																			 curFis.triggerChannel,
																			 curFis.triggerPSP,
																			 curFis.triggerTail);

						goodFis.beamTime = deltaFissBeam;
						goodFis.beamEnergy = curBeam.getEnergy();
						goodFis.beamPSP = curBeam.getPsp();
						goodFis.beamChannel = curBeam.getDetector();
						goodFis.beamMicroIndex = curBeam.getMicroIndex();

						FissionBuffer.push(goodFis);
						ValidTriggerBuffer.pop(); // discard only the fission, there could be several fiss for same beam.
				}

				else
				{
					cout << "Computation failed on " << deltaFissBeam << endl;
					exit(4);
				}

				// cout << "FB" << endl;
			}
		}






		//  ___ _       _
		// | __(_)_____(_)___ _ _
		// | _|| (_-<_-< / _ \ ' \
		// |_| |_/__/__/_\___/_||_|
		//

		// before we can fill the detectors, we need to make sure that there are
		// enough events to start populating the fissions
		bool readyDet = true;
		// look at the detection events
		for(int detIndex = 0; detIndex < NUM_DETS; detIndex++)
		{
			// assign detection time
			if(DetectorBuffer[detIndex].empty())
			{
				readyDet = false;
			}
		}

		// cout << FissionBuffer.size() << endl;

		// if the detector buffers are ready, proceed to go through the fission and check
		// whether there are particle-trigger coincidences
		while(!FissionBuffer.empty() & readyDet)
		{

			// cout << "F" << endl;



			totMult = 0;

			// reset all the indices to 0
			for(int jMult = 0; jMult < MAX_MULTIPLICITY; jMult++)
			{
				totToF[jMult] = 0;
				totDep[jMult] = 0;
				totPSP[jMult] = 0;
				totChan[jMult] = 0;
			}

			// assign fission event from first list
			qFission = FissionBuffer.front();

			// assign the detector channel
			fissionChan = qFission.getTriggerChan();
			fissionTime = qFission.getTriggerTime();
			fissionEnergy = qFission.getEnergy();
			fissionPSP = qFission.getTriggerPSP();
			fissionTail = qFission.getTriggerTail();


			beamTime = qFission.getBeamTime();
			beamChan = qFission.getBeamChan();
			beamEnergy = qFission.getBeamEnergy();
			beamPSP = qFission.getBeamPSP();
			beamIndex = qFission.getBeamIndex();
			beamTail = qFission.getBeamTail();

			// look at the detection events
			for(int detIndex = 0; detIndex < NUM_DETS; detIndex++)
			{
				// assign detection time
				if(!DetectorBuffer[detIndex].empty())
				{
					compDetTime = DetectorBuffer[detIndex].front().getTime();
					// cout << fissionTime << " - " << compDetTime << " = " << fissionTime - compDetTime <<  endl;
				}

				// eliminate bad coincidence events
				while((!DetectorBuffer[detIndex].empty() ) & (fissionTime - compDetTime > COINC_WINDOW) )
				{
					// cout << fissionTime - compDetTime << endl;
					DetectorBuffer[detIndex].pop();
					compDetTime = DetectorBuffer[detIndex].front().getTime();
					// cout << "time checked" << endl;
				}

				// if the next event is empty, we don't know for sure if it was in coincidence, back to the drawing board

				if(DetectorBuffer[detIndex].empty())
				{
					readyDet = false;
					//cout << detIndex << endl;
				}

			}

			// now we have made sure that all the detectors contributed an event if they had one

			if(readyDet)
			{

				for(int detIndex = 0; detIndex < NUM_DETS; detIndex++)
				{
					qParticle = DetectorBuffer[detIndex].front();
					detTime = qParticle.getTime();
					deltaT = detTime - fissionTime;

					// create the coincidence event
					if(abs(deltaT) < COINC_WINDOW)
					{
						totToF[totMult] = deltaT;
						totPSP[totMult] = qParticle.getPsp();
						totDep[totMult] = qParticle.getEnergy();
						totTail[totMult] = qParticle.getTail();
						totChan[totMult] = qParticle.getDetector();
						totMult++;
					}
				}

				// get rid of analyzed fission event
				FissionBuffer.pop();
				// cout << "pop" <<  FissionBuffer.size() << endl;

				// now fill the histogram of particle-particle coincidences
				tMult = totMult;
				tTime = fissionTime;
				tDep = fissionEnergy;
				tPSP = fissionPSP;
				tChan = fissionChan;
				tTail = fissionTail;

				if(NUM_BEAMS >0)
				{
					bErg = beamEnergy;
					bPSP = beamPSP;
					bChan = beamChan;
					bIndex = beamIndex;
					bTime = beamTime;
					bTail = beamTail;
				}

				// fill the tree branches
				coincTree->Fill();
				fisTracker++;

				// cout << tTime << " " << tMult << " " << tDep << " " << tPSP << " " << tChan << endl;

				// update user on status of processing
				if(fisTracker%100000 == 0)
				{
					cout << fisTracker << " fissions in " << tTime/1e9 << " seconds " << endl;
				}

			}

		 }

		}

		if(NUM_BEAMS > 0)
		{
			h_microTimeDiff->Write();
			h_macroTimeDiff->Write();
			h_microTime->Write();
			h_macroPop->Write();
		}



	 //   _____             _               _____        _
	 //  / ____|           (_)             |  __ \      | |
	 // | (___   __ ___   ___ _ __   __ _  | |  | | __ _| |_ __ _
	 //  \___ \ / _` \ \ / / | '_ \ / _` | | |  | |/ _` | __/ _` |
	 //  ____) | (_| |\ V /| | | | | (_| | | |__| | (_| | || (_| |
	 // |_____/ \__,_| \_/ |_|_| |_|\__, | |_____/ \__,_|\__\__,_|
	 //                              __/ |
	 //                             |___/


	// cd back into the main file
	cout << "Saving the tree to file. " << endl;

	cout << "In total: " << fisTracker << " fissions formed. " << endl;

	// coincTree->Write();
	expFile = coincTree->GetCurrentFile();
	expFile->Write();
	//expFile->Close();
	//h_Dt->Write();

	return 1;
}
