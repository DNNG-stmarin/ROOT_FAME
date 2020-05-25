// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystem_h
#define InfoSystem_h

// experiment specific information
const int NUM_BEAMS = 0;
const int NUM_CHAMBERS = 3;
const int NUM_DETS = 6;

const int BEAM[NUM_BEAMS] = {};
// location of fission chambers and detectors
const int FISSION_CHAMBERS[NUM_CHAMBERS] = {7};

const int DETECTORS[NUM_DETS] = { 9, 10, 11, 12, 13, 14, 15, 16, 25,
	            				 26, 27, 28, 29, 30, 31, 32, 41, 42,
	            				 43, 44, 45, 46, 47, 48, 49, 50, 51,
	            				 52, 53, 54, 55, 56, 57, 58, 59, 60,
	            				 61, 62, 63, 64, 65, 66, 67, 68, 69,
	            				 70, 71, 72, 73, 74, 75, 76, 77, 78};

// channel delays (create delay script)
//const double CHANNEL_DELAYS[NUM_DETS] = {35.3, 39.6, 32.6, 32.4, 35.1, 35.3}; // from photon ToF approximate

// calibration of detectors for energy
const double INT_CALIBRATION = 0.477/1000; // from Cs calibration
const double CHAMB_CALIBRATION = 0.477/1000; // from Cs calibration

// particle types definitions
const short NO_PARTICLE = 0;
const short NEUTRON = 1;
const short PHOTON = 2;

// multiplicity max to store
const int MAX_MULTIPLICITY = 50;

const double MAX_CHAMBER_DRIFT = 1; // ns
const double COINC_WINDOW = 600; // ns

const double CHAMBER_THRESHOLD = 0.01;
const double CHAMBER_CLIP = 10.0;

const int DETECTORS[NUM_DETS] = {2, 10, 18, 26, 34, 42};

#endif
