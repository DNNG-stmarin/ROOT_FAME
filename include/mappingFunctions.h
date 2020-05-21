// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef mappingFunctions_h
#define mappingFunctions_h

// returns the index of the detector or -1 if not a detector
int isDetector(int detectorNumber, InfoSystemTest info);

int isChamber(int detectorNumber, InfoSystemTest info);

#endif
