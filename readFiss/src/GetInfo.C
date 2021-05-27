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
  InitializeFunctions();

  // loop through
  LoopExp();
  if(mode == 1)
  {
    LoopSim();
  }
  if(mode == 2) // JONATHAN - beam placeholder
  {
    LoopBeam(); // JONATHAN - not implemented
    ReadBeamInfo();
    BeamDepAnalysis();
    // BeamErgAnalysis();
  }

  // run CovEM if user wanted to
  if(CovEM_in){
    // covEM plot
    CovEM();
    WriteCovEM();
  }

  // plotting sections
  PlotAll();
  if(mode == 1)
  {
    CompareAll();
  }
  if(mode == 2)
  {
    PlotDepSubtraction();
    PlotRatioMult();
    // JONATHAN - placeholder
  }
  writeFile->Close();
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
          "Detector threshold [MeVee] Chamber threshold [V us] tMax Time [ns] \n " <<
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
  cout << " Using mode " << mode << endl;

  while(!(mode == 0 || mode == 1 || mode == 2))
  {
    cout << "Invalid mode. Please input 0, 1, or 2." << endl;
    inputStream >> mode;
    cout << " Using mode " << mode << endl;
  }
  cout << "\n";



  // get writeFile from user
  cout << "Input writeFile path" << endl;
  inputStream >> writeName;
  cout << " Using writeFile " << writeName << endl;
  cout << "\n";

  // get writeFile ready
  writeFile = new TFile(writeName, "RECREATE");
  cd_basics = writeFile->mkdir("Basic");
  cd_individual = writeFile->mkdir("Individual");
  cd_FAME = writeFile->mkdir("FAME");
  cd_correlated = writeFile->mkdir("Correlated");



/*
 ___                _     ___ _ _
|_ _|_ _  _ __ _  _| |_  | __(_) |___ ___
| || ' \| '_ \ || |  _| | _|| | / -_|_-<
|___|_||_| .__/\_,_|\__| |_| |_|_\___/__/
        |_|
*/

  // get expFile from user
  cout << "Input expFile path (without .root extension)" << endl;
  inputStream >> nameExp;

  cout << "Input number of exp files" << endl;
  inputStream >> numExpFiles;

  // initialize experiment tree
  // TTree* tree;
  TChain *expChain = new TChain("Fiss");
  cout << "Initializing experiment tree from " << nameExp << endl;
  TString nameExpAdd = (TString)nameExp + ".root";
  expChain->Add(nameExpAdd);
  for(int expInd = 1; expInd < numExpFiles; expInd++)
  {
    nameExpAdd = (TString)nameExp + "_" + (TString)to_string(expInd) + ".root";
    expChain->Add(nameExpAdd);
  }
  InitExp(expChain);

  // if in simulation mode, get simFile from user and initialize simulation tree
  if(mode == 1)
  {
    cd_simComparison = writeFile->mkdir("SimComparison");
    cout << "Input simFile path (without .root extension)" << endl;
    inputStream >> nameSim;

    cout << "Input number of sim files" << endl;
    inputStream >> numSimFiles;

    TChain *simChain = new TChain("fissionTree");
    // cout << "Initializing simulation tree from " << nameSim << endl;
    // simFile = (TFile*)gROOT->GetListOfFiles()->FindObject(nameSim);
    // if (!simFile || !simFile->IsOpen())
    // {
    //     simFile = new TFile(nameSim);
    // }
    // simFile->GetObject("fissionTree", tree);
    TString nameSimAdd = (TString)nameSim + ".root";
    simChain->Add(nameSimAdd);
    for(int simInd = 1; simInd < numSimFiles; simInd++)
    {
      nameSimAdd = (TString)nameSim + "_" + (TString)to_string(simInd) + ".root";
      simChain->Add(nameSimAdd);
    }
    InitSim(simChain);
  }

  // if in beam mode, get beamFile from user and ???
  else if(mode == 2)
  {
    cd_beam = writeFile->mkdir("Beam");
    cout << "Input beamFile path" << endl;
    inputStream >> nameBeam;
    cout << "Reading beam information from " << nameBeam << endl;
    // TODO - BEAM FUNCTIONALITY
  }

  cout << "\n";


/*
  _   _               ___                _
 | | | |___ ___ _ _  |_ _|_ _  _ __ _  _| |_
 | |_| (_-</ -_) '_|  | || ' \| '_ \ || |  _|
  \___//__/\___|_|   |___|_||_| .__/\_,_|\__|
                              |_|
*/
  // get threshold and max time from user
  cout << "Input detector threshold [MeVee], fission chamber threshold [V us], and max time [ns]. Sample input: \n 0.20 0.005 70.0" << endl;
  inputStream >> THRESHOLD >> THRESHOLD_DEP >> MAX_TIME_N;
  cout << " Using: detector threshold = " << THRESHOLD << " MeVee, "
       << " fission chamber threshold = " << THRESHOLD_DEP << " V us, "
       << " Tmax = " << MAX_TIME_N << " ns." << endl;
  cout << "\n";

  // ask user for background delay
  cout << "background delay to visualize background (ns), put 0 if unsure" << endl;
  inputStream >> BACKGROUND_DELAY;
  cout << "\n";

  // ask user if they want to use CovEM
  cout << "Input 1 for CovEM, input 0 for no CovEM" << endl;
  inputStream >> CovEM_in;
  if(CovEM_in) cout << " Using CovEM" << endl;
  else cout << " Not using CovEM" << endl;
  cout << "\n\n";

  // get CovEM options from user if they want to use CovEM
  if(CovEM_in)
  {
    cout << "Input min/max energy limits for neutrons and photons. Sample input:" <<
            "\n 0 10.0 0 4.0" << endl;
    inputStream >> MIN_N_ERG >> MAX_N_ERG >> MIN_P_ERG >> MAX_P_ERG;
    cout << " Using neutron energy range " << MIN_N_ERG << " to " << MAX_N_ERG <<
            " and photon energy range " << MIN_P_ERG << " to " << MAX_P_ERG << endl;
    cout << "\n";

    cout << "Input BN and BP. Sample input: \n 10 50" << endl;
    inputStream >> BN >> BP;
    cout << " Using BN = " << BN << " and BP = " << BP << ". \n";
    cout << "\n";
  }

  // get number of triggers and trigger numbers from user
  if(mode == 2)
  {
    cout << "Input number of channels. Sample input: \n 10 \n";
    inputStream >> NUM_TRIGGERS;
    cout << " Using " << NUM_TRIGGERS << " triggers. \n\n";

    TRIGGERS = new int[NUM_TRIGGERS];

    cout << "Input trigger numbers. Sample input: \n" <<
    " \n4 \n5 \n6 \n21 \n22 \n23 \n24 \n31 \n32 \n38 \n";
    for(int i = 0; i < NUM_TRIGGERS; ++i)
    {
      inputStream >> TRIGGERS[i];
    }
    cout << " Using trigger numbers ";
    for(int i = 0; i < NUM_TRIGGERS; ++i)
    {
      cout << TRIGGERS[i] << " ";
    }
    cout << "\n\n";

    cout << "Input the minimum and maximum beam energies [MeV], and the # of energy bins";
    inputStream >> BEAM_ERG_MIN >> BEAM_ERG_MAX >> BEAM_ERG_BINNUM;
    cout << "\nUsing beam range between " << BEAM_ERG_MIN << " and "
         << BEAM_ERG_MAX << " MeV, "
         << BEAM_ERG_BINNUM << " bins" << endl;


  }
}
