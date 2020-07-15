// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: functions to map channel numbers to detectors and identify the type of signal
// Date: 03.20.2020

#define mappingFunctions_h

#include "InfoSystem.h"
#include "mappingFunctions.h"

// returns the index of the detector or -1 if not a detector
int isDetector(int detectorNumber, InfoSystem* info)
{
	int detIndex = -1;

	for(int index = 0; index < info->NUM_DETS; index++)
	{
		//cout << "In loop!!\n";
		if(detectorNumber == info->DETECTORS[index])
		{
			detIndex = index;
			break;
		}
	}
	return detIndex;
}


int isChamber(int detectorNumber, InfoSystem* info)
{
	int chamberIndex = -1;
	for(int index = 0; index < info->NUM_CHAMBERS; index++)
	{
		if(detectorNumber == info->FISSION_CHAMBERS[index])
		{
			chamberIndex = index;
			break;
		}
	}
	return chamberIndex;
}
