// interface for create tree 
// Stefano April 29th, 2021
#include <TROOT.h>
#include <TTree.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>

#include <string.h>

using namespace std;

#ifndef createTree_h
#define createTree_h

void createTree(TString nameSim, int firstFile, int numFiles);
// void createTree(TString simFile, int firstFile, int numFiles);

#endif