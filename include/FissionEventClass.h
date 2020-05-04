/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Class with all of the properties of the detection system

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


class DetectorClass
{
public:
	int neutMult;
	int gammaMult;

	int backNeutMult;
	int backGammaMult;

	
};

#endif