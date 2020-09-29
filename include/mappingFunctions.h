// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef mappingFunctions_h
#define mappingFunctions_h

// returns the index of the detector or -1 if not a detector
int isDetector(int detectorNumber, int NUM_DETS, int* DETECTORS);

int isChamber(int detectorNumber, int NUM_CHAMBERS, int* FISSION_CHAMBERS);

#endif
