/*
Author: Stefano Marin
Purpose: Interface for the detection analysis function
*/ 

#ifndef DetectionAnalysis_h
#define DetectionAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int DetectionAnalysis(int firstFile, int lastFile);
//int SystemAnalysis();

#endif