#include "readFiss.h"


using namespace std;


// readFiss main constructor constructor
readFiss::readFiss(int &argc, char** &argv)
{
  // get input from user
  if(argc == 1)
  {
    GetInfo(cin);
  }

  // get input from file
  else if(argc == 2)
  {
    ifstream fin(argv[1]);
    // check if file opens properly
    if(!fin.is_open())
    {
      cout << "Could not open " << argv[1] << endl;
      BadInputMessage();
      return;
    }
    GetInfo(fin);
  }

  // bad input
  else
  {
    BadInputMessage();
    return;
  }


  InitializeHistograms();


  // loop through
  LoopExp();
  if(mode == 1)
  {
    LoopSim();
  }
  if(mode == 2) // JONATHAN - beam placeholder
  {
    LoopBeam(); // JONATHAN - not implemented
  }


  // run CovEM if user wanted to
  if(CovEM_in){
    // covEM plot
    CovEM();
    WriteCovEM();
  }


  // plotting sections
  PlotAll();
  if(mode == 1) // JONATHAN - again, simMode -> mode
  {
    CompareAll();
  }
  if(mode == 2)
  {
    cout << "BEAM PLOT FUNCTIONALITY NOT IMPLEMENTED." << endl;
    // JONATHAN - placeholder
  }
}

// tell user how to use input
void readFiss::BadInputMessage()
{
  cout << "Bad input. Please run with no arguments and input options manually" <<
          " or run with an input file with format: \n " <<

          "Mode number (0/1/2) \n " <<
          "writeFile path \n " <<
          "expFile path \n " <<
          "If in mode 1: simFile path \n " <<
          "If in mode 2: beamFile path \n " <<
          "Threshold [MeVee] Max Time [ns] \n " <<
          "Whether or not you want to use CovEM (0/1) \n " <<
          "If using CovEM: BN BP \n " <<
          "If using CovEM: MIN_N_ERG MAX_N_ERG MIN_P_ERG MAX_P_ERG \n\n" <<

          "Sample input file using mode 0 and not using CovEM: \n " <<
          "0 \n " <<
          "out.root \n " <<
          "../data/Fiss.root \n " <<
          "0.2 70 \n " <<
          "0 \n\n" <<

          "Sample input file using mode 2 and CovEM: \n " <<
          "2 \n " <<
          "out.root \n " <<
          "../data/Fiss.root \n " <<
          "../data/Beam.root \n " <<
          "0.2 70 \n " <<
          "1 \n " <<
          "10 50 \n " <<
          "0 10 0 4 \n\n";
}

// gets input from istream, which should either point to a file or cin
void readFiss::GetInfo(istream &inputStream)
{
  TString writeName;

  // get mode from user
  cout << "What mode? \n" <<
          "Options: \n " <<
          "0: Experimental data only \n " <<
          "1: Experimental and simulation data \n " <<
          "2: Experimental and beam data" << endl;
  inputStream >> mode;
  cout << "Using mode " << mode << endl;

  while(!(mode == 0 || mode == 1 || mode == 2))
  {
    cout << "Invalid mode. Please input 0, 1, or 2." << endl;
    inputStream >> mode;
    cout << "Using mode " << mode << endl;
  }
  cout << "\n";



  // get writeFile from user
  cout << "Input writeFile path" << endl;
  inputStream >> writeName;
  cout << "Using writeFile " << writeName << endl;
  cout << "\n";

  // get writeFile ready
  writeFile = new TFile(writeName, "RECREATE");
  cd_basics = writeFile->mkdir("Basic");
  cd_individual = writeFile->mkdir("Individual");
  cd_FAME = writeFile->mkdir("FAME");
  cd_correlated = writeFile->mkdir("Correlated");

  // get expFile from user
  cout << "Input expFile path" << endl;
  inputStream >> nameExp;

  // initialize experiment tree
  TTree* tree;

  cout << "Initializing experiment tree from " << nameExp << endl;
  expFile = (TFile*)gROOT->GetListOfFiles()->FindObject(nameExp);
  if (!expFile || !expFile->IsOpen())
  {
      expFile = new TFile(nameExp);
  }
  expFile->GetObject("Fiss", tree);
  InitExp(tree);

  // if in simulation mode, get simFile from user and initialize simulation tree
  if(mode == 1)
  {
    cd_simComparison = writeFile->mkdir("SimComparison");
    cout << "Input simFile path" << endl;
    inputStream >> nameSim;
    cout << "Initializing simulation tree from " << nameSim << endl; // JONATHAN - used to be from nameExp
    simFile = (TFile*)gROOT->GetListOfFiles()->FindObject(nameSim);
    if (!simFile || !simFile->IsOpen())
    {
        simFile = new TFile(nameSim);
    }
    simFile->GetObject("fissionTree", tree);
    InitSim(tree);
  }

  // if in beam mode, get beamFile from user and ???
  else if(mode == 2)
  {
    cd_beam = writeFile->mkdir("Beam");
    cout << "Input beamFile path" << endl;
    inputStream >> nameBeam;
    // TODO - BEAM FUNCTIONALITY
  }

  cout << "\n";



  // get threshold and max time from user
  cout << "Input threshold [MeVee] and max time [ns]. Sample input: \n0.20 70.0" << endl;
  inputStream >> THRESHOLD >> MAX_TIME_N;
  cout << "Using: threshold = " << THRESHOLD << " MeVee, " << " Tmax = " << MAX_TIME_N << " ns." << endl;
  cout << "\n";



  // ask user if they want to use CovEM
  cout << "Input 1 for CovEM, input 0 for no CovEM" << endl;
  inputStream >> CovEM_in;
  if(CovEM_in) cout << "Using CovEM" << endl;
  else cout << "Not using CovEM" << endl;
  cout << "\n\n";

  // get CovEM options from user if they want to use CovEM
  if(CovEM_in)
  {
    cout << "Input min/max energy limits for neutrons and photons. Sample input:" <<
            "\n0 10.0 0 4.0" << endl;
    inputStream >> MIN_N_ERG >> MAX_N_ERG >> MIN_P_ERG >> MAX_P_ERG;
    cout << "Using neutron energy range " << MIN_N_ERG << " to " << MAX_N_ERG <<
            " and photon energy range " << MIN_P_ERG << " to " << MAX_P_ERG << endl;
    cout << "\n";

    cout << "Input BN and BP. Sample input: \n10 50" << endl;
    inputStream >> BN >> BP;
    cout << "Using BN = " << BN << " and BP = " << BP << ". \n";
    cout << "\n";
  }
}
