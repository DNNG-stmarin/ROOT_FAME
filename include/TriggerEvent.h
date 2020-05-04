// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: class intented to store the information related to a single detected particle
// Date: 02.20.2020

#ifndef TriggerEvent_h
#define TriggerEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "InfoSystem.h"
#include "ParticleEvent.h"


class TriggerEvent
{
private:
	double depositedEnergy;
	double time;
	int detChannel;
	double psp;
	int type;
	bool isFission;

public:

	TriggerEvent()
	{
		depositedEnergy = 0;
		time = 0;
		psp = 0;
		detChannel = 0;
		type = 0;
		isFission = true;
	}

	TriggerEvent(int detNumber, ULong64_t timestamp, UShort_t energy, UShort_t energyShort)
	{
		depositedEnergy = (double)INT_CALIBRATION * energy;
		time = (double)timestamp/1000; // convert picosecond to nanosecond
		detChannel = detNumber;
		psp = (double)energyShort/depositedEnergy;
		type = NO_PARTICLE;
		isFission = true;
	}

		// the next few functions are used to get the properties of the particle
	double getEnergy()
	{
		return depositedEnergy;
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

#endif 