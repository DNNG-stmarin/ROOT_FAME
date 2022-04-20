#ifndef constants_h
#define constants_h

const bool WAVE_MODE = false;

// energy acceptance region
const double MIN_ANODE1 = 100;
const double MIN_ANODE2 = 100;

// angle acceptance region
const double MIN_ANG1 = 0.5;
const double MIN_ANG2 = 0.5;
const double MAX_ANG1 = 1.9;
const double MAX_ANG2 = 1.9;

// angle finding parameters
const int N_BINS_RATIO = 100;
const double MIN_RATIO = 0.05;
const double MAX_RATIO = 0.25;
const int MIN_ENTRIES_PROJ = 400;


const int N_BINS_APH = 160;
const int MAX_APH = 800;

// breakpoint guesses for angle reconstruction
const double MIN_BR_1 = 150;
const double MAX_BR_1 = 700;
const double MIN_BR_2 = 150;
const double MAX_BR_2 = 700;
const double MID_BR_1 = 400;
const double MID_BR_2 = 400;
const double BREAK_POINT_RATIO = 0.02;

// fitting parameter for attenuation analysis
const double MIN_INV_ANG_FIT = 1.1;
const double MAX_INV_ANG_FIT = 3.0;


const double MAX_KE = 150;

// for fitting anode pulses
const double SIG_ANODE = 50;

// phd for calibration
const double PHD_L = 4.122; // +/- 0.05 MeV
const double PHD_H = 4.313; // +/- 0.03 MeV


// converging parameter
const int A_TOT = 252;
const int MAX_CONV_ITER = 252;
const double CONVERGEANCE_CONST = 0.002;


// wave stuff
const int RECORD_LENGTH = 1280;
const int NUM_WAVES = 100;

const int TIME_SAMPLE_CONVERSION = 16.0;

const int DIGITIZER_BITS = pow(2, 12);




#endif
