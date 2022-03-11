// Programmer: Stefano Marin, Isabel Hernandez
// Purpose: header file containting useful constants
// Date: 03.20.2020

#ifndef InfoSystem_h
#define InfoSystem_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TGraph.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

class InfoSystem
{
private:

public:
  int NUM_FILES;
  int* FILE_LIST;

  InfoSystem()
  {
    NUM_FILES = 1;
    FILE_LIST = NULL;
  }

  void ReadInput(TString inputFile)
  {
    string inputFileString = string(inputFile);
    ifstream file (inputFileString);

    if(!file.is_open())
    {
      cout << "Failed to open input file\n";
      exit(0);
    }

    string line;
    string tag, value;
    while(file >> tag)
    {
      if(tag == "<NUM_FILES>:")
      {
        file >> value;
        NUM_FILES = stoi(value);
      }
      else if(tag == "<FILE_LIST>:")
      {
        FILE_LIST = new int[NUM_FILES];
        for(int i=0; i<NUM_FILES; i++)
        {
          file >> value;
          FILE_LIST[i] = stoi(value);
        }
      }
    }
    file.close();
  }

  ~InfoSystem()
  {
    delete FILE_LIST;
  }
};

#endif
