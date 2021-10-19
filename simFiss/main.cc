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

#include "SFAME.h"

using namespace std;

int main(int argc, char** argv)
{

  cout << "\n Welcome to SIM_FAME \n" << endl;

  TString simFile;
  int firstFile;
  int numFiles;

  /*if(argc == 1)
  {
    cout << "USAGE STATEMENT:" << endl;
    cout << "Provide name of sim file, first file, and number of files to process" << endl;
    return 1;
  }
  else if(argc == 4)
  {
    simFile = TString(argv[1]);
    firstFile = stoi(argv[2]);
    numFiles = stoi(argv[3]);
  }
  else
  {
    cout << "wrong number of inputs provided! \n";
    return 2;
  }*/

  sfame* simFame = new sfame();

  cout << "finished constructing" << endl;

  simFame->readFromCol();
  simFame->LoopColl();

  // simFame->readFromCol();
  // simFame->readFromSimFile(simFile, firstFile, numFiles);
  //


  return 0;
}
