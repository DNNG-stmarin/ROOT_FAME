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

  if(argc == 1) {
    cout << "Name of input tree not provided! \n ";
    cout << "provide the following inputs: " << endl;
    cout << "(1) name of expFile" << endl;
    cout << "(2, optional) name of simFile" << endl;
    
    return 1;
  }
  if(argc == 2)
  {
    expFile = TString(argv[1]);
  }
  if(argc == 3)
  {
    expFile = TString(argv[1]);
    simFile = TString(argv[2]);
  }
  else if(argc > 3)
  {
    cout << "Too many inputs provided" << endl;
    return 2;
  }

  // TBrowser* browser = new TBrowser();

  readFiss* f = new readFiss(expFile);
  f->LoopExp();
  f->PlotLightOut();
  f->PlotTof();
  f->PlotErg();

  // delete f;

  return 0;
}
