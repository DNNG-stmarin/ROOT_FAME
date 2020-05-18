// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: class intented to store a complete coincidence event with all the particle information
// Date: 02.20.2020 Ann Arbor

#ifndef CoincidenceEvent_h
#define CoincidenceEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "InfoSystem.h"
#include "ParticleEvent.h"

class CoincidenceEvent {

private:

public:
	// attributes of the coincidence class
	int multiplicityNeutron;
	int multiplicityPhoton;
	int totalMultiplicity;

	double triggerEnergy;
	long double triggerTime;
	bool isValidFission;

	ParticleEvent particles[MAX_MULTIPLICITY];
	ParticleEvent neutrons[MAX_MULTIPLICITY];
	ParticleEvent photons[MAX_MULTIPLICITY];

	CoincidenceEvent()
	{
		multiplicityNeutron = 0;
		multiplicityPhoton = 0;
		totalMultiplicity = 0;
		triggerTime = 0;
		triggerEnergy = 0;
	}

	CoincidenceEvent(double triggerTimeIn, double triggerEnergyIn)
	{
		multiplicityNeutron = 0;
		multiplicityPhoton = 0;
		totalMultiplicity = 0;
		triggerTime = triggerTimeIn;
		triggerEnergy = triggerEnergyIn;
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

};

#endif // #ifdef FissionEvent.cxx