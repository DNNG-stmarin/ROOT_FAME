// Programmer: Stefano Marin
// Purpose: Reading the fission events generated frmo ROOT_FAME
// Date: April 2021

// JONATHAN PRACTICE VER
// MAIN UNCHANGED
// Any changes will have a comment containing "jonathan" or "JONATHAN" nearby.

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TBrowser.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "readFiss.h"

using namespace std;

int main(int argc, char** argv)
{

  gErrorIgnoreLevel = kWarning;

  cout << "\n Welcome to READ_FAME \n" << endl;

  TString expFile;
  TString simFile;
  TString writeFile;

  readFiss* f;

  bool simMode;

  if(argc == 1) {
    cout << "Name of input tree not provided! \n ";
    cout << "provide the following inputs: " << endl;
    cout << "(1) name of writeFile" << endl;
    cout << "(2) name of expFile" << endl;
    cout << "(3, optional) name of simFile" << endl;

    return 1;
  }
  else if(argc == 3)
  {
    writeFile = TString(argv[1]);
    expFile = TString(argv[2]);
    f = new readFiss(writeFile, expFile);
    simMode = false;
  }
  else if(argc == 4)
  {
    writeFile = TString(argv[1]);
    expFile = TString(argv[2]);
    simFile = TString(argv[3]);
    f = new readFiss(writeFile, expFile, simFile);
    simMode = true;
  }
  else
  {
    cout << "not enough (or too many) arguments provided" << endl;
    return 2;
  }



  // TBrowser* browser = new TBrowser();
  f->SetBNBP(10,50);
  f->SetRunThresholds(0.20, 70.0);
  f->SetEnergyLimits(0,10.0,0,4.0);

  // loop through
  f->LoopExp();
  if(simMode)
  {
    f->LoopSim();
  }

  // // covEM plot
  // f->CovEM();
  // f->WriteCovEM();

  // plotting sections
  f->PlotAll();

  if(simMode)
  {
    f->CompareAll();
  }


  // delete f;

  return 0;
}
