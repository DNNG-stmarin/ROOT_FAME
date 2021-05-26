#include "readFiss.h"

int readFiss::isTrigger(int detectorNumber)
{
	int chamberIndex = -1;
	for(int index = 0; index < NUM_TRIGGERS; index++)
	{
		if(detectorNumber == TRIGGERS[index])
		{
			chamberIndex = index;
			break;
		}
	}
	return chamberIndex;
}
