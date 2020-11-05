// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: class intented to store a complete coincidence event with all the particle information
// Date: 02.20.2020 Ann Arbor

#ifndef CoincidenceEvent_h
#define CoincidenceEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "ParticleEvent.h"
#include "InfoSystem.h"

#include "ProcessingConstants.h"

class CoincidenceEvent {

private:

public:
	// ISABEL
	// beamTime;
	// beamEnergy;
	// beamChannel;
	// beamMicroIndex;

	// attributes of the coincidence class
	int totalMultiplicity;

	// ISABEL
	// add the channel of the target
	int triggerChannel;
	double triggerEnergy;
	double triggerTime;
	bool isValidFission;
	double triggerPSP;

	ParticleEvent particles[MAX_MULTIPLICITY];
	ParticleEvent neutrons[MAX_MULTIPLICITY];
	ParticleEvent photons[MAX_MULTIPLICITY];



	CoincidenceEvent()
	{
		totalMultiplicity = 0;
		triggerTime = 0;
		triggerEnergy = 0;
		triggerChannel = 0;
		triggerPSP = 0;
	}

	// CoincidenceEvent(double triggerTimeIn, double triggerEnergyIn)
	// {
	// 	totalMultiplicity = 0;
	// 	triggerTime = triggerTimeIn;
	// 	triggerEnergy = triggerEnergyIn;
	// }

	CoincidenceEvent(double triggerTimein, double triggerEnergyin, int triggerChannelin, double triggerPSPin) {
		totalMultiplicity = 0;
		triggerTime = triggerTimein;
		triggerEnergy = triggerEnergyin;
		triggerChannel = triggerChannelin;
		triggerPSP = triggerPSPin;
	}

	// default destructor
	~CoincidenceEvent(){
	}

	// get the trigger time
	double getTriggerTime()
	{
		return triggerTime;
	}

	// get the trigger time
	double getEnergy()
	{
		return triggerEnergy;
	}

	int getTriggerChan()
	{
		return triggerChannel;
	}

	double getTriggerPSP()
	{
		return triggerPSP;
	}

};

#endif // #ifdef FissionEvent.cxx
