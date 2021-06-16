#ifndef Constants_h
#define Constants_h

// Fixed size dimensions of array or collections stored in the TTree if any.
const int MAX_MULT = 20;
const int MAX_TRIGGERS = 1000;

// Set physical constants
const double BEAM_PATH = 2150;//cm
const double N_MASS = 939.565; // MeV
const double LIGHT_C = 29.98; // cm/ns

// types of cut particles
const int ACCEPTED_SIGNAL = 0;
const int CUT_BEAM = 1;
const int CUT_DEP = 2;
const int CUT_PILEUP = 3;

#endif
