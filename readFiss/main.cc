// Programmer: Stefano Marin
// Purpose: Reading the fission events generated frmo ROOT_FAME
// Date: April 2021

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

  f->LoopExp(0.03, 45.0);
  if(simMode)
  {
    f->LoopSim(0.03, 45.0);
  }

  f->PlotLightOut();
  f->PlotTof();
  f->PlotErg();
  f->PlotMult();
  f->PlotPSD();

  if(simMode)
  {
    f->CompareLightOut();
    f->CompareTof();
    f->CompareErg();
    f->CompareMult();
    f->ComparePSD();
  }


  // delete f;

  return 0;
}
