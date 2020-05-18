// Programmer: Stefano Marin
// Purpose: Compass post-process and forming fission coincidences in fission
// Date: April 2020

#include "MidasClass.h"

#include <stdlib.h>
#include <stdio.h>
#include <TString.h>

#include "FissionExperimentClass.h"

using namespace std;

int main(int argc, char** argv)
{

  cout << "Welcome to ROOT FAME" << endl;

  // find the name of the file to use
  TString fileName;

  if(argc >= 2)
  {
    fileName =  argv[1];
  }
  else
  {
    fileName = "compassCf_";
  }

  // now that name is acquired proceed to read root file
  //cout << "Reading from file " << fileName << endl;

  FissionExperimentClass experiment = FissionExperimentClass();

  cout << "Forming Coincidences" << endl;
  experiment.CreateCoincidenceTree(fileName, experiment.expFile, 1000000);

  cout << "Analyzing single detectors" << endl;
  experiment.CreateDetectionAnalysis(experiment.coincTreeChain, experiment.detFile);

  cout << "Analyzing multiple detectors" << endl;
  experiment.CreateSystemAnalysis(experiment.coincTreeChain, experiment.sysFile);

  cout << "Creating the cleaned trees " << endl;

  cout << "Saving the results" << endl;
  experiment.saveAll();

  return 0;
}