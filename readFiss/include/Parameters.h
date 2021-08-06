#ifndef Parameters_h
#define Parameters_h

const int numTofBins = 240;
const double minTof = -20;
const double maxTof = 100;

const int numLObins = 250;
const double minLO = 0;
const double maxLO = 5;

const int numErgBins = 100;
const double minErg = 0;
const double maxErg = 10;

const int numPSDBins = 100;
const double minPSP = 0;
const double maxPSP = 1;

const int minMult = 0;
const int maxMult = 10;

const int numCosBins = 100;

const int numfisDepBins = 500; //JJ import these numbers from the beamInfo.root file
const double minDep = 0;
const double maxDep = 0.05;

const int numfisBeamTimeBins= 2000;
const double minBeamTime = -200;
const double maxBeamTime = 1800;

const int numDeltaT = 200;
const double minDeltaT = 0;
const double maxDeltaT = 10000;

#endif
