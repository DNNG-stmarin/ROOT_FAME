// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystem_h
#define InfoSystem_h

// hardware 
const int 	CHAN_PER_BOARD = 16;

// experiment specific information
const int NUM_BOARDS = 3;
const int NUM_DETS = 6;
const int NUM_CHAMBERS = 3;

// location of fission chambers and detectors
const int FISSION_CHAMBERS[NUM_CHAMBERS] = {0, 16, 32};
const int DETECTORS[NUM_DETS] = {2, 10, 18, 26, 34, 42};
const int REF_CHANNEL = 0; // consider removing

// channel delays (create delay script)
const double CHANNEL_DELAYS[NUM_DETS] = {35.3e3, 39.6e3, 32.6e3, 32.4e3, 35.1e3, 35.3e3}; // from photon ToF approximate

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
const double COINC_WINDOW = 200; // ns


const double CHAMBER_THRESHOLD = 0.03;
const double DETECTOR_THRESHOLD = 0.15;
const double PSP_DISCRIMINATION = 0.85; //very basic psd, we can do much more advanced than that


#endif