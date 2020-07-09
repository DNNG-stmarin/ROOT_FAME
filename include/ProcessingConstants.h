/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Store the constants used in the processing
*/

#ifndef ProcessingConstants_h_
#define ProcessingConstants_h_

// calibration of detectors for energy
const double INT_CALIBRATION = (1.0/1000.0)*(1.0/1024.0); // from Cs calibration
const double CHAMB_CALIBRATION = (1.0/1000.0)*(1.0/1024.0);  // from Cs calibration

// these are too important to be left out like this
const double DETECTOR_THRESHOLD = 0.1;

// fission trigger allowed
const double CHAMBER_THRESHOLD = 0.;
const double CHAMBER_CLIP = 16384;
const double MAX_CHAMBER_DRIFT = 1; // ns
const double COINC_WINDOW = 500; // ns

// particle types definitions
const short NO_PARTICLE = 0;
const short NEUTRON = 1;
const short PHOTON = 2;


// time regions
const double MIN_TIME_P = -5.0;
const double MAX_TIME_P = 10.0;

const double MIN_TIME_N = 20.0;
const double MAX_TIME_N = 150.0;

// bacgkround regions
const double DELTA_BACK_SIG = 10;
const double BACKGROUND_SHIFT = MAX_TIME_N - MIN_TIME_P + DELTA_BACK_SIG;


// multiplicity max to store
const int MAX_MULTIPLICITY = 50;

#endif
