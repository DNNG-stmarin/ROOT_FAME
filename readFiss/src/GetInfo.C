#include "readFiss.h"

#include <sstream>

using namespace std;


// readFiss main constructor
readFiss::readFiss()
{
  runNum = 0;
  expFile = nullptr;
  simFile = nullptr;
  beamFile = nullptr;
  TRIGGERS = nullptr;
  writeFile = nullptr;

  //ANN
  if (ANN_mode) crossTalkANN = new myANN();
}

void readFiss::SetInfo(MainWindow* main_in)
{
  if(main_in == nullptr)
  {
    cout << "main_in is nullptr" << endl;
    return;
  }
  w = main_in;

  cout << "Getting user input..." << endl;

  // modes
  mode = w->mode();
  NUM_RUNS = w->NUM_RUNS();
  int compensator = NUM_RUNS;
  if(compensator > 1) compensator = 0;
  CovEM_in = w->CovEM_in();

  // paths
  nameExp = w->nameExp();
  if(mode == 1)
      nameSim = w->nameSim();
  else if(mode == 2)
      nameBeam = w->nameBeam();
  nameCoords = w->nameCoords();
  nameWrite = w->nameWrite();

  numExpFiles = w->numExpFiles();
  if(mode == 1)
      numSimFiles = w->numSimFiles();

  // detectors
  NUM_DETECTORS = w->NUM_DETECTORS();
  THRESHOLD = w->THRESHOLD() + (((w->THRESHOLD2() - w->THRESHOLD()) / (NUM_RUNS - 1 + compensator)) * runNum);
  CLIPPING = w->CLIPPING() + (((w->CLIPPING2() - w->CLIPPING()) / (NUM_RUNS - 1 + compensator)) * runNum);
  MAX_TIME_N = w->MAX_TIME_N() + (((w->MAX_TIME_N2() - w->MAX_TIME_N()) / (NUM_RUNS - 1 + compensator)) * runNum);

  // triggers
  NUM_TRIGGERS = w->NUM_TRIGGERS();
  if(TRIGGERS)
  {
    delete[] TRIGGERS;
  }
  TRIGGERS = w->TRIGGERS();
  THRESHOLD_DEP = w->THRESHOLD_DEP() + (((w->THRESHOLD_DEP2() - w->THRESHOLD_DEP()) / (NUM_RUNS - 1 + compensator)) * runNum);
  CLIPPING_DEP = w->CLIPPING_DEP() + (((w->CLIPPING_DEP2() - w->CLIPPING_DEP()) / (NUM_RUNS - 1 + compensator)) * runNum);

  // other settings for all modes
  BACKGROUND_DELAY = w->BACKGROUND_DELAY() + (((w->BACKGROUND_DELAY2() - w->BACKGROUND_DELAY()) / (NUM_RUNS - 1 + compensator)) * runNum);
  FISS_PILEUP_TIME = w->FISS_PILEUP_TIME() + (((w->FISS_PILEUP_TIME2() - w->FISS_PILEUP_TIME()) / (NUM_RUNS - 1 + compensator)) * runNum);

  // beam settings
  if(mode == 2)
  {
      BEAM_ERG_MIN = w->BEAM_ERG_MIN();
      BEAM_ERG_MAX = w->BEAM_ERG_MAX();
      BEAM_ERG_BINNUM = w->BEAM_ERG_BINNUM();
  }

  // CovEM settings
  if(CovEM_in)
  {
      MIN_N_ERG = w->MIN_N_ERG();
      MAX_N_ERG = w->MAX_N_ERG();
      MIN_P_ERG = w->MIN_P_ERG();
      MAX_P_ERG = w->MAX_P_ERG();
      BN = w->BN();
      BP = w->BP();
  }
}

void readFiss::Run()
{
  cout << "\n\nPreparing for analysis..." << endl;
  // get writeFile ready
  if(NUM_RUNS == 1)
  {
    writeFile = new TFile((TString)nameWrite + (TString)".root", "RECREATE");
    cout << "Writing output to " << ((TString)nameWrite + (TString)".root") << endl;
    if(!writeFile->IsOpen())
    {
      w->noWriteFile();
    }
  }
  else
  {
    writeFile = new TFile((TString)nameWrite + (TString)to_string(runNum) + (TString)".root", "RECREATE");
    cout << "Writing run " << runNum << " output to " << ((TString)nameWrite + (TString)to_string(runNum) + (TString)".root") << endl;
    if(!writeFile->IsOpen())
    {
      w->noWriteFile();
    }
  }
  cd_basics = writeFile->mkdir("Basic");
  cd_individual = writeFile->mkdir("Individual");
  cd_FAME = writeFile->mkdir("FAME");
  cd_correlated = writeFile->mkdir("Correlated");
  cd_trigger = writeFile->mkdir("Trigger");

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

  // if in simulation mode, initialize simulation tree
  if(mode == 1)
  {
    cd_simComparison = writeFile->mkdir("SimComparison");

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

  // if in beam mode, prep directories
  else if(mode == 2)
  {
    cd_beam = writeFile->mkdir("Beam");
    cd_alphaSub = cd_beam->mkdir("AlphaSubtraction");
    cd_multRatio = cd_beam->mkdir("MultiplicityRatio");
    cd_multErg = cd_beam->mkdir("MultipicityErg");
    cd_stack = cd_beam->mkdir("FissionSpectrumDueToBeamErg");
    cd_LO = cd_beam->mkdir("LightOutput");
    cd_gammaSpecBeam = cd_beam->mkdir("gammaSpecBeam");

    cout << "Reading beam information from " << nameBeam << endl;
  }

  // change visuals for plots
  legend_x1 = 0.48;
  legend_x2 = 0.8;
  legend_y1 = 0.7;
  legend_y2 = 0.9;

  x_labelSize = 0.04;
  y_labelSize = 0.04;

  x_tickSize = 0.05;
  y_tickSize = 0.03;

  // perform analysis
  cout << "Performing analysis..." << endl;

  GenerateAngles();
  InitializeHistograms();
  InitializeFunctions();

  // loop through
  LoopExp();
  if(mode == 1)
  {
    LoopSim();
  }
  if(mode == 2)
  {
    ReadBeamInfo();
    BeamDepAnalysis();
    BeamErgAnalysis();
    FitMult();
  }

  // correlated analysis
  CorrAnalysis();

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
    PlotMultErg();
    PlotStack();
    PlotLO();
    PlotBeamLO();
  }

  writeFile->Close();

  ++runNum;
}

void readFiss::Save(MainWindow* main_in)
{
  if(main_in == nullptr)
  {
    cout << "main_in is nullptr" << endl;
    return;
  }
  w = main_in;

  string nameSave = w->GetNameInput();
  cout << "Saving " << nameSave << endl;
  ofstream fout(nameSave);
  if(!fout.is_open())
  {
      cout << "Couldn't open " << nameSave << " for saving" << endl;
      return;
  }

  Print(fout);
  fout.close();
}

void readFiss::Load(MainWindow* main_in)
{
  if(main_in == nullptr)
  {
    cout << "main_in is nullptr" << endl;
    return;
  }
  w = main_in;

  string nameLoad = w->GetNameInput();
  cout << "Loading " << nameLoad << endl;
  ifstream fin(nameLoad);
  if(!fin.is_open())
  {
      cout << "Couldn't open " << nameLoad << " for loading" << endl;
      return;
  }
  LoadInput(fin);
  fin.close();
}

void readFiss::Print(ostream &out)
{
    // cout <<  w->GetNameInput() << endl;
    out << w->THRESHOLD(-1) << endl;
    out << w->CLIPPING(-1) << endl;
    out << w->MAX_TIME_N(-1) << endl;
    out << w->THRESHOLD_DEP(-1) << endl;
    out << w->CLIPPING_DEP(-1) << endl;
    out << w->BACKGROUND_DELAY(-1) << endl;
    out << w->FISS_PILEUP_TIME(-1)<< endl;

    out << w->mode() << endl;
    out << w->NUM_RUNS() << endl;
    out << w->CovEM_in() << endl;

    out << w->nameWrite() << endl;
    out << w->nameExp() << endl;
    out << w->numExpFiles() << endl;
    if(mode == SIM_MODE)
    {
        out << w->nameSim() << endl;
        out << w->numSimFiles() << endl;
    }
    else if(mode == BEAM_MODE)
    {
        out << w->nameBeam() << endl;
    }
    out << w->nameCoords() << endl;

    out << w->NUM_DETECTORS() << endl;
    // out << w->THRESHOLD() << endl;
    // out << w->CLIPPING() <<  endl;
    // out << w->MAX_TIME_N() << endl;

    out << w->NUM_TRIGGERS() << endl;
    int* saveTRIGGERS = w->TRIGGERS();
    for(int i = 0; i < w->NUM_TRIGGERS(); ++i)
    {
        out << saveTRIGGERS[i] << " ";
    }
    delete[] saveTRIGGERS;
    out << endl;
    out << w->THRESHOLD_DEP() << endl;
    out << w->CLIPPING_DEP() << endl;

    out << w->BACKGROUND_DELAY() << endl;
    out << w->FISS_PILEUP_TIME() << endl;

    if(w->CovEM_in())
    {
        out << w->MIN_N_ERG() << " " << w->MAX_N_ERG() << " ";
        out << w->MIN_P_ERG() << " " << w->MAX_P_ERG() << endl;
        out << w->BN() << " " << w->BP() << endl;
    }

    if(w->mode() == BEAM_MODE)
    {
        out << w->BEAM_ERG_MIN() << " " << w->BEAM_ERG_MAX() << " ";
        out << w->BEAM_ERG_BINNUM() << endl;
    }
}

void readFiss::LoadInput(istream &in)
{
    // load in settings
    string word;
    int integer;
    double dbl;
    cout << "Loading Input" << endl;

    // cout << w << endl;

    // range check boxes
    in >> dbl;
    w->THRESHOLD(dbl);
    in >> dbl;
    w->CLIPPING(dbl);
    in >> dbl;
    w->MAX_TIME_N(dbl);
    in >> dbl;
    w->THRESHOLD_DEP(dbl);
    in >> dbl;
    w->CLIPPING_DEP(dbl);
    in >> dbl;
    w->BACKGROUND_DELAY(dbl);
    in >> dbl;
    w->FISS_PILEUP_TIME(dbl);

    // modes
    in >> integer;
    w->mode(integer);

    in >> integer;
    w->NUM_RUNS(integer);

    in >> integer;
    w->CovEM_in(integer);

    // paths
    in >> word;
    w->nameWrite(word);
    in >> word;
    w->nameExp(word);
    in >> integer;
    w->numExpFiles(integer);

    if(w->mode() == SIM_MODE)
    {
        in >> word;
        w->nameSim(word);
        in >> integer;
        w->numSimFiles(integer);
    }

    else if(w->mode() == BEAM_MODE)
    {
        in >> word;
        w->nameBeam(word);
    }

    in >> word;
    w->nameCoords(word);

    // detectors
    in >> integer;
    w->NUM_DETECTORS(integer);
    // in >> dbl;
    // w->THRESHOLD(dbl);
    //in >> dbl;
    //w->THRESHOLD2(dbl);
    // in >> dbl;
    // w->CLIPPING(dbl);
    //in >> dbl;
    //w->CLIPPING2(dbl);
    // in >> dbl;
    // w->MAX_TIME_N(dbl);
    //in >> dbl;
    //w->MAX_TIME_N2(dbl);

    // triggers
    in >> integer;
    w->NUM_TRIGGERS(integer);
    int loadTRIGGERS[w->NUM_TRIGGERS()];
    for(int i = 0; i < w->NUM_TRIGGERS(); ++i)
    {
      in >> loadTRIGGERS[i];
    }
    w->TRIGGERS(loadTRIGGERS);
    in >> dbl;
    // w->THRESHOLD_DEP(dbl);
    // //in >> dbl;
    // //w->THRESHOLD_DEP2(dbl);
    // in >> dbl;
    // w->CLIPPING_DEP(dbl);
    // // in >> dbl;
    // // w->CLIPPING_DEP2(dbl);
    //
    // // other settings for all modes
    // in >> dbl;
    // w->BACKGROUND_DELAY(dbl);
    // // in >> dbl;
    // // w->BACKGROUND_DELAY2(dbl);
    // in >> dbl;
    // w->FISS_PILEUP_TIME(dbl);
    // in >> dbl;
    // w->FISS_PILEUP_TIME2(dbl);

    // CovEM settings
    if(CovEM_in)
    {
        in >> dbl;
        w->MIN_N_ERG(dbl);
        in >> dbl;
        w->MAX_N_ERG(dbl);
        in >> dbl;
        w->MIN_P_ERG(dbl);
        in >> dbl;
        w->MAX_P_ERG(dbl);
        in >> integer;
        w->BN(integer);
        in >> integer;
        w->BP(integer);
    }

    // beam settings
    if(mode == BEAM_MODE)
    {
        in >> dbl;
        w->BEAM_ERG_MIN(dbl);
        in >> dbl;
        w->BEAM_ERG_MAX(dbl);
        in >> integer;
        w->BEAM_ERG_BINNUM(integer);
    }
}
// OLD CODE

// readFiss::readFiss(int &argc, char** &argv)
// {
//   // get input from user
//   if(argc == 1)
//   {
//     GetInfo(cin);
//   }
//
//   // get input from file
//   else if(argc == 2)
//   {
//     ifstream fin(argv[1]);
//     // check if file opens properly
//     if(!fin.is_open())
//     {
//       cout << "Could not open " << argv[1] << endl;
//       BadInputMessage();
//       return;
//     }
//     GetInfo(fin);
//   }
//
//   // bad input
//   else
//   {
//     BadInputMessage();
//     return;
//   }
//
//   GenerateAngles();
//   InitializeHistograms();
//   InitializeFunctions();
//
//   // loop through
//   LoopExp();
//   if(mode == 1)
//   {
//     LoopSim();
//   }
//   if(mode == 2)
//   {
//     ReadBeamInfo();
//     BeamDepAnalysis();
//     BeamErgAnalysis();
//     FitMult();
//   }
//
//   // correlated analysis
//   CorrAnalysis();
//
//   // run CovEM if user wanted to
//   if(CovEM_in){
//     // covEM plot
//     CovEM();
//     WriteCovEM();
//   }
//
//   // plotting sections
//   PlotAll();
//   if(mode == 1)
//   {
//     CompareAll();
//   }
//
//   if(mode == 2)
//   {
//     PlotDepSubtraction();
//     PlotRatioMult();
//     PlotMultErg();
//     PlotStack();
//     PlotLO();
//   }
//
//   writeFile->Close();
// }
//
// // tell user how to use input
// void readFiss::BadInputMessage()
// {
//   cout << "Bad input. Please run with no arguments and input options manually" <<
//           " or run with a text file containing your input.\n" << endl;
// }
//
// // gets input from istream, which should either point to a file or cin
// void readFiss::GetInfo(istream &inputStream)
// {
//   // get mode from user
//   cout << "What mode? \n" <<
//           "Options: \n " <<
//           "0: Experimental data only \n " <<
//           "1: Experimental and simulation data \n " <<
//           "2: Experimental and beam data" << endl;
//   inputStream >> mode;
//   cout << " Using mode " << mode << endl;
//
//   while(!(mode == 0 || mode == 1 || mode == 2))
//   {
//     cout << "Invalid mode. Please input 0, 1, or 2." << endl;
//     inputStream >> mode;
//     cout << " Using mode " << mode << endl;
//   }
//   cout << "\n";
//
//
//
//   // get writeFile from user
//   cout << "Input writeFile path" << endl;
//   inputStream >> nameWrite;
//   cout << " Using writeFile " << nameWrite << endl;
//   cout << "\n";
//
//   // get writeFile ready
//   writeFile = new TFile((TString)nameWrite, "RECREATE");
//   cd_basics = writeFile->mkdir("Basic");
//   cd_individual = writeFile->mkdir("Individual");
//   cd_FAME = writeFile->mkdir("FAME");
//   cd_correlated = writeFile->mkdir("Correlated");
//   cd_trigger = writeFile->mkdir("Trigger");
//
// /*
//  ___                _     ___ _ _
// |_ _|_ _  _ __ _  _| |_  | __(_) |___ ___
// | || ' \| '_ \ || |  _| | _|| | / -_|_-<
// |___|_||_| .__/\_,_|\__| |_| |_|_\___/__/
//         |_|
// */
//
//   // get expFile from user
//   cout << "Input expFile path (without .root extension)" << endl;
//   inputStream >> nameExp;
//
//   cout << "Input number of exp files" << endl;
//   inputStream >> numExpFiles;
//   cout << "rading " << numExpFiles << " files." << endl;
//
//   // initialize experiment tree
//   // TTree* tree;
//   TChain *expChain = new TChain("Fiss");
//   cout << "Initializing experiment tree from " << nameExp << endl;
//   TString nameExpAdd = (TString)nameExp + ".root";
//   expChain->Add(nameExpAdd);
//   for(int expInd = 1; expInd < numExpFiles; expInd++)
//   {
//     nameExpAdd = (TString)nameExp + "_" + (TString)to_string(expInd) + ".root";
//     expChain->Add(nameExpAdd);
//   }
//   InitExp(expChain);
//
//   // if in simulation mode, get simFile from user and initialize simulation tree
//   if(mode == 1)
//   {
//     cd_simComparison = writeFile->mkdir("SimComparison");
//     cout << "Input simFile path (without .root extension)" << endl;
//     inputStream >> nameSim;
//
//     cout << "Input number of sim files" << endl;
//     inputStream >> numSimFiles;
//
//     TChain *simChain = new TChain("fissionTree");
//     // cout << "Initializing simulation tree from " << nameSim << endl;
//     // simFile = (TFile*)gROOT->GetListOfFiles()->FindObject(nameSim);
//     // if (!simFile || !simFile->IsOpen())
//     // {
//     //     simFile = new TFile(nameSim);
//     // }
//     // simFile->GetObject("fissionTree", tree);
//     TString nameSimAdd = (TString)nameSim + ".root";
//     simChain->Add(nameSimAdd);
//     for(int simInd = 1; simInd < numSimFiles; simInd++)
//     {
//       nameSimAdd = (TString)nameSim + "_" + (TString)to_string(simInd) + ".root";
//       simChain->Add(nameSimAdd);
//     }
//     InitSim(simChain);
//   }
//
//   // if in beam mode, get beamFile from user and prep directories
//   else if(mode == 2)
//   {
//     cd_beam = writeFile->mkdir("Beam");
//     //******
//     cd_alphaSub = cd_beam->mkdir("AlphaSubtraction");
//     cd_multRatio = cd_beam->mkdir("MultiplicityRatio");
//     cd_multErg = cd_beam->mkdir("MultipicityErg");
//     cd_stack = cd_beam->mkdir("FissionSpectrumDueToBeamErg");
//     cd_LO = cd_beam->mkdir("LightOutput");
//     //******
//     cout << "Input beamFile path" << endl;
//     inputStream >> nameBeam;
//     cout << "Reading beam information from " << nameBeam << endl;
//   }
//
//   cout << "\n";
//
//   cout << "Input coordinate file path" << endl;
//   inputStream >> nameCoords;
//   cout << "Using coordinate file " << nameCoords << "\n" << endl;
//
// /*
//   _   _               ___                _
//  | | | |___ ___ _ _  |_ _|_ _  _ __ _  _| |_
//  | |_| (_-</ -_) '_|  | || ' \| '_ \ || |  _|
//   \___//__/\___|_|   |___|_||_| .__/\_,_|\__|
//                               |_|
// */
//   // get triggers and trigger numbers from user
//   cout << "Input number of channels. Sample input: \n 10 \n";
//   inputStream >> NUM_TRIGGERS;
//   cout << " Using " << NUM_TRIGGERS << " triggers. \n\n";
//
//   TRIGGERS = new int[NUM_TRIGGERS];
//
//   cout << "Input trigger numbers. \n";
//   for(int i = 0; i < NUM_TRIGGERS; ++i)
//   {
//     inputStream >> TRIGGERS[i];
//   }
//
//   cout << " Using trigger numbers ";
//   for(int i = 0; i < NUM_TRIGGERS; ++i)
//   {
//     cout << TRIGGERS[i] << " ";
//   }
//   cout << "\n\n";
//
//   // get detectors from user
//   cout << "Input number of detectors. Sample input: \n 40 \n";
//   inputStream >> NUM_DETECTORS;
//   cout << " Using " << NUM_DETECTORS << " detectors. \n\n";
//
//   /*DETECTORS = new int[NUM_DETECTORS];
//
//   cout << "Input detector numbers. \n";
//   for(int i = 0; i < NUM_DETECTORS; ++i)
//   {
//     inputStream >> DETECTORS[i];
//   }
//
//   cout << " Using detector numbers ";
//   for(int i = 0; i < NUM_DETECTORS; ++i)
//   {
//     cout << DETECTORS[i] << " ";
//   }
//   cout << "\n\n";*/
//
//   // get threshold and max time from user
//   cout << "Input detector threshold [MeVee], detector clipping [MeVee] and max neutron time [ns]. Sample input: \n 0.20 4.0 70.0" << endl;
//   inputStream >> THRESHOLD >> CLIPPING >> MAX_TIME_N;
//   cout << " Using: detector threshold = " << THRESHOLD << " MeVee, "
//        << " detector clipping = " << CLIPPING << " MeVee, "
//        << " Tmax = " << MAX_TIME_N << " ns." << endl;
//   cout << "\n";
//
//   cout << "Input trigger threshold [V us], and trigger clipping [V us].\nSample input: 0.005 0.1" << endl;
//   inputStream >> THRESHOLD_DEP >> CLIPPING_DEP;
//   cout << " Using: trigger threshold = " << THRESHOLD_DEP << " V us, "
//        << " fission chamber clipping = " << CLIPPING_DEP << " V us, "<< endl;
//   cout << "\n";
//
//
//   // ask user for background delay
//   cout << "background delay to visualize background (ns), put 0 if unsure" << endl;
//   inputStream >> BACKGROUND_DELAY;
//   cout << "using " << BACKGROUND_DELAY << " ns" << endl;
//   cout << "\n";
//
//   // ask user for background delay
//   cout << "time for separation of pileup fissions (ns): " << endl;
//   inputStream >> FISS_PILEUP_TIME;
//   cout << "using " << FISS_PILEUP_TIME << " ns" << endl;
//   cout << "\n";
//
//   // ask user if they want to use CovEM
//   cout << "Input 1 for CovEM, input 0 for no CovEM" << endl;
//   inputStream >> CovEM_in;
//   if(CovEM_in) cout << " Using CovEM" << endl;
//   else cout << " Not using CovEM" << endl;
//   cout << "\n\n";
//
//   // get CovEM options from user if they want to use CovEM
//   if(CovEM_in)
//   {
//     cout << "Input min/max energy limits for neutrons and photons. Sample input:" <<
//             "\n 0 10.0 0 4.0" << endl;
//     inputStream >> MIN_N_ERG >> MAX_N_ERG >> MIN_P_ERG >> MAX_P_ERG;
//     cout << " Using neutron energy range " << MIN_N_ERG << " to " << MAX_N_ERG <<
//             " and photon energy range " << MIN_P_ERG << " to " << MAX_P_ERG << endl;
//     cout << "\n";
//
//     cout << "Input BN and BP. Sample input: \n 10 50" << endl;
//     inputStream >> BN >> BP;
//     cout << " Using BN = " << BN << " and BP = " << BP << ". \n";
//     cout << "\n";
//   }
//
//   // get beam specs from user
//   if(mode == 2)
//   {
//     cout << "Input the minimum and maximum beam energies [MeV], and the # of energy bins" << endl;
//     inputStream >> BEAM_ERG_MIN >> BEAM_ERG_MAX >> BEAM_ERG_BINNUM;
//     cout << "\nUsing beam range between " << BEAM_ERG_MIN << " and "
//          << BEAM_ERG_MAX << " MeV, "
//          << BEAM_ERG_BINNUM << " bins" << endl;
//   }
//
//   // histogram visual parameters, get from user?
//   legend_x1 = 0.48;
//   legend_x2 = 0.8;
//   legend_y1 = 0.7;
//   legend_y2 = 0.9;
//
//   x_labelSize = 0.04;
//   y_labelSize = 0.04;
//
//   x_tickSize = 0.05;
//   y_tickSize = 0.03;
// }
