// Programmer: Stefano Marin
// Purpose: Compass post-process and forming fission coincidences in fission
// Date: April 2020

#include <stdlib.h>
#include <stdio.h>
#include <TString.h>

#include "FissionExperimentClass.h"

using namespace std;

int main(int argc, char** argv)
{

  cout << "\nWelcome to ROOT FAME" << endl;

  TString fileName;
  TString inputFileName;

  if(argc == 1) {
    cout << "ERROR: input file not given\n";
    return 0;
  }

  inputFileName = TString(argv[1]);

  if(argc >= 3) {
    fileName = argv[2];
  }
  else
  {
    fileName = "compassCf_";
  }


  FissionExperimentClass experiment = FissionExperimentClass(inputFileName);

  cout << "\nForming Coincidences" << endl;
  experiment.CreateCoincidenceTree(fileName, experiment.expFile, 1000000);
  cout << "Done CreateCoincidenceTree";

  cout << "\n";

  cout << "Performing detector analysis" << endl;
  experiment.CreateDetectionAnalysis(experiment.detFile);
  cout << "Finished DetectionAnalysis\n" << endl;

  cout << "Saving the results" << endl;
  experiment.saveAll();

  return 0;
}
