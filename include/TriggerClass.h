/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Class with all of the properties of the triggering system

Date: Ann Arbor, MI, May 3rd, 2020
*/

#ifndef TriggerClass_h
#define TriggerClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


class TriggerClass
{
public:
  TString name = "trig";

  double crossSection;

  double energyResolution;
  double timeResolution;

};

#endif
