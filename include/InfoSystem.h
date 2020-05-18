// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystem_h
#define InfoSystem_h

// experiment specific information
const int NUM_BEAM = 0;
const int NUM_CHAMBERS = 1;
const int NUM_DETS = 54;

// location of fission chambers and detectors
const int FISSION_CHAMBERS[NUM_CHAMBERS] = {7};

const int DETECTORS[NUM_DETS] = { 9, 10, 11, 12, 13, 14, 15, 16, 25,
	            				 					 26, 27, 28, 29, 30, 31, 32, 41, 42,
	            				           43, 44, 45, 46, 47, 48, 49, 50, 51,
	            				           52, 53, 54, 55, 56, 57, 58, 59, 60,
	            				           61, 62, 63, 64, 65, 66, 67, 68, 69,
	            				           70, 71, 72, 73, 74, 75, 76, 77, 78};
#endif
