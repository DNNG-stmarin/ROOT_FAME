/*
CleanFissionClass
Author: Stefano Marin
Purpose: store the event in the cleanest forma
Date: Ann Arbor, May 12th, 2020
*/

#ifndef CleanTreeClass_h
#define CleanTreeClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

class CleanTreeClass
{
private:

public:
  // attributes to create
  TFile* cleanFile;
  TChain* chainIn;


};

#endif
