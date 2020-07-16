/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Store the constants used in the processing
*/

#ifndef ProcessingConstants_h_
#define ProcessingConstants_h_

// calibration of detectors for energy
const double CHAN_VUS = (1.0/1000.0)*(1.0/1024.0); // from digitizer channel to Vus
const double CSCOMPTEDGE = 0.477;
// ISABEL // see constants
const double MASS_NEUTRONS = 939.6 // MeV/c^2
const double LIGHT_C = 30 // cm/ns

// these are too important to be left out like this
const double DETECTOR_THRESHOLD = 0.1;

// fission trigger allowed
const double CHAMBER_THRESHOLD = 0.0004;  //change - intersection
const double CHAMBER_CLIP = 0.005; //limit
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

//psd discrimination guesses - hard code discrimination

//callibrations for each detector individually

//time discriminations (fissionanalysis.c)
const int MINTIMEN = -5;
const int MAXTIMEN = 150;
const int MINTIMEP = -5;
const int MAXTIMEP = 10;
const int BACKSHIFT = -80;


#endif
