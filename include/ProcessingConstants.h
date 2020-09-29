/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Store the constants used in the processing
*/

#ifndef ProcessingConstants_h_
#define ProcessingConstants_h_

// calibration of detectors for energy
const double CHAN_VUS = (1.0/1000.0)*(1.0/1024.0); // from digitizer channel to Vus
const double CSCOMPTEDGE = 0.477;
const double MASS_NEUTRONS = 939.6; // MeV/c^2
const double LIGHT_C = 30; // cm/ns

// particle types definitions
const short NO_PARTICLE = 0;
const short NEUTRON = 1;
const short PHOTON = 2;

// multiplicity max to store
const int MAX_MULTIPLICITY = 50;
const int MAX_EVENTS = 50;

#endif
