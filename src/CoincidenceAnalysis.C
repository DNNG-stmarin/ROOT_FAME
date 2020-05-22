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

//#include "InfoSystem.h"
#include "InfoSystemTest.h"
#include "PhysicalConstants.h"
#include "ProcessingConstants.h"

#include "CoincidenceEvent.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"

#include "mappingFunctions.h"

using namespace std;

int CoincidenceAnalysis::CreateCoincidenceTree(InfoSystemTest info, int fileNum, Long64_t entriesToProc)
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
	queue<TriggerEvent> TriggerBuffer[info.NUM_CHAMBERS]; //from InfoSystem
	queue<ParticleEvent> DetectorBuffer[info.NUM_DETS];


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


		if(digType == 0) //compass type digitizer
		{
			detChannel = cp->getDetector();
			timeDet = cp->getTime();
			energyDep = cp->getEnergy();
			energyTail = cp->getTail();

		}
		else if(digType == 1) //midas type digitizer
		{
			detChannel = md->getDetector();
			timeDet = md->getTime();
			energyDep = md->getEnergy();
			energyTail = md->getTail();
		}

		cout << "\ndetChannel=" << detChannel << endl;
		cout << isDetector(detChannel, info) << endl;
		cout << "Detector mapping done\n";
		cout << isChamber(detChannel, info) << endl;
		if(isDetector(detChannel, info) >= 0)
		{
			newParticle = ParticleEvent(detChannel, timeDet, energyDep, energyTail);
			DetectorBuffer[isDetector(detChannel, info)].push(newParticle);
		}
		else if(isChamber(detChannel, info) >= 0)
		{
			cout << "\nChamber if statement\n";

			newTrigger = TriggerEvent(detChannel, timeDet, energyDep, energyTail);
			TriggerBuffer[isChamber(detChannel, info)].push(newTrigger);
		}

	}


	// output the content of the buffers

	cout << endl;
	cout << "Buffers are ready: " << endl;


	cout << endl;
	cout << "Triggers: " << endl;

	for(int d = 0; d < info.NUM_CHAMBERS; d++)
	{
		cout << "trigger: " << info.FISSION_CHAMBERS[d] << ": " << TriggerBuffer[d].size() << endl;
	}

	cout << endl;
	cout << "Detectors: " << endl;

	for(int d = 0; d < info.NUM_DETS; d++)
	{
		cout << "channel: " << info.DETECTORS[d] << ": " << DetectorBuffer[d].size() << endl;
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
	//create TFile object
	// TString fileName = "FissionOutput" + to_string(fileNum) + ".root";
	// TFile *first = new TFile(fileName, "RECREATE");	//filename/option/filetitle

	expFile->cd();
	fileTreeDir->cd();

	coincTree = new TTree("CoincidenceTree", "Tree of Experimental Coincidences");


	// declatre the variables to store the fission branches
	int tMult = 0;
	double tTime = 0;
	double tDep = 0;
	double totToF[MAX_MULTIPLICITY] = {0};
	double totPSP[MAX_MULTIPLICITY] = {0};
	double totDep[MAX_MULTIPLICITY] = {0};
	double totTail[MAX_MULTIPLICITY] = {0};
	int totChan[MAX_MULTIPLICITY] = {0};

	// scalar variables
	coincTree->Branch("tMult", &tMult, "tMult/I");
	coincTree->Branch("tTime", &tTime, "fissionTime/D");
	coincTree->Branch("tDep", &tDep, "fissionErg/D");

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
	//Long64_t indexTrig[info.NUM_CHAMBERS] = {0};
	Long64_t *indexTrig = new Long64_t[info.NUM_CHAMBERS];


	// keep track of the earliest times in each of the channels
	double *DetectorLastTime = new double[info.NUM_DETS];

	// distribution of chamber times and energy
	double *chamberTimes = new double[info.NUM_CHAMBERS];
	double *chamberErgs = new double[info.NUM_CHAMBERS];

	// initialize the new particle
	TriggerEvent qTrigger = TriggerEvent();
	CoincidenceEvent newFission = CoincidenceEvent(0, 0);

	// start at the beginning of the array and also keep track of the the number of coincidence events found.
	ULong64_t numFissEvents = 0;

	// initialize fission tracker
	bool validFiss = true;
	double averageTrigTime = 0;
	double sumTrigErg = 0;

	long double beginTime = TriggerBuffer[0].front().getTime();

	// first start by looking for valid fission triggers
	while (!TriggerBuffer[0].empty())
	{
		// boolean to keep track of valid fissions
		bool validFiss = true;

		// reset the times and energies
		for(int chambIndex = 0; chambIndex < info.NUM_CHAMBERS; chambIndex++)
		{
			chamberTimes[chambIndex] = 0;
			chamberErgs[chambIndex] = 0;
		}

		// assign fission event from first list
		qTrigger = TriggerBuffer[0].front();
		chamberTimes[0] = qTrigger.getTime();
		chamberErgs[0] = qTrigger.getEnergy();

		// look at the other fission lists
		for(int chambIndex = 1; chambIndex < info.NUM_CHAMBERS; chambIndex++)
		{

			if(!TriggerBuffer[chambIndex].empty())
			{
				chamberTimes[chambIndex] = TriggerBuffer[chambIndex].front().getTime();
				chamberErgs[chambIndex] = TriggerBuffer[chambIndex].front().getEnergy();
			}
			// match the indices for other queus
			while((chamberTimes[0] - chamberTimes[chambIndex] > COINC_WINDOW) and (!TriggerBuffer[chambIndex].empty()) )
			{
				TriggerBuffer[chambIndex].pop();
				chamberTimes[chambIndex] = TriggerBuffer[chambIndex].front().getTime();
				chamberErgs[chambIndex] = TriggerBuffer[chambIndex].front().getEnergy();
			}
			// find coincidences, set validity to 0 if one of the events is lost
			if(abs(chamberTimes[0] - chamberTimes[chambIndex]) > MAX_CHAMBER_DRIFT)
			{
				validFiss = false;
			}
		}
		// calculate the average of times and sum of energies
		averageTrigTime = 0;
		sumTrigErg = 0;

		for(int chambIndex = 0; chambIndex < info.NUM_CHAMBERS; chambIndex++)
		{
			averageTrigTime += chamberTimes[chambIndex];
			sumTrigErg += chamberErgs[chambIndex];
		}
		averageTrigTime /= info.NUM_CHAMBERS;

		// energy discrimination of fission
		if((sumTrigErg < CHAMBER_THRESHOLD) or (sumTrigErg > CHAMBER_CLIP))
		{
			validFiss = false;
		}

		// if fission is valid, store it in queue
		if(validFiss)
		{
			// update the number of valid fissions and populate queue
			newFission = CoincidenceEvent(averageTrigTime, sumTrigErg);
			FissionBuffer.push(newFission);
		}

		// get rid of currently analyzed event in queue
		TriggerBuffer[0].pop();
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
	CoincidenceEvent qFission = CoincidenceEvent(0, 0);
	double fissionTime = 0;
	double fissionEnergy = 0;

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
		// cout << FissionBuffer.size() << endl;
		qFission = FissionBuffer.front();
		fissionTime = qFission.getTriggerTime();
		fissionEnergy = qFission.getEnergy();

		//cout << "Fission at time " << fissionTime << endl;

		// look at the detection events
		for(int detIndex = 0; detIndex < info.NUM_DETS; detIndex++)
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
				//cout << "detector " << detIndex << " is empty." << endl;
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
				totMult++;
			}
		}

		//cout << "_____________________________" << endl;

		// get rid of analyzed fission event
		FissionBuffer.pop();

		// now fill the histogram of particle-particle coincidences
		tMult = totMult;
		tTime = fissionTime;
		tDep = fissionEnergy;

		// fill the tree branches
		coincTree->Fill();
	}


	// output the content of the buffers

	cout << endl;
	cout << "Buffers are finished: " << endl;


	cout << endl;
	cout << "Triggers: " << endl;

	for(int d = 0; d < info.NUM_CHAMBERS; d++)
	{
		cout << "trigger: " << info.FISSION_CHAMBERS[d] << ": " << TriggerBuffer[d].size() << endl;
	}

	cout << endl;
	cout << "Detectors: " << endl;

	for(int d = 0; d < info.NUM_DETS; d++)
	{
		cout << "channel: " << info.DETECTORS[d] << ": " << DetectorBuffer[d].size() << endl;
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
