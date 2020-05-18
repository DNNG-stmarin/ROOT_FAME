/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Store the constants used in the processing
*/

#ifndef ProcessingConstants_h_
#define ProcessingConstants_h_

// these are too important to be left out like this
const double DETECTOR_THRESHOLD = 0.15;

// calibration of detectors for energy
const double INT_CALIBRATION = 0.477/1000; // from Cs calibration
const double CHAMB_CALIBRATION = 0.477/1000; // from Cs calibration

// fission trigger allowed
const double CHAMBER_THRESHOLD = 0.03;
const double CHAMBER_CLIP = 10;
const double MAX_CHAMBER_DRIFT = 1; // ns
const double COINC_WINDOW = 200; // ns

// particle types definitions
const short NO_PARTICLE = 0;
const short NEUTRON = 1;
const short PHOTON = 2;

// multiplicity max to store
const int MAX_MULTIPLICITY = 50;

#endif
