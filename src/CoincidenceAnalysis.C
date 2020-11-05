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

	TH1F* h_Dt = new TH1F("delT", "delT", 1000, 0, 5e12);



	/*
	  _______              _____            _                 _   _
	 |__   __|            |  __ \          | |               | | (_)
	    | |_ __ ___  ___  | |  | | ___  ___| | __ _ _ __ __ _| |_ _  ___  _ __
	    | |  __/ _ \/ _ \ | |  | |/ _ \/ __| |/ _  |  __/ _  | __| |/ _ \|  _ \
	    | | | |  __/  __/ | |__| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | |
	    |_|_|  \___|\___| |_____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|
  */

	cout << "Initializing coincidence TTree " << endl;

	expFile->cd();

	coincTree = new TTree("CoincidenceTree", "Tree of Experimental Coincidences");


	// declatre the variables to store the fission branches
	int tMult = 0;
	double tTime = 0;
	double tDep = 0;
	int tChan = 0;
	double tPSP = 0;


	double totToF[MAX_MULTIPLICITY] = {0};
	double totPSP[MAX_MULTIPLICITY] = {0};
	double totDep[MAX_MULTIPLICITY] = {0};
	double totTail[MAX_MULTIPLICITY] = {0};
	int totChan[MAX_MULTIPLICITY] = {0};

	// scalar variables
	coincTree->Branch("tMult", &tMult, "tMult/I");
	coincTree->Branch("tTime", &tTime, "fissionTime/D");
	coincTree->Branch("tDep", &tDep, "fissionErg/D");
	coincTree->Branch("tChan", &tChan, "fissionChan/I");
	coincTree->Branch("tPSP", &tPSP, "fissionPSP/D");

	// list variables
	coincTree->Branch("totToF", totToF, "totToF[tMult]/D");
	coincTree->Branch("totPSP", totPSP, "totPSP[tMult]/D");
	coincTree->Branch("totDep", totDep, "totDep[tMult]/D");
	coincTree->Branch("totChan", totChan, "totChan[tMult]/I");
	// coincTree->Branch("totTail", totTail, "totTail[tMult]/D");


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
		//CoincidenceEvent newFission = CoincidenceEvent(0, 0);
		CoincidenceEvent newFission = CoincidenceEvent(0, 0, 0, 0);

		// start at the beginning of the array and also keep track of the the number of coincidence events found.
		ULong64_t numFissEvents = 0;

		// initialize fission tracker
		bool validFiss = true;
		double averageTrigTime = 0;
		double sumTrigErg = 0;
		double averageTrigPSP = 0;

		long double beginTime = 0;


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
		CoincidenceEvent qFission = CoincidenceEvent(0, 0, 0, 0);
		double fissionTime = 0;
		double fissionEnergy = 0;
		int fissionChan = 0;
		double fissionPSP = 0;

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
		int fisTracker = 0;



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
	// nentries = 100000; for debug
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

	// loop through array
	for (Long64_t jentry = 0; jentry < nentries; jentry++)
	{

		Long64_t ientry = LoadTree(jentry);
	  	if (ientry < 0) break;

		// load current entry
		nb = fChain->GetEntry(jentry);   nbytes += nb;


		//  ___ _ _ _ _
		// | __(_) | (_)_ _  __ _
		// | _|| | | | | ' \/ _` |
		// |_| |_|_|_|_|_||_\__, |
		// 							   	|___/

		// choose the correct data format
		if(DATA_TYPE == 0) //compass type digitizer
		{
			detChannel = cp->getDetector();
			timeDet = cp->getTime();
			energyDep = cp->getEnergy();
			energyTail = cp->getTail();

		}
		else if(DATA_TYPE == 1) //midas type digitizer
		{
			detChannel = md->getDetector();
			timeDet = md->getTime();
			energyDep = md->getEnergy();
			energyTail = md->getTail();
		}

		// new events is in a detector
		if(isDetector(detChannel, NUM_DETS, DETECTORS) >= 0)
		{
			entryChannel = isDetector(detChannel, NUM_DETS, DETECTORS);
			newParticle = ParticleEvent(detChannel, timeDet, energyDep, energyTail);
			DetectorBuffer[entryChannel].push(newParticle);
		}

		// new event is in a trigger
		else if(isTrigger(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS) >= 0)
		{
			entryChannel = isTrigger(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS);
			newTrigger = TriggerEvent(detChannel, timeDet, energyDep, energyTail);

			// energy discrimination of fission
			if((newTrigger.getEnergy() < TRIGGER_THRESHOLD) || (newTrigger.getEnergy() > TRIGGER_CLIP) || (newTrigger.getPsp() > TRIGGER_MIN_PSP) || (newTrigger.getPsp() < TRIGGER_MAX_PSP) )
			{
				TriggerBuffer[entryChannel].push(newTrigger);

				// for(int trigIndex = 0; trigIndex < NUM_TRIGGERS; trigIndex++)
				// {
				// 	cout << trigIndex << " " << TriggerBuffer[trigIndex].size() << endl;
				// }

			}
		}

		//  _____    _
		// |_   _| _(_)__ _ __ _ ___ _ _
		//  | || '_| / _` / _` / -_) '_|
		//  |_||_| |_\__, \__, \___|_|
		// 					 |___/|___/

		// if trigger is not split, check if you can store in a fission
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
																				TriggerBuffer[recentIndex].front().getPsp());
					FissionBuffer.push(newFission);

					TriggerBuffer[recentIndex].pop();

					// cout << "tM: " << TriggerBuffer[recentIndex].front().getTime() << endl;


					// cout << FissionBuffer.front().getTriggerTime();
				}
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
						h_Dt->Fill(compTrigTime - refTrigTime);
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
																			TriggerBuffer[refTrig].front().getPsp());
				FissionBuffer.push(newFission);
				TriggerBuffer[refTrig].pop();
			}

			// if the reference channel is outdated, pop and proceed
			else if(pastRef)
			{
				TriggerBuffer[refTrig].pop();
			}

		}



		//  ___ _       _
		// | __(_)_____(_)___ _ _
		// | _|| (_-<_-< / _ \ ' \
		// |_| |_/__/__/_\___/_||_|
		//

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

		while(!FissionBuffer.empty() & readyDet)
		{

			// cout << "Fissions in buffer: " << FissionBuffer.size() << endl;

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

				// cout << DetectorBuffer[detIndex].empty() << " ";
				if(DetectorBuffer[detIndex].empty())
				{
					readyDet = false;
				}

			}

			if(FissionBuffer.size() > 1e6)
			{
				for(int detIndex = 0; detIndex < NUM_DETS; detIndex++)
				{
					cout << DetectorBuffer[detIndex].size() << " ";
				}
				cout << "\n";
			}

			// cout << "\n";

			if(readyDet)
			{
				// cout << "hello " << endl;

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

				// now fill the histogram of particle-particle coincidences
				tMult = totMult;
				tTime = fissionTime;
				tDep = fissionEnergy;
				tPSP = fissionPSP;
				tChan = fissionChan;

				// fill the tree branches
				coincTree->Fill();
				fisTracker++;

				// cout << tTime << " " << tMult << " " << tDep << " " << tPSP << " " << tChan << endl;

				// update user on status of processing
				if(fisTracker%1000000 == 0)
				{
					cout << fisTracker << " fissions in " << tTime/1e9 << "seconds " << endl;
				}
			}

		 }
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

	coincTree->Write();
	h_Dt->Write();

	return 1;
}
