//
//
#ifndef Constants_h
#define Constants_h

// Fixed size dimensions of array or collections stored in the TTree if any.
const int MAX_LINES = 1000; // put a large number representing the maximum number of interactions a single history can generate (i.e. the number of lines all having the same history)
const int NUM_ROWS = 16;

// partilce types
const int NEUTRONS = 1;
const int PHOTONS = 2;
const int BADNEUTRON = 11;
const int BADPHOTON = 12;
const int CUTNEUTRON = 21;
const int CUTPHOTON = 22;

// zaid values
const int HYDROGEN = 1001;
const int CARBON = 6000;

// constants

const double C = 29.9792; // speed of light in cm/ns
const double MASS_NEUTRON = 939.565;   // mass of the neutron in MeV/c^2

const double N_PER_FISSION = 3.757;
const double P_PER_FISSION = 8.29;

#endif
