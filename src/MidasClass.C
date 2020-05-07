#define MidasClass_cxx

#include "MidasClass.h"
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
#include "FissionEvent.h"
#include "ParticleEvent.h"
#include "TriggerEvent.h"

#include "mappingFunctions.h"

using namespace std;

int MidasClass::CreateFissionTree(int fileNum, Long64_t entriesToProc)
{

	 //  _                          ____  _     _           _       
	 // | |                        / __ \| |   (_)         | |      
	 // | |     ___   ___  _ __   | |  | | |__  _  ___  ___| |_ ___ 
	 // | |    / _ \ / _ \| '_ \  | |  | | '_ \| |/ _ \/ __| __/ __|
	 // | |___| (_) | (_) | |_) | | |__| | |_) | |  __/ (__| |_\__ \
	 // |______\___/ \___/| .__/   \____/|_.__/| |\___|\___|\__|___/
	 //                   | |                 _/ |                  
	 //                   |_|                |__/                   

	// use an array of fifo to store particles and chambers
	queue<ParticleEvent> DetectorBuffer[NUM_DETS]; 
	queue<TriggerEvent> TriggerBuffer[NUM_CHAMBERS]; 


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
	//cout << "For this file there are: " << nentries << " entries." << endl;

	if(entriesToProc > 0)
	{
		//cout << "Reading the first " << entriesToProc << " events." << endl;
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
			//cout << "now reading entry " << jentry << endl;
		}

		// different reading programs
		detChannel = (int)detector;
		timeDet = 2*( (double)time + (double)interpolation );
		energyDep = (double)total;
		energyTail = (double)tail;



		if(isDetector(detChannel) >= 0)
		{
			newParticle = ParticleEvent(detChannel, timeDet, energyDep, energyTail);
			DetectorBuffer[isDetector(detChannel)].push(newParticle);
		}
		else if(isChamber(detChannel) >= 0)
		{
			newTrigger = TriggerEvent(detChannel, timeDet, energyDep, energyTail);
			TriggerBuffer[isChamber(detChannel)].push(newTrigger);
		}

	}

	return 1;
}
