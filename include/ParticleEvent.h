// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: class intented to store the information related to a single detected particle
// Date: 02.20.2020

#ifndef ParticleEvent_h
#define ParticleEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "InfoSystem.h"


class ParticleEvent {

private:

	double depositedEnergy;
	double time;
	int detChannel;
	double psp;
	double tailEnergy;
	int type;

public:

	// the default and value constructor

	ParticleEvent()
	{
		depositedEnergy = 0;
		time = 0;
		tailEnergy = 0;
		psp = 0;
		detChannel = 0;
		type = 0;
	}

	ParticleEvent(int detNumber, ULong64_t timestamp, UShort_t energy, UShort_t energyShort)
	{
		depositedEnergy = (double)INT_CALIBRATION * energy;
		tailEnergy = (double)INT_CALIBRATION * energyShort;

		time = (double)timestamp/1000; // convert picosecond to nanosecond
		
		detChannel = detNumber;
		psp = tailEnergy/depositedEnergy;
		type = NO_PARTICLE;
	}

	// the next few functions are used to get the properties of the particle
	double getEnergy()
	{
		return depositedEnergy;
	}

	double getTail()
	{
		return tailEnergy;
	}

	double getTime()
	{
		return time;
	}

	int getDetector()
	{
		return detChannel;
	}

	double getPsp()
	{
		return psp;
	}

	int getType()
	{
		return type;
	}

	void correctTime(double triggerTime)
	{
		time -= triggerTime;
	}

};

#endif // #ifdef FissionEvent.cxx