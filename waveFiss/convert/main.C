// Programmers: Nathan Giha (giha@umich.edu)
// Purpose: Convert CoMPASS output to a format compatible with Dana's scripts
// Date: March 2022

// #include "InfoSystem.h"
#include "ConvertCompass.h"

#include <stdlib.h>
#include <stdio.h>
#include <TString.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{

  cout << "\nWelcome to WAVE_FAME" << endl;

  if(argc == 1) {
    cout << "ERROR: config file not given\n";
    return 0;
  }

  TString configFileName;
  TString fileName;

  configFileName = TString(argv[1]);

  if(argc >= 3) {
    fileName = TString(argv[2]);
  }
  else
  {
    fileName = "compassCf_";
  }

  ConvertCompass* convert = new ConvertCompass(configFileName);
  convert->Loop();

  return 0;
}
