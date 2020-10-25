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

int CoincidenceAnalysis::CreateCoincidenceTree(int fileNum, Long64_t entriesToProc)
{

	TH1F* hDt = new TH1F("delT", "delT", 1000, 0, 5e12);

	 //  _                          ____  _     _           _
	 // | |                        / __ \| |   (_)         | |
	 // | |     ___   ___  _ __   | |  | | |__  _  ___  ___| |_ ___
	 // | |    / _ \ / _ \| '_ \  | |  | | '_ \| |/ _ \/ __| __/ __|
	 // | |___| (_) | (_) | |_) | | |__| | |_) | |  __/ (__| |_\__ \
	 // |______\___/ \___/| .__/   \____/|_.__/| |\___|\___|\__|___/
	 //                   | |                 _/ |
	 //                   |_|                |__/

	// use an array of fifo to store particles and chambers
	queue<TriggerEvent> TriggerBuffer[NUM_TRIGGERS];
	queue<ParticleEvent> DetectorBuffer[NUM_DETS];


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
	Long64_t nentries = fChain->GetEntriesFast();
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

	// debugging
	double oldTime = 0;

	//cout << NUM_TRIGGERS << endl;

	// loop through array
	for (Long64_t jentry = 0; jentry < nentries; jentry++)
	{

		Long64_t ientry = LoadTree(jentry);
	    if (ientry < 0) break;

		// load current entry
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		// update user on status of processing
		if(jentry%1000000 == 0)
		{
			cout << "now reading entry " << jentry << endl;
		}


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

		//cout << detChannel << endl;

		//cout << isDetector(detChannel, NUM_DETS, DETECTORS) << endl;
		//cout << isChamber(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS) << endl;



		if(isDetector(detChannel, NUM_DETS, DETECTORS) >= 0)
		{
			newParticle = ParticleEvent(detChannel, timeDet, energyDep, energyTail);
			DetectorBuffer[isDetector(detChannel, NUM_DETS, DETECTORS)].push(newParticle);
		}
		else if(isChamber(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS) >= 0)
		{
			newTrigger = TriggerEvent(detChannel, timeDet, energyDep, energyTail);
			TriggerBuffer[isChamber(detChannel, NUM_TRIGGERS, FISSION_TRIGGERS)].push(newTrigger);
		}



	}


	// output the content of the buffers

	cout << endl;
	cout << "Buffers are ready: " << endl;


	cout << endl;
	cout << "Triggers: " << NUM_TRIGGERS << endl;

	for(int d = 0; d < NUM_TRIGGERS; d++)
	{
		cout << "trigger at " << d << ": "  << FISSION_TRIGGERS[d] << ": " << TriggerBuffer[d].size() << endl;
	}

	cout << endl;
	cout << "Detectors: " << NUM_DETS << endl;

	for(int d = 0; d < NUM_DETS; d++)
	{
		cout << "channel at "  << d <<  ": " << DETECTORS[d] << ": " << DetectorBuffer[d].size() << endl;
	}

	cout << endl;

	/*
	  _______              _____            _                 _   _
	 |__   __|            |  __ \          | |               | | (_)
	    | |_ __ ___  ___  | |  | | ___  ___| | __ _ _ __ __ _| |_ _  ___  _ __
	    | |  __/ _ \/ _ \ | |  | |/ _ \/ __| |/ _  |  __/ _  | __| |/ _ \|  _ \
	    | | | |  __/  __/ | |__| |  __/ (__| | (_| | | | (_| | |_| | (_) | | | |
	    |_|_|  \___|\___| |_____/ \___|\___|_|\__,_|_|  \__,_|\__|_|\___/|_| |_|
*/

	expFile->cd();
	fileTreeDir->cd();

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
	// ISABEL
	coincTree->Branch("tChan", &tChan, "fissionChan/I");
	coincTree->Branch("tPSP", &tPSP, "fissionPSP/D");

	// list variables
	coincTree->Branch("totToF", totToF, "totToF[tMult]/D");
	coincTree->Branch("totPSP", totPSP, "totPSP[tMult]/D");
	coincTree->Branch("totDep", totDep, "totDep[tMult]/D");
	coincTree->Branch("totChan", totChan, "totChan[tMult]/I");
	coincTree->Branch("totTail", totTail, "totTail[tMult]/D");


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

	// queue containing valid fission events
	queue<CoincidenceEvent> FissionBuffer;

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

	long double beginTime = TriggerBuffer[0].front().getTime();

	if(!TRIGGER_SPLIT) {
		//cout << "hello\n";
		bool allEmpty = false;
		while(!allEmpty)
		{

			bool validFiss = true;

			//find most recent fission trigger
			int recentIndex = 0;
			for(int chambIndex=0; chambIndex<NUM_TRIGGERS; chambIndex++)
			{
				while(TriggerBuffer[recentIndex].empty()) {
					recentIndex++;
				}
				if(!TriggerBuffer[chambIndex].empty()) {
					if(TriggerBuffer[chambIndex].front().getTime() < TriggerBuffer[recentIndex].front().getTime()) {
						recentIndex = chambIndex;
					}
				}
			}

			qTrigger = TriggerBuffer[recentIndex].front();
			chamberTimes[0] = qTrigger.getTime();
			chamberErgs[0] = qTrigger.getEnergy();
			chamberPSP[0] = qTrigger.getPsp();

			// energy discrimination of fission
			if((chamberErgs[0] < TRIGGER_THRESHOLD) || (chamberErgs[0] > TRIGGER_CLIP) || (chamberPSP[0] < TRIGGER_MIN_PSP) || (chamberPSP[0] > TRIGGER_MAX_PSP) )
			{
				validFiss = false;
			}

			// if fission is valid, store it in queue
			if(validFiss)
			{
				// update the number of valid fissions and populate queue
				newFission = CoincidenceEvent(chamberTimes[0], chamberErgs[0], recentIndex, chamberPSP[0]);
				FissionBuffer.push(newFission);
			}

			TriggerBuffer[recentIndex].pop();

			//count the #events left before killed
			// int h=0;
			// for(int i=0; i<NUM_TRIGGERS; i++) {
			// 	h += TriggerBuffer[i].size();
			// }
			// cout << h << "\n";

			allEmpty = true;
			for(int i=0; i<NUM_TRIGGERS; i++) {
				if(!TriggerBuffer[i].empty()) {
					allEmpty = false;
				}
			}
		}//end of while loop
	}//end of if statement

	else if(TRIGGER_SPLIT) {
		// first start by looking for valid fission triggers
		while (!TriggerBuffer[0].empty())
		{
			// boolean to keep track of valid fissions
			bool validFiss = true;

			// reset the times and energies
			for(int chambIndex = 0; chambIndex < NUM_TRIGGERS; chambIndex++)
			{
				chamberTimes[chambIndex] = 0;
				chamberErgs[chambIndex] = 0;
			}

			// assign fission event from first list
			qTrigger = TriggerBuffer[0].front();
			chamberTimes[0] = qTrigger.getTime();
			chamberErgs[0] = qTrigger.getEnergy();
			chamberPSP[0] = qTrigger.getPsp();

			chamberChan = 0;

			// look at the other fission lists
			for(int chambIndex = 1; chambIndex < NUM_TRIGGERS; chambIndex++)
			{
				if(!TriggerBuffer[chambIndex].empty())
				{
					chamberTimes[chambIndex] = TriggerBuffer[chambIndex].front().getTime();
					chamberErgs[chambIndex] = TriggerBuffer[chambIndex].front().getEnergy();
					chamberPSP[chambIndex] = TriggerBuffer[chambIndex].front().getPsp();
				}
				// match the indices for other queus
				while((chamberTimes[0] - chamberTimes[chambIndex] > COINC_WINDOW) and (!TriggerBuffer[chambIndex].empty()) )
				{
					TriggerBuffer[chambIndex].pop();
					chamberTimes[chambIndex] = TriggerBuffer[chambIndex].front().getTime();
					chamberErgs[chambIndex] = TriggerBuffer[chambIndex].front().getEnergy();
					chamberPSP[chambIndex] = TriggerBuffer[chambIndex].front().getPsp();
				}
				// find coincidences, set validity to 0 if one of the events is lost
				if(abs(chamberTimes[0] - chamberTimes[chambIndex]) > MAX_TRIGGER_DRIFT)
				{
					validFiss = false;
				}
			}
			// calculate the average of times and sum of energies
			averageTrigTime = 0;
			averageTrigPSP = 0;
			sumTrigErg = 0;

			for(int chambIndex = 0; chambIndex < NUM_TRIGGERS; chambIndex++)
			{
				averageTrigTime += chamberTimes[chambIndex];
				sumTrigErg += chamberErgs[chambIndex];
				averageTrigPSP += chamberPSP[chambIndex];
			}
			averageTrigTime /= NUM_TRIGGERS;
			averageTrigPSP /= NUM_TRIGGERS;

			// energy discrimination of fission
			if((sumTrigErg < TRIGGER_THRESHOLD) or (sumTrigErg > TRIGGER_CLIP) or (averageTrigPSP < TRIGGER_MIN_PSP) or (averageTrigPSP > TRIGGER_MAX_PSP) )
			{
				validFiss = false;
			}

			// if fission is valid, store it in queue
			if(validFiss)
			{
				// update the number of valid fissions and populate queue
				newFission = CoincidenceEvent(averageTrigTime, sumTrigErg, chamberChan, averageTrigPSP);
				FissionBuffer.push(newFission);
			}

			// get rid of currently analyzed event in queue
			TriggerBuffer[0].pop();
		}
	}

	/*
	___       _             _     ___       __  __
 / _ \ _  _| |_ _ __ _  _| |_  | _ )_  _ / _|/ _|___ _ _
| (_) | || |  _| '_ \ || |  _| | _ \ || |  _|  _/ -_) '_|
 \___/ \_,_|\__| .__/\_,_|\__| |___/\_,_|_| |_| \___|_|
							 |_|
	*/

	cout << TriggerBuffer[0].size() << " events remaining in buffer" << endl;

	long double stopTime = averageTrigTime;

	cout << "Number of Fissions is " << FissionBuffer.size() << endl;

	long double elapTime = (stopTime - beginTime)*1e-9;

	cout << "Time elapsed is: " << elapTime << endl;

	cout << "Fission rate at: " << FissionBuffer.size()/elapTime << " fissions/s" <<  endl;

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
	cout << endl;
	cout << "Now looping throught detectors. " << endl;

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
	double deltaT = 0;
	ParticleEvent qParticle = ParticleEvent();

	// keep track of the fission index
	int fisTracker = 0;

	// first start by looking for valid fission triggers
	while (!FissionBuffer.empty())
	{
		// increment the fission tracker
		fisTracker++;

		// reset the attributes of the fission event
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
		// ISABEL
		// assign the detector channel
		fissionChan = qFission.getTriggerChan();
		fissionTime = qFission.getTriggerTime();
		fissionEnergy = qFission.getEnergy();
		fissionPSP = qFission.getTriggerPSP();

		//cout << "Fission at time " << fissionTime << endl;

		// look at the detection events
		for(int detIndex = 0; detIndex < NUM_DETS; detIndex++)
		{
			// match the indices for other queues
			if(!DetectorBuffer[detIndex].empty())
			{
				qParticle = DetectorBuffer[detIndex].front();
				detTime = qParticle.getTime();
				deltaT = detTime - fissionTime;
			}

			// detector has no more valid detection events, skip it.
			else
			{
				continue;
			}

			// cycle through the indices of the other array
			while( (fissionTime - detTime > COINC_WINDOW) and !DetectorBuffer[detIndex].empty())
			{
				DetectorBuffer[detIndex].pop();
				qParticle = DetectorBuffer[detIndex].front();
				detTime = qParticle.getTime();
				deltaT = detTime - fissionTime;
			}


			//cout << "detC " << qParticle.getDetector() << ": " <<  deltaT << endl;

			// create the coincidence event
			if(abs(deltaT) < COINC_WINDOW)
			{
				totToF[totMult] = deltaT;
				totPSP[totMult] = qParticle.getPsp();
				totDep[totMult] = qParticle.getEnergy();
				totTail[totMult] = qParticle.getTail();
				totChan[totMult] = qParticle.getDetector();
				if(totChan[totMult] == 0)
				{
					cout << "0 channel inserted" << endl;
				}
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
	}


	// output the content of the buffers

	cout << endl;
	cout << "Buffers are finished: " << endl;


	cout << endl;
	cout << "Triggers: " << endl;

	for(int d = 0; d < NUM_TRIGGERS; d++)
	{
		cout << "trigger: " << FISSION_TRIGGERS[d] << ": " << TriggerBuffer[d].size() << endl;
	}

	cout << endl;
	cout << "Detectors: " << endl;

	for(int d = 0; d < NUM_DETS; d++)
	{
		cout << "channel: " << DETECTORS[d] << ": " << DetectorBuffer[d].size() << endl;
	}

	cout << endl;




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
	//first->cd();

	coincTree->Write();
	hDt->Write();
	//expFile->Close();

	return 1;
}
