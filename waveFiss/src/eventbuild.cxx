////////////////////////////////////////////////////////////////////////////////
///  Shea M. Mosby
///  smosby@lanl.gov
///  Devin S. Connolly
///  dconnolly@lanl.gov
///  eventbuild.cxx
////////////////////////////////////////////////////////////////////////////////

// C/C++ includes
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

// ROOT includes
#include "TTree.h"
#include "TLeaf.h"
#include "TFile.h"

// File includes
#include "WaveformProcessing.h"

int main(int argc, char* argv[])
{
  // Creation of variables section
  char   ipath[512];
  char   opath[512];
  char   buf[512];
  string cfgfile;

#ifdef SAVE_WAVEFORMS
  // Initialize the waveform processing settings
  Double_t    anodeSF = 1.0;
  TKESettings tke;
  /////////////////////////////////////////////////////
  // SET UP YOUR WAVEFORM PROCESSING PARAMETERS HERE //
  /////////////////////////////////////////////////////
  // defaults
  tke.reset();
  Int_t i_bnum  = 0;
  Int_t i_chnum = 0;
  // sum grid + anode
  for (i_chnum = 0; i_chnum < 2; ++i_chnum) {
    tke.Ns[i_bnum][i_chnum]        = WF_SIZE;
    tke.thresh[i_bnum][i_chnum]    = -1;
    tke.polarity[i_bnum][i_chnum]  = "positive";
    tke.bLineOS[i_bnum][i_chnum]   = 10;
    tke.bLineNpts[i_bnum][i_chnum] = 100;
    tke.eMethod[i_bnum][i_chnum]   = "trapFilter";
    tke.tRise[i_bnum][i_chnum]     = 400;
    tke.tGap[i_bnum][i_chnum]      = 100;
    tke.tPeak[i_bnum][i_chnum]     = tke.tGap[i_bnum][i_chnum] - 1;
    tke.nPeak[i_bnum][i_chnum]     = 2;
    tke.tMethod[i_bnum][i_chnum]   = "ffCFD";
    tke.ffRise[i_bnum][i_chnum]    = 16;
    tke.delay[i_bnum][i_chnum]     = 4;
    tke.frac[i_bnum][i_chnum]      = 0.98;
    tke.interp[i_bnum][i_chnum]    = "cubic";
    tke.nZero[i_bnum][i_chnum]     = 4;
    tke.tOffset[i_bnum][i_chnum]   = 350;
    tke.tau[i_bnum][i_chnum]       = 2.50e4;
    // tke.phGuess[i_bnum][i_chnum]   = 550;
    // tke.sampleDelta[i_bnum][i_chnum] = 1;
    // tke.derivePointDelta[i_bnum][i_chnum] = 3;
    // tke.nPeak[i_bnum][i_chnum] = 300; // for U5
    // tke.tOffset[i_bnum][i_chnum] = 400; // for U5
    // tke.bLineNpts[i_bnum][i_chnum] = 150; // good for U5
  }
#endif

  printf("\n");
  printf(" _______________________________________________________\n");
  printf("|                                                       |\n");
  printf("|                 CAEN Event Building                   |\n");
  printf("|-------------------------------------------------------|\n");
  printf("|          Analyze DPP-enabled CAEN digital ouput       |\n");
  printf("|                                                       |\n");
  printf("|           (send bugs to dconnolly@lanl.gov)           |\n");
  printf("|_______________________________________________________|\n");
  printf("\n");
  // Parsing of input parameters
  if (argc == 3) {
    sprintf(ipath, "%s", argv[1]);
    sprintf(opath, "%s", argv[2]);
  } else if (argc == 4) {
    sprintf(ipath, "%s", argv[1]);
    sprintf(opath, "%s", argv[2]);
    cfgfile = argv[3];

#ifdef SAVE_WAVEFORMS
    cout << "Reading config file\n";
    ifstream cfgf;
    cfgf.open(cfgfile.c_str());

    if (!(cfgf.is_open())) {
      cout << "!!!Error!!! could not open config file - using default analysis parameters\n";
    } else {
      // mmsg.str("");
      // mmsg << "Configuration File " << cfgfile.c_str() << " is Open";
      // Spider_Success("Main", mmsg.str());
      string item;
      string line;
      while (getline(cfgf, line)) {
        if (line.compare(0, 1, "#") == 0) {
          continue;
        } else {
          cfgf >> item;
          if (item.compare("sum_eMethod") == 0) { cfgf >> tke.eMethod[i_bnum][0]; }
          if (item.compare("sum_tMethod") == 0) { cfgf >> tke.tMethod[i_bnum][0]; }
          if (item.compare("sum_interp") == 0) { cfgf >> tke.interp[i_bnum][0]; }
          if (item.compare("sum_Ns") == 0) { cfgf >> tke.Ns[i_bnum][0]; }
          if (item.compare("sum_thresh") == 0) { cfgf >> tke.thresh[i_bnum][0]; }
          if (item.compare("sum_bLineOS") == 0) { cfgf >> tke.bLineOS[i_bnum][0]; }
          if (item.compare("sum_bLineNpts") == 0) { cfgf >> tke.bLineNpts[i_bnum][0]; }
          if (item.compare("sum_nPeak") == 0) { cfgf >> tke.nPeak[i_bnum][0]; }
          if (item.compare("sum_nZero") == 0) { cfgf >> tke.nZero[i_bnum][0]; }
          if (item.compare("sum_tOffset") == 0) { cfgf >> tke.tOffset[i_bnum][0]; }
          if (item.compare("sum_phGuess") == 0) { cfgf >> tke.phGuess[i_bnum][0]; }
          if (item.compare("sum_tau") == 0) { cfgf >> tke.tau[i_bnum][0]; }
          if (item.compare("sum_ffRise") == 0) { cfgf >> tke.ffRise[i_bnum][0]; }
          if (item.compare("sum_tRise") == 0) { cfgf >> tke.tRise[i_bnum][0]; }
          if (item.compare("sum_tGap") == 0) { cfgf >> tke.tGap[i_bnum][0]; }
          if (item.compare("sum_delay") == 0) { cfgf >> tke.delay[i_bnum][0]; }
          if (item.compare("sum_frac") == 0) { cfgf >> tke.frac[i_bnum][0]; }
          if (item.compare("sum_scaleFactor") == 0) { cfgf >> anodeSF; }
        }
      }
    }

    cfgf.close();

    tke.Ns[i_bnum][1]        = tke.Ns[i_bnum][0];
    tke.thresh[i_bnum][1]    = tke.thresh[i_bnum][0];
    tke.polarity[i_bnum][1]  = tke.polarity[i_bnum][0];
    tke.bLineOS[i_bnum][1]   = tke.bLineOS[i_bnum][0];
    tke.bLineNpts[i_bnum][1] = tke.bLineNpts[i_bnum][0];
    tke.eMethod[i_bnum][1]   = tke.eMethod[i_bnum][0];
    tke.tRise[i_bnum][1]     = tke.tRise[i_bnum][0];
    tke.tGap[i_bnum][1]      = tke.tGap[i_bnum][0];
    tke.tPeak[i_bnum][1]     = tke.tPeak[i_bnum][0];
    tke.nPeak[i_bnum][1]     = tke.nPeak[i_bnum][0];
    tke.nZero[i_bnum][1]     = tke.nZero[i_bnum][0];
    tke.tMethod[i_bnum][1]   = tke.tMethod[i_bnum][0];
    tke.ffRise[i_bnum][1]    = tke.ffRise[i_bnum][0];
    tke.delay[i_bnum][1]     = tke.delay[i_bnum][0];
    tke.frac[i_bnum][1]      = tke.frac[i_bnum][0];
    tke.interp[i_bnum][1]    = tke.interp[i_bnum][0];
    tke.tOffset[i_bnum][1]   = tke.tOffset[i_bnum][0];
    tke.tau[i_bnum][1]       = tke.tau[i_bnum][0];

    cout << setw(32) << left << "sum grids + anodes: "
         << "\n";
    cout << setw(32) << left << "\tpolarity: " << left << tke.polarity[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tE method: " << left << tke.eMethod[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tT method: " << left << tke.tMethod[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tZero interpolation: " << left << tke.interp[i_bnum][0] << "\n";
    cout << setw(32) << left << "\t# interpolation pts: " << left << tke.nZero[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tN samples: " << left << tke.Ns[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tthreshold: " << left << tke.thresh[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tbaseline offset: " << left << tke.bLineOS[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tbaseline N pts: " << left << tke.bLineNpts[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tpeak npts: " << left << tke.nPeak[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tpeak offset: " << left << tke.tOffset[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tpeak height guess: " << left << tke.phGuess[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tdecay constant: " << left << tke.tau[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tfast filter rise time: " << left << tke.ffRise[i_bnum][0]
         << "\n";
    cout << setw(32) << left << "\ttrapezoid rise time: " << left << tke.tRise[i_bnum][0] << "\n";
    cout << setw(32) << left << "\ttrapezoid gap time: " << left << tke.tGap[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tcfd delay: " << left << tke.delay[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tcfd fraction: " << left << tke.frac[i_bnum][0] << "\n";
    cout << setw(32) << left << "\tanode scale factor: " << left << anodeSF << "\n";
#endif
  } else {
    printf("--> EXIT:\nPlease, start event building as follows: \n");
    printf("--> EXIT: eventbuild string=PATH_TO_INPUT_FILE string=PATH_TO_OUTPUT_FILE\n");
    return -1;
  }

  // Get file / tree paths set
  TFile*             ifile       = new TFile(ipath);
  const Short_t      board_ana   = 0; // what board to be analyzing
  const Short_t      channel_ana = 8; // number of channels to analyze
  TTree*             itree[channel_ana];
  ProcessedRootEvent proc_evt[channel_ana];
  if (ifile->IsZombie()) {
    cout << "Error opening file " << ifile << "Aborting\n";
    exit(-1);
  }
  for (int eye = 0; eye < channel_ana; eye++) {
    sprintf(buf, "pt%i_%i", board_ana, eye);
    if (!(ifile->GetListOfKeys()->Contains(buf))) {
      printf("Error in file %s - does not contain singles tree pt%d_%d! Aborting\n", ipath,
             board_ana, channel_ana);
      exit(-1);
    }
  }
  TFile* ofile = new TFile(opath, "recreate");
  TTree* otree[2];
  otree[0] = new TTree("et", "event tree from waveform analysis");
#ifndef GAMMAS_OFF
  otree[1] = new TTree("etg", "event tree from waveform analysis - gamma rays");
  GammaSingles gs_evt;
#endif
  // set up tree representing classes
#ifdef SAVE_WAVEFORMS
  CAEN_DGTZ_Event sum0_raw, sum1_raw;
#endif
  for (int eye = 0; eye < channel_ana; eye++) {
    sprintf(buf, "pt%i_%i", board_ana, eye);
    itree[eye] = (TTree*)ifile->Get(buf);
    proc_evt[eye].reset();
    itree[eye]->SetBranchAddress("ts", &proc_evt[eye].ts);
    itree[eye]->SetBranchAddress("tTrig", &proc_evt[eye].tTrig);
    itree[eye]->SetBranchAddress("peak", &proc_evt[eye].peak);
    itree[eye]->SetBranchAddress("baseline", &proc_evt[eye].baseline);
  }
  EventInfo coinc_evt[2];
  // create branches
  cout << "Creating output tree\n";
  for (int eye = 0; eye < 1; ++eye) {
    coinc_evt[eye].reset();
    otree[eye]->Branch("t0t", &coinc_evt[eye].t0t); // beam time
    otree[eye]->Branch("ct", &coinc_evt[eye].ct); // cathode time
    otree[eye]->Branch("dt", &coinc_evt[eye].dt); // delta beam cathode
    otree[eye]->Branch("cph", &coinc_evt[eye].cph); // cathode pulse height
    otree[eye]->Branch("tTrig[8]", coinc_evt[eye].tTrig); // time trigger for all channels
    otree[eye]->Branch("aph[2]", coinc_evt[eye].aph); // anode pulse height
    otree[eye]->Branch("gph[2]", coinc_evt[eye].gph); // grid pulse height
    otree[eye]->Branch("sumph[2]", coinc_evt[eye].sumph); // sum pulse height
    otree[eye]->Branch("sumtTrig[2]", coinc_evt[eye].sumtTrig);
    otree[eye]->Branch("sumtPeak[2]", coinc_evt[eye].sumtPeak);
    otree[eye]->Branch("ccoinc", &coinc_evt[eye].ccoinc);
#ifdef SAVE_WAVEFORMS
    sprintf(buf, "a0wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].a0wf);
    sprintf(buf, "a1wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].a1wf);
    sprintf(buf, "g0wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].g0wf);
    sprintf(buf, "g1wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].g1wf);
    sprintf(buf, "sum0wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum0wf);
    sprintf(buf, "sum1wf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum1wf);
#ifdef WRITE_FILTERS
    sprintf(buf, "sum0Eflt[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum0Eflt);
    sprintf(buf, "sum1Eflt[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum1Eflt);
    sprintf(buf, "sum0Tflt[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum0Tflt);
    sprintf(buf, "sum1Tflt[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].sum1Tflt);
#endif
    sprintf(buf, "cwf[%i]", WF_SIZE);
    otree[eye]->Branch(buf, coinc_evt[eye].cwf);
#endif
#ifndef GAMMAS_OFF
    otree[eye]->Branch("gt[2]", coinc_evt[eye].gt);
    otree[eye]->Branch("ge[2]", coinc_evt[eye].ge);
    otree[eye]->Branch("gtl[10]", coinc_evt[eye].gtl);
    otree[eye]->Branch("gel[10]", coinc_evt[eye].gel);
    otree[eye]->Branch("gchl[10]", coinc_evt[eye].gchl);
    otree[eye]->Branch("gmult", &coinc_evt[eye].gmult);
#endif
  }
#ifndef GAMMAS_OFF
  // cout << "about to look at singles tree\n";
  gs_evt.reset();
  otree[1]->Branch("bnum", &gs_evt.bnum);
  otree[1]->Branch("chnum", &gs_evt.chnum);
  otree[1]->Branch("t0t", &gs_evt.t0t);
  otree[1]->Branch("dt", &gs_evt.dt);
  otree[1]->Branch("ge", &gs_evt.ge);
  otree[1]->Branch("baseline", &gs_evt.baseline);
#endif
  /////////////////////////////////////////////////
  // SET UP EVENT RECONSTRUCTION PARAMETERS HERE //
  /////////////////////////////////////////////////

  // General note, we're still working in dig units - sample rate is 4 ns
  // Also, we'll do the event reconstruction based on the trigger time tag
  // but tof based on everything
  // the written tof output will be in ns
  // also, remember we reconstruct relative to the macropulse right now
  // and finally, we've got 2 trees. The first one is for the TKE experiment
  // and the 2nd is for gamma rays that don't coincide with any fission event

  // event reconstruction offsets (dig units)
  Long64_t rt_offset[channel_ana];
  rt_offset[0] = 0;    // t0
  rt_offset[1] = 0;    // gamma detector 0
  rt_offset[2] = -250; // anode 0
  rt_offset[3] = -250; // anode 1
  rt_offset[4] = -25;  // grid 0
  rt_offset[5] = -25;  // grid 1
  rt_offset[6] = 0;    // cathode
  rt_offset[7] = 0;    // empty
  /*
    rt_offset[0] = 0; // gamma detector 0
    rt_offset[1] = 0; // gamma detector 1
    rt_offset[2] = 0; // t0
    rt_offset[3] = -250; // anode 0
    rt_offset[4] = -250; // anode 1
    rt_offset[5] = -25; // grid 0
    rt_offset[6] = -25; // grid 1
    rt_offset[7] = 0; // cathode
  */

  // reconstruction window (dig units, goes on either side)
  Long64_t recon_window = 250;

  Double_t dig2ns = 4.; // sample rate of the digitizer

  // time of flight calculation offsets (ns)
  Double_t tof_offset[channel_ana];
  tof_offset[0] = 0; // gamma detector 0
  tof_offset[1] = 0; // gamma detector 1
  tof_offset[2] = 0; // t0
  tof_offset[3] = 0; // anode 0
  tof_offset[4] = 0; // anode 1
  tof_offset[5] = 0; // grid 0
  tof_offset[6] = 0; // grid 1
  tof_offset[7] = 0; // cathode

  ULong64_t current_index[channel_ana];
  for (int eye = 0; eye < channel_ana; eye++) { current_index[eye] = itree[eye]->GetEntries() - 1; }
  Int_t t0_channel      = 0;
  Int_t cathode_channel = 6;

  const Int_t n_other_ic_channels                    = 4;
  Int_t       other_ic_channels[n_other_ic_channels] = {2, 3, 4, 5};
  // Int_t other_ic_channels[n_other_ic_channels] = {3};

  ////////////////////////////////////////////////
  // END SET UP EVENT RECONSTRUCTION PARAMETERS //
  ////////////////////////////////////////////////
  // we reverse loop over everything for "simplicity"
  cout << "Building events\n";
  for (int t0i = current_index[t0_channel]; t0i > -1; --t0i) 
  {
    

    if (t0i % 100 == 0) 
    {
      cout << "\r"
           << "t0 index " << t0i;
    }


    itree[t0_channel]->GetEntry(t0i);
    // if (proc_evt[t0_channel].peak>-2500.) continue;
    // if (proc_evt[t0_channel].peak > -500) continue;


    int cathode_i = current_index[cathode_channel];
    while (true) // this looks for fission events - cathode is the reference
    {
      if (cathode_i < 0) break;
      coinc_evt[0].reset();

      ////////////////////////////////////
      // CATHODE / T0 COINCIDENCE BLOCK //
      ////////////////////////////////////
      itree[cathode_channel]->GetEntry(cathode_i);
      // break out if we are less than the t0
      if (proc_evt[cathode_channel].ts + rt_offset[cathode_channel] <
          proc_evt[t0_channel].ts + rt_offset[t0_channel]) {
        break;
      }

      // cout << "t0 time " << proc_evt[0].ts+rt_offset[0] << " cathode time " <<
      // proc_evt[cathode_channel].ts+rt_offset[cathode_channel] << " difference " <<  endl;
      // otherwise keep trying to reconstruct
      coinc_evt[0].tTrig[t0_channel]      = proc_evt[t0_channel].tTrig;
      coinc_evt[0].tTrig[cathode_channel] = proc_evt[cathode_channel].tTrig;
      coinc_evt[0].t0t = dig2ns * ((double)(proc_evt[t0_channel].ts + rt_offset[t0_channel]) +
                                   proc_evt[t0_channel].tTrig) +
                         tof_offset[t0_channel];
      coinc_evt[0].ct =
          dig2ns * ((double)(proc_evt[cathode_channel].ts + rt_offset[cathode_channel]) +
                    proc_evt[cathode_channel].tTrig) +
          tof_offset[cathode_channel];
      coinc_evt[0].cph = proc_evt[cathode_channel].peak;
      coinc_evt[0].dt  = coinc_evt[0].ct - coinc_evt[0].t0t;
      for (int eye = 0; eye < WF_SIZE; eye++) {
        coinc_evt[0].cwf[eye] = itree[cathode_channel]->GetLeaf("wf")->GetValue(eye);
      }
      // coinc_evt[0].dt = proc_evt[cathode_channel].ts-proc_evt[t0_channel].ts;
      cathode_i -= 1;
      ///////////////////////
      // OTHER IC CHANNELS //
      ///////////////////////
      for (int ic_loop = 0; ic_loop < n_other_ic_channels; ++ic_loop) {
        int ic_channel = other_ic_channels[ic_loop];
        int ic_i       = current_index[ic_channel];
        while (true) {
          if (ic_i < 0) break;
          itree[ic_channel]->GetEntry(ic_i);
          // break out if we are less than low limit of coinc window
          Long64_t ic_channel_time = proc_evt[ic_channel].ts + rt_offset[ic_channel];
          Long64_t cathode_time    = proc_evt[cathode_channel].ts + rt_offset[cathode_channel];
          // cout << "channel " << ic_channel;
          if (ic_channel_time < (cathode_time - recon_window)) {
            break;
          } else if (ic_channel_time > (cathode_time + recon_window)) {
            // continue on if we are greater than high limit of coinc window
            switch (ic_channel) {
#ifndef GAMMAS_OFF
            case 1:
              gs_evt.bnum  = 0;
              gs_evt.chnum = ic_channel;
              gs_evt.t0t   = coinc_evt[0].t0t;
              gs_evt.dt    = dig2ns * ((double)(proc_evt[ic_channel].ts + rt_offset[ic_channel] +
                                             proc_evt[ic_channel].tTrig + tof_offset[ic_channel])) -
                          coinc_evt[0].t0t;
              gs_evt.ge = proc_evt[ic_channel].peak;
              otree[1]->Fill();
              break;
              /*
                case 1:
                gs_evt.bnum = 0;
                gs_evt.chnum = ic_channel;
                gs_evt.chnum = ic_channel;
                gs_evt.t0t = coinc_evt[0].t0t;
                gs_evt.dt =  dig2ns*((double)(proc_evt[ic_channel].ts + rt_offset[ic_channel] +
                proc_evt[ic_channel].tTrig + tof_offset[ic_channel])) - coinc_evt[0].t0t; gs_evt.ge
                = proc_evt[ic_channel].peak; otree[1]->Fill(); break;
              */
#endif
            default: break;
            }
            ic_i -= 1;
            current_index[ic_channel] = ic_i;
            continue;
          } else {
            // if we are inside the coinc window we actually do something
            // if ((cathode_time - recon_window) < ic_channel_time < (cathode_time + recon_window))
            // {
            switch (ic_channel) {
            case 2:
              coinc_evt[0].aph[0]            = proc_evt[ic_channel].peak;
              coinc_evt[0].tTrig[ic_channel] = proc_evt[ic_channel].tTrig;
#ifdef SAVE_WAVEFORMS
              for (int eye = 0; eye < WF_SIZE; eye++) {
                coinc_evt[0].a0wf[eye] = itree[ic_channel]->GetLeaf("wf")->GetValue(eye);
              }
#endif
              break;
            case 3:
              coinc_evt[0].aph[1]            = proc_evt[ic_channel].peak;
              coinc_evt[0].tTrig[ic_channel] = proc_evt[ic_channel].tTrig;
#ifdef SAVE_WAVEFORMS
              for (int eye = 0; eye < WF_SIZE; eye++) {
                coinc_evt[0].a1wf[eye] = itree[ic_channel]->GetLeaf("wf")->GetValue(eye);
              }
#endif
              break;
            case 4:
              coinc_evt[0].gph[0]            = proc_evt[ic_channel].peak;
              coinc_evt[0].tTrig[ic_channel] = proc_evt[ic_channel].tTrig;
#ifdef SAVE_WAVEFORMS
              for (int eye = 0; eye < WF_SIZE; eye++) {
                coinc_evt[0].g0wf[eye] = itree[ic_channel]->GetLeaf("wf")->GetValue(eye);
              }
#endif
              break;
            case 5:
              coinc_evt[0].gph[1]            = proc_evt[ic_channel].peak;
              coinc_evt[0].tTrig[ic_channel] = proc_evt[ic_channel].tTrig;
#ifdef SAVE_WAVEFORMS
              for (int eye = 0; eye < WF_SIZE; eye++) {
                coinc_evt[0].g1wf[eye] = itree[ic_channel]->GetLeaf("wf")->GetValue(eye);
              }
#endif
              break;
              // case 1:
              //   coinc_evt[0].gt[0] =
              //       dig2ns * (double)(proc_evt[ic_channel].ts + rt_offset[ic_channel]) -
              //       coinc_evt[0].ct;
              //   coinc_evt[0].ge[0] = proc_evt[ic_channel].peak;
              //   coinc_evt[0].gmult += 1;
              //   break;
              /*
                case 1:
                coinc_evt[0].gt[1] = dig2ns*(double)(proc_evt[ic_channel].ts +
                rt_offset[ic_channel]) - coinc_evt[0].ct; coinc_evt[0].ge[1] =
                proc_evt[ic_channel].peak; coinc_evt[0].gmult += 1; break;
              */
            default: break;
            }
          }
          ic_i -= 1;
          current_index[ic_channel] = ic_i;
        }
#ifdef SAVE_WAVEFORMS
        sum0_raw.reset(), sum1_raw.reset();
        sum0_raw.bnum = 0, sum0_raw.chnum = 0, sum1_raw.bnum = 0, sum1_raw.chnum = 1;
        for (int eye = 0; eye < WF_SIZE; eye++) {
          sum0_raw.wf[eye] = anodeSF * coinc_evt[0].a0wf[eye] + coinc_evt[0].g0wf[eye];
          sum1_raw.wf[eye] = anodeSF * coinc_evt[0].a1wf[eye] + coinc_evt[0].g1wf[eye];
        }
        Int_t retval = sum0_raw.processWf(tke, &coinc_evt[0].sumtTrig[0], &coinc_evt[0].sumtPeak[0],
                                          &coinc_evt[0].sumph[0], &coinc_evt[0].sumBl[0]);
        retval       = sum1_raw.processWf(tke, &coinc_evt[0].sumtTrig[1], &coinc_evt[0].sumtPeak[1],
                                    &coinc_evt[0].sumph[1], &coinc_evt[0].sumBl[1]);
        for (int eye = 0; eye < WF_SIZE; eye++) {
          coinc_evt[0].sum0wf[eye] = sum0_raw.wf[eye];
          coinc_evt[0].sum1wf[eye] = sum1_raw.wf[eye];
#ifdef WRITE_FILTERS
          coinc_evt[0].sum0Eflt[eye] = sum0_raw.Eflt[eye];
          coinc_evt[0].sum0Tflt[eye] = sum0_raw.Tflt[eye];
          coinc_evt[0].sum1Eflt[eye] = sum1_raw.Eflt[eye];
          coinc_evt[0].sum1Tflt[eye] = sum1_raw.Tflt[eye];
#endif
        }
#endif
      }
      otree[0]->Fill();
      current_index[cathode_channel] = cathode_i;
    }
  }
  cout << "\n";
  cout << "Writing tke event tree to rootfile...\n";
  otree[0]->Write();
#ifndef GAMMAS_OFF
  cout << "Writing gamma singles event tree to rootfile...\n";
  otree[1]->Write();
#endif
  cout << "...success!\n";
  ofile->Close();
  ifile->Close();
  return 0;
}
