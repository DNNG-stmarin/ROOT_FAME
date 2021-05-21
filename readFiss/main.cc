// Programmer: Stefano Marin, Jonathan Giha
// Purpose: Reading the fission events generated frmo ROOT_FAME
// Date: April 2021

// NOTE: Beam functionality is not at all implemented!
// I've left some placeholders, but not for everything.

// Changes/concerns will have a comment containing "JONATHAN" nearby.
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
  TString beamFile; // JONATHAN - beam functionality
  TString writeFile;

  readFiss* f;

  int mode; // JONATHAN - simMode bool to mode int
  // JONATHAN - mode menu and filenames
  cout << "What mode? \n" <<
          "Options: \n " <<
          "0: Experimental data only \n " <<
          "1: Experimental and simulation data \n " <<
          "2: Experimental and beam data" << endl;
  cin >> mode;
  cout << "Using mode " << mode << endl;

  while(!(mode == 0 || mode == 1 || mode == 2))
  {
    cout << "Invalid mode. Please input 0, 1, or 2." << endl;
    cin >> mode;
    cout << "Using mode " << mode << endl;
  }
  cout << "\n";

  cout << "Input writeFile path" << endl;
  cin >> writeFile;
  cout << "\n";

  cout << "Input expFile path" << endl;
  cin >> expFile;
  cout << "\n";

  if(mode == 1)
  {
    cout << "Input simFile path" << endl;
    cin >> simFile;
    f = new readFiss(writeFile, expFile, simFile);
  }

  else if(mode == 2)
  {
    cout << "Input beamFile path" << endl;
    cin >> beamFile;
    f = new readFiss(writeFile, expFile, beamFile, mode);
  }

  else if(mode == 0)
  {
    f = new readFiss(writeFile, expFile);
  }

  cout << "\n";

  /* JONATHAN - old command line functionality - delete?
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
  */


  // TBrowser* browser = new TBrowser();
  // JONATHAN - BNBP, threshold/max time, min/max energies, CovEM menu
  double threshold_in;
  double maxtime_in;
  cout << "Input threshold [MeVee] and max time [ns]. Sample input: \n0.20 70.0" << endl;
  cin >> threshold_in >> maxtime_in;
  f->SetRunThresholds(threshold_in, maxtime_in);
  cout << "\n";

  bool CovEM_in;
  cout << "Input 1 for CovEM, input 0 for no CovEM" << endl;
  cin >> CovEM_in;
  if(!CovEM_in) cout << "Not u";
  else cout << "U";
  cout << "sing CovEM" << endl;
  cout << "\n";

  if(CovEM_in){
    double min_n_in;
    double max_n_in;
    double min_p_in;
    double max_p_in;
    cout << "Input min/max energy limits for neutrons and photons. Sample input:" <<
            "\n0 10.0 0 4.0" << endl;
    cin >> min_n_in >> max_n_in >> min_p_in >> max_p_in;
    f->SetEnergyLimits(min_n_in,max_n_in,min_p_in,max_p_in);
    cout << "\n";

    int bn_in;
    int bp_in;
    cout << "Input BN and BP. Sample input: \n10 50" << endl;
    cin >> bn_in >> bp_in;
    f->SetBNBP(bn_in,bp_in);
    cout << "\n";
  }

  // loop through
  f->LoopExp();
  if(mode == 1) // JONATHAN - replace simMode here with mode
  {
    f->LoopSim();
  }
  if(mode == 2) // JONATHAN - beam placeholder
  {
    f->LoopBeam(); // JONATHAN - not implemented
  }

  // JONATHAN - CovEM
  if(CovEM_in){
    // covEM plot
    f->CovEM();
    f->WriteCovEM();
  }

  // plotting sections
  f->PlotAll();
  if(mode == 1) // JONATHAN - again, simMode -> mode
  {
    f->CompareAll();
  }

  if(mode == 2)
  {
    cout << "BEAM PLOT FUNCTIONALITY NOT IMPLEMENTED." << endl;
    // JONATHAN - placeholder
  }
  // delete f;

  return 0;
}
