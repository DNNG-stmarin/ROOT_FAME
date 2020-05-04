/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Class which gets updated to idntify valid particles

Date: Ann Arbor, MI, May 3rd, 2020
*/

#ifndef DetectorClass_h
#define DetectorClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "FissionAnalysis.h"
#include "SystemAnalysis.h"

using namespace std;


class CutClass
{
public:
	TCut neutronCut;
	TCut gammaCut;
};

#endif