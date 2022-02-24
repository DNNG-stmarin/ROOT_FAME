// Programmer: Nathan and Stefano
// Purpose: Compass post-process and forming fission coincidences in fission
// Date: February 2022

#include "InfoSystem.h"
#include "fragFiss.h"

#include <stdlib.h>
#include <stdio.h>
#include <TString.h>

using namespace std;

int main(int argc, char** argv)
{

  cout << "\nWelcome to FRAG_FAME" << endl;

  TString fileName;
  TString inputFileName;

  if(argc == 1) {
    cout << "ERROR: input file not given\n";
    return 0;
  }

  inputFileName = TString(argv[1]);

  if(argc >= 3) {
    fileName = TString(argv[2]);
  }
  else
  {
    fileName = "compassCf_";
  }

  InfoSystem* info = new InfoSystem();
  info->ReadInput(inputFileName);

  fragFiss* frag = new fragFiss(info, fileName);
  frag->ReadEventTree();




  // FissionExperimentClass experiment = FissionExperimentClass(inputFileName);
  //
  // cout << "\nForming Coincidences" << endl;
  // experiment.CreateCoincidenceTree(fileName, experiment.expFile, -1);
  // cout << "Done CreateCoincidenceTree";
  //
  // cout << "\n";
  //
  // cout << "Performing detector analysis" << endl;
  // experiment.CreateDetectionAnalysis();
  // cout << "Finished DetectionAnalysis\n" << endl;
  //
  // cout << "Saving the results" << endl;
  // experiment.saveAll();



  return 0;
}
