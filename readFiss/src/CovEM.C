#include "readFiss.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMatrixD.h>
#include <TMath.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Parameters.h"

#include <sys/stat.h>
#include <sys/types.h>

// #include <filesystem>
//
//
// namespace fs = std::filesystem;
using namespace std;

void readFiss::CovEM()
{
//
//    cout << "Beginning the great CovEM loop" << endl;
//    // place to store all the result
//
//    h2_arrayCorr = new TH2D("CovMat", "Covariance Matrix; Neutron ToF Energy (MeV); Photon Deposited Energy (MeVee); counts", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
//    h2_arraySpec = new TH2D("SpecMat", "Spectrum Matrix; Neutron ToF Energy (MeV); Photon Deposited Energy (MeVee); counts", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
//
//    // make sure tree is open
//    if (expTree == 0) return;
//
//    // updated event-by-event history
//    int* listNerg;
//    int* listPerg;
//    listNerg = new int[BN];
//    listPerg = new int[BP];
//
//    // initialize the EM histograms
//    TString t_nameH;
//    TString t_rootName = "em_";
//    TString titleH = "EnergyMode Correlations";
//
//    TH2I*** emMode;
//    emMode = new TH2I** [BN];
//    for(int eN = 0; eN < BN; eN++)
//    {
//    	emMode[eN] = new TH2I* [BP];
//
//       for(int eP = 0; eP < BP; eP++)
//       {
//          t_nameH = t_rootName + to_string(eN) + " " + to_string(eP);
//          emMode[eN][eP] = new TH2I(t_nameH, titleH, 10,0,10, 10,0,10);
//       }
//    }
//
//    TH2I**** emModeChunks;
//    emModeChunks = new TH2I*** [NUM_CHUNKS];
//    for(int chunk = 0; chunk < NUM_CHUNKS; chunk++)
//    {
//      emModeChunks[chunk] = new TH2I** [BN];
//      for(int eN = 0; eN < BN; eN++)
//      {
//      	emModeChunks[chunk][eN] = new TH2I* [BP];
//
//         for(int eP = 0; eP < BP; eP++)
//         {
//            t_nameH = t_rootName + to_string(eN) + " " + to_string(eP) + " " + to_string(chunk);
//            emModeChunks[chunk][eN][eP] = new TH2I(t_nameH, titleH, 10,0,10, 10,0,10);
//         }
//      }
//    }
//
//
//    int encN, encP;
//    // read the entries
//    Long64_t nentries = expTree->GetEntriesFast();
//    nentries = 1000000;
//    Long64_t nbytes = 0, nb = 0;
//
//    for (Long64_t jentry=0; jentry<nentries;jentry++)
//    {
//
//       if(jentry%1000000 == 0)
//       {
//         cout << jentry << " entries done. " << endl;
//       }
//
//
//
//    	  // load the new entry
//       Long64_t ientry = LoadExpTree(jentry);
//       if (ientry < 0) break;
//       nb = expTree->GetEntry(jentry);   nbytes += nb;
//
//      // cout << "loaded the new entry " << endl;
//
//      // reset the event-by-event list to 0
//      for(int eN = 0; eN < BN; eN ++)
//      {
//       listNerg[eN] = 0;
//      }
//      for(int eP = 0; eP < BP; eP ++)
//      {
//       listPerg[eP] = 0;
//      }
//
//      // cout << "resetted lists" << endl;
//
//
//      // Neutron event-by-event spectrum
//      for(int nN = 0; nN < neutronMult; nN++)
//      {
//
//        if(neutronLightOut[nN] > THRESHOLD && neutronDetTimes[nN] < MAX_TIME_N)
//        {
//          encN = int((neutronToFErg[nN] - MIN_N_ERG)/sizeNerg);
//
//          if(encN >= BN)
//          {
//            encN = BN-1;
//          }
//          else if(encN < 0)
//          {
//            encN = 0;
//          }
//
//          listNerg[encN] += 1;
//        }
//
//      }
//
//      // cout << "encoded neutrons" << endl;
//
//      // Photon event-by-event spectrum
//      for(int nP = 0; nP < gammaMult; nP++)
//      {
//        encP = int((photonLightOut[nP] - MIN_P_ERG)/sizePerg);
//
//        if(encP >= BP)
//        {
//          encP = BP-1;
//        }
//        else if(encP < 0)
//        {
//          encP = 0;
//        }
//
//        listPerg[encP] += 1;
//      }
//
//      // cout << "encoded gammas" << endl;
//
//
//      // populate the em histograms
//      for(int eN = 0; eN < BN; eN ++)
//      {
//       for(int eP = 0; eP < BP; eP ++)
//        {
//         emMode[eN][eP]->Fill(listNerg[eN], listPerg[eP]);
//
//         if(SELF_ERROR)
//         {
//           emModeChunks[jentry%NUM_CHUNKS][eN][eP]->Fill(listNerg[eN], listPerg[eP]);
//         }
//        }
//      }
//
//      // cout << "filled EM histograms" << endl;
//
//     }
//
//    // find the covariance and spec at each point
//      for(int eN = 0; eN < BN; eN ++)
//      {
//       for(int eP = 0; eP < BP; eP ++)
//        {
//         h2_arrayCorr->SetBinContent(eN+1, eP+1, emMode[eN][eP]->GetCovariance());
//         h2_arraySpec->SetBinContent(eN+1, eP+1, (emMode[eN][eP]->GetMean(1))*(emMode[eN][eP]->GetMean(2)));
//
//        }
//      }
//
//      cout << "finished setting EM matrices " << endl;
//
//
//
//
//      if(SELF_ERROR)
//      {
//        double listCov[NUM_CHUNKS] = {0};
//        double listSpec[NUM_CHUNKS] = {0};
//
//        double totCov = 0;
//        double totSpec = 0;
//
//        cout << "computing self error from data fluctuations" << endl;
//        // find the covariance at each point
//          for(int eN = 0; eN < BN; eN ++)
//          {
//           for(int eP = 0; eP < BP; eP ++)
//            {
//              for(int chunk = 0; chunk < NUM_CHUNKS; chunk++)
//              {
//                double thisCov = emModeChunks[chunk][eN][eP]->GetCovariance();
//                double thisSpec = (emModeChunks[chunk][eN][eP]->GetMean(1))*(emMode[eN][eP]->GetMean(2));
//
//                listCov[chunk] = thisCov;
//                listSpec[chunk] = thisSpec;
//
//              }
//
//              h2_arrayCorr->SetBinError(eN+1, eP+1, TMath::StdDev(NUM_CHUNKS, listCov)/sqrt(NUM_CHUNKS-1) );
//              h2_arraySpec->SetBinError(eN+1, eP+1, TMath::StdDev(NUM_CHUNKS, listSpec)/sqrt(NUM_CHUNKS-1) );
//            }
//          }
//      }
//
//
}

void readFiss::ExtractCov()
{
  TH1D* h_Angles = new TH1D("Angles","angles between detectors",BA,MIN_THETA,MAX_THETA);
  cout << "extracting cov " << endl;

  arrayCorrExp = new double** [BA];
  arraySpecExp = new double** [BA];
  // *****************
  arrayCorrBack = new double** [BA];
  arraySpecBack = new double** [BA];

  if (mode == BEAM_MODE)
  {
    arrayCorrExpBeam = new double*** [BEAM_ERG_BINNUM];
    arraySpecExpBeam = new double*** [BEAM_ERG_BINNUM];
    arrayCorrBackBeam = new double*** [BEAM_ERG_BINNUM];
    arraySpecBackBeam = new double*** [BEAM_ERG_BINNUM];
    for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
    {
      arrayCorrExpBeam[bb] = new double** [BA];
      arraySpecExpBeam[bb] = new double** [BA];
      arrayCorrBackBeam[bb] = new double** [BA];
      arraySpecBackBeam[bb] = new double** [BA];
    }
  }
  // *******************
  for(int ba = 0; ba < BA; ba++)
  {
    arrayCorrExp[ba] = new double* [BN];
    arraySpecExp[ba] = new double* [BN];
    arrayCorrBack[ba] = new double* [BN];
    arraySpecBack[ba] = new double* [BN];

    // *******************
    if (mode == BEAM_MODE)
    {
      for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
      {
        arrayCorrExpBeam[bb][ba] = new double* [BN];
        arraySpecExpBeam[bb][ba] = new double* [BN];
        arrayCorrBackBeam[bb][ba] = new double* [BN];
        arraySpecBackBeam[bb][ba] = new double* [BN];
      }
    }
    // *********************
    for(int bn = 0; bn < BN; bn++)
    {
      arrayCorrExp[ba][bn] = new double [BP];
      arraySpecExp[ba][bn] = new double [BP];
      arrayCorrBack[ba][bn] = new double [BP];
      arraySpecBack[ba][bn] = new double [BP];

      // *********************
      if (mode == BEAM_MODE)
      {
        for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
        {
        arrayCorrExpBeam[bb][ba][bn] = new double [BP];
        arraySpecExpBeam[bb][ba][bn] = new double [BP];
        arrayCorrBackBeam[bb][ba][bn] = new double [BP];
        arraySpecBackBeam[bb][ba][bn] = new double [BP];
        }
      }
      // *********************
      for(int bp = 0; bp < BP; bp++)
      {
        arrayCorrExp[ba][bn][bp] = 0.;
        arraySpecExp[ba][bn][bp] = 0.;
        arrayCorrBack[ba][bn][bp] = 0.;
        arraySpecBack[ba][bn][bp] = 0.;

        // *********************
        if (mode == BEAM_MODE)
        {
          for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
          {
          arrayCorrExpBeam[bb][ba][bn][bp] = 0.;
          arraySpecExpBeam[bb][ba][bn][bp] = 0.;
          arrayCorrBackBeam[bb][ba][bn][bp] = 0.;
          arraySpecBackBeam[bb][ba][bn][bp] = 0.;
          }
        }
        // *********************
      }
    }
  }

  double covVal, specValN, specValP, specVal;
  int ngEnc;
  double totCovCheck = 0;
  double totSpecCheck = 0;
  // ************************
  double covValBack, specValNBack, specValPBack, specValBack;
  int* multPosBeam;
  double* covValBeam;
  double* specValNBeam;
  double* specValPBeam;
  double* specValBeam;
  int* multPosBeamBack;
  double* covValBeamBack;
  double* specValNBeamBack;
  double* specValPBeamBack;
  double* specValBeamBack;
  // int* listPerg;
  //    listNerg = new int[BN];
  multPosBeam = new int[BEAM_ERG_BINNUM];
  covValBeam = new double[BEAM_ERG_BINNUM];
  specValNBeam = new double[BEAM_ERG_BINNUM];
  specValPBeam = new double[BEAM_ERG_BINNUM];
  specValBeam = new double[BEAM_ERG_BINNUM];
  multPosBeamBack = new int[BEAM_ERG_BINNUM];
  covValBeamBack = new double[BEAM_ERG_BINNUM];
  specValNBeamBack = new double[BEAM_ERG_BINNUM];
  specValPBeamBack = new double[BEAM_ERG_BINNUM];
  specValBeamBack = new double[BEAM_ERG_BINNUM];
  // **************************
  for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
  {
    for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
    {
      if(d1 == d2) continue;
      ngEnc = int ((angles[d1][d2] - MIN_THETA)/sizeNgAng);
      // ***********
      h_Angles->Fill(angles[d1][d2]);
      // *****************
      // cout << MIN_THETA << " " << sizeNgAng << endl;
      // cout << angles[d1][d2] << ": " << ngEnc << endl;

      if(ngEnc >= BA) ngEnc = BA-1;
      else if(ngEnc < 0) ngEnc = 0;

      for(int e1 = 0; e1 < BN; e1++)
      {
        for(int e2 = 0; e2 < BP; e2++)
        {
          covVal = 0;
          specValN = 0;
          specValP = 0;
          specVal = 0;
// ********************
          covValBack = 0;
          specValNBack = 0;
          specValPBack = 0;
          specValBack = 0;
// ********************

          for(int n1 = 0; n1 < MAX_MULT_DET; n1++)
          {
            for(int n2 = 0; n2 < MAX_MULT_DET; n2++)
            {
              int multPos = arrayExp[d1][d2][e1][e2][n1][n2];
              covVal += multPos*n1*n2;
              specValN += multPos*n1;
              specValP += multPos*n2;
              //cout << multPos << " ";
              // ***********************
              // background
              int multPosBack = arrayBack[d1][d2][e1][e2][n1][n2];
              covValBack += multPosBack*n1*n2;
              specValNBack += multPosBack*n1;
              specValPBack += multPosBack*n2;

              // arrayBack[detN][detP][ergN][ergP][1][1]
              if (mode == BEAM_MODE)
              {
                for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
                {
                  multPosBeam[eB] = arrayExpBeam[eB][d1][d2][e1][e2][n1][n2];
                  covValBeam[eB] += multPosBeam[eB]*n1*n2;
                  specValNBeam[eB] += multPosBeam[eB]*n1;
                  specValPBeam[eB] += multPosBeam[eB]*n2;

                  // Background
                  multPosBeamBack[eB] = arrayBackBeam[eB][d1][d2][e1][e2][n1][n2];
                  covValBeamBack[eB] += multPosBeamBack[eB]*n1*n2;
                  specValNBeamBack[eB] += multPosBeamBack[eB]*n1;
                  specValPBeamBack[eB] += multPosBeamBack[eB]*n2;

                }
              }
              // ***********************
            }
            //cout << endl;
          }
          covVal /= expEntries;
          specValN /= expEntries;
          specValP /= expEntries;

          covVal = (covVal - specValN*specValP);
          specVal = (specValN*specValP);

          //cout << covVal << endl;

          arrayCorrExp[ngEnc][e1][e2] += covVal;
          arraySpecExp[ngEnc][e1][e2] += specVal;
          totCovCheck += covVal;
          totSpecCheck += specVal;

          // Background
          covValBack /= expEntries;
          specValNBack /= expEntries;
          specValPBack /= expEntries;

          covValBack = (covValBack - specValNBack*specValPBack);
          specValBack = (specValNBack*specValPBack);


          arrayCorrBack[ngEnc][e1][e2] += covValBack;
          arraySpecBack[ngEnc][e1][e2] += specValBack;
          // totCovCheck += covVal;
          // totSpecCheck += specVal;


          // *********************
          if (mode == BEAM_MODE)
          {
            double beamExpEntries;
            for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
            {
              beamExpEntries = 0;
              for (int chan = 0; chan < NUM_TRIGGERS; chan++)
              {
                beamExpEntries += h_beamErg[chan]->GetBinContent(eB+1);
              }
              covValBeam[eB] /= beamExpEntries;
              specValNBeam[eB] /= beamExpEntries;
              specValPBeam[eB] /= beamExpEntries;

              covValBeam[eB] = (covValBeam[eB] - specValNBeam[eB]*specValPBeam[eB]);
              specValBeam[eB] = (specValNBeam[eB]*specValPBeam[eB]);

              arrayCorrExpBeam[eB][ngEnc][e1][e2] += covValBeam[eB];
              arraySpecExpBeam[eB][ngEnc][e1][e2] += specValBeam[eB];
              // totCovCheck += covVal;
              // totSpecCheck += specVal;

              // Background
              covValBeamBack[eB] /= beamExpEntries;
              specValNBeamBack[eB] /= beamExpEntries;
              specValPBeamBack[eB] /= beamExpEntries;

              covValBeamBack[eB] = (covValBeamBack[eB] - specValNBeamBack[eB]*specValPBeamBack[eB]);
              specValBeamBack[eB] = (specValNBeamBack[eB]*specValPBeamBack[eB]);
              arrayCorrBackBeam[eB][ngEnc][e1][e2] += covValBeamBack[eB];
              arraySpecBackBeam[eB][ngEnc][e1][e2] += specValBeamBack[eB];
              // totCovBackCheck += covValBack;
              // totSpecBackCheck += specValBack;
            }
          }


          // Correlation histogram? Ratio of cov/spec

          // ******************

          // cout << arrayCorrExp[ngEnc][e1][e2] << endl;
        }
      }
    }
  }

  cout << "Cov macro checks:" << endl;
  cout << "cov = "<< totCovCheck << ", spec =  " << totSpecCheck << endl;
  cout << "corrected ratio: " << totCovCheck/totSpecCheck << endl;
  // *************
  h_Angles->Write();
  // *********** *****
}


// write the content of the matrices
void readFiss::WriteCovEM()
{
  writeFile->cd();
  cd_FAME->cd();
  // ************************
  cout <<  "wirting covEM resutls to root"  << endl;

  TCanvas* c_CovExp =  new TCanvas("c_CovExp", "arrayCorr and arraySpec", 800, 1000);
  TCanvas* c_CovBack =  new TCanvas("c_CovBack", "arrayCorrBack and arraySpecBack", 800, 1000);
  c_CovExp->Divide(1,2);
  c_CovBack->Divide(1,2);
  c_CovBack->cd(1);
  c_CovExp->cd(1);

  h3_arrayCorrExp = new TH3D("h3_arrayCorrExp", "Experimental Corr Matrix; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
  h3_arraySpecExp = new TH3D("h3_arraySpecExp", "Experimental Spec Matrix; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
  h3_arrayCorrBack = new TH3D("h3_arrayCorrBack", "Background Corr Matrix; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
  h3_arraySpecBack = new TH3D("h3_arraySpecBack", "Background Spec Matrix; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);

  if (mode == BEAM_MODE)
  {
    h3_arrayCorrExpBeam = new TH3D*[BEAM_ERG_BINNUM];
    h3_arraySpecExpBeam = new TH3D*[BEAM_ERG_BINNUM];
    h3_arrayCorrBackBeam = new TH3D*[BEAM_ERG_BINNUM];
    h3_arraySpecBackBeam = new TH3D*[BEAM_ERG_BINNUM];

    for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
    {
      h3_arrayCorrExpBeam[eB] = new TH3D("h3_arrayCorrExpBeam", "Experimental Corr Matrix with Beam Energy MeV; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
      h3_arraySpecExpBeam[eB] = new TH3D("h3_arraySpecExpBeam", "Experimental Spec Matrix with Beam Energy MeV; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
      h3_arrayCorrBackBeam[eB] = new TH3D("h3_arrayCorrBackBeam", "Background Corr Matrix with Beam Energy MeV; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
      h3_arraySpecBackBeam[eB] = new TH3D("h3_arraySpecBackBeam", "Background Spec Matrix with Beam Energy MeV; Neutron Energy (MeV); Gamma Light Out (MeVee); Neutron-Gamma Angle", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG, BA, MIN_THETA, MAX_THETA);
    }
  }

  // Fill TH3Ds with respective matrixices and set proper histogram titles when drawing to canvas
  for (int ngAng = 0; ngAng < BA; ngAng++)
  {
    for (int bN = 0; bN < BN; bN++)
    {
      for (int bP = 0; bP < BP; bP++)
      {
        h3_arrayCorrExp->SetBinContent(bN+1,bP+1,ngAng+1,arrayCorrExp[ngAng][bN][bP]);  // neutrons = x-axis , photons = y-axis ; n-p angle = z-axis
        h3_arraySpecExp->SetBinContent(bN+1,bP+1,ngAng+1,arraySpecExp[ngAng][bN][bP]);
        h3_arrayCorrBack->SetBinContent(bN+1,bP+1,ngAng+1,arrayCorrBack[ngAng][bN][bP]);
        h3_arraySpecBack->SetBinContent(bN+1,bP+1,ngAng+1,arraySpecBack[ngAng][bN][bP]);
        if (mode == BEAM_MODE){
          for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
          {
            h3_arrayCorrExpBeam[eB]->SetBinContent(bN+1,bP+1,ngAng+1,arrayCorrExpBeam[eB][ngAng][bN][bP]);
            h3_arraySpecExpBeam[eB]->SetBinContent(bN+1,bP+1,ngAng+1,arraySpecExpBeam[eB][ngAng][bN][bP]);
            h3_arrayCorrBackBeam[eB]->SetBinContent(bN+1,bP+1,ngAng+1,arrayCorrBackBeam[eB][ngAng][bN][bP]);
            h3_arraySpecBackBeam[eB]->SetBinContent(bN+1,bP+1,ngAng+1,arraySpecBackBeam[eB][ngAng][bN][bP]);
          }
        }
      }
    }
  }
  h3_arrayCorrExp->Draw("LEGO2");
  h3_arrayCorrExp->SetTitle("Experiment Covariance Matrix");

  c_CovExp->cd(2);
  h3_arraySpecExp->Draw("LEGO2");
  h3_arraySpecExp->SetTitle("Experiment Spectrum");
  c_CovExp->Write();

  c_CovBack->cd(1);
  h3_arrayCorrBack->Draw("LEGO2");
  h3_arrayCorrBack->SetTitle("Background Covariance Matrix");

  c_CovBack->cd(2);
  h3_arraySpecBack->Draw("LEGO2");
  h3_arraySpecBack->SetTitle("Background Spectrum");
  c_CovBack->Write();


  if (mode == BEAM_MODE)
  {
    TCanvas** c_corrSpecBeam =  new TCanvas* [BEAM_ERG_BINNUM];
    TCanvas** c_corrSpecBeamBack =  new TCanvas* [BEAM_ERG_BINNUM];
    for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
  	{
      TString s_BEAM_Erg_Low = (TString)to_string(eB+2);
      TString s_BEAM_Erg_High = (TString)to_string(eB+3);

      // Experiment
      c_corrSpecBeam[eB] = new TCanvas("c_corrSpecBeam_" + s_BEAM_Erg_Low, "arrayCorr and arraySpec for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High, 800, 1000);
      c_corrSpecBeam[eB]->Divide(1,2);

      c_corrSpecBeam[eB]->cd(1);
      h3_arrayCorrExpBeam[eB]->Draw("LEGO2");

      c_corrSpecBeam[eB]->cd(2);
      h3_arraySpecExpBeam[eB]->Draw("LEGO2");
      h3_arrayCorrExpBeam[eB]->SetTitle("Experiment Covariance Matrix for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High + " MeV");
      h3_arraySpecExpBeam[eB]->SetTitle("Experiment Spectrum for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High + " MeV");

      c_corrSpecBeam[eB]->Write();

      // Background
      c_corrSpecBeamBack[eB] = new TCanvas("c_corrSpecBeamBack_" + s_BEAM_Erg_Low, "arrayCorrBack and arraySpecBack for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High, 800, 1000);
      c_corrSpecBeamBack[eB]->Divide(1,2);

      c_corrSpecBeamBack[eB]->cd(1);
      h3_arrayCorrBackBeam[eB]->Draw("LEGO2");
      h3_arrayCorrBackBeam[eB]->SetTitle("Background Covariance Matrix for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High + " MeV");

      c_corrSpecBeamBack[eB]->cd(2);
      h3_arraySpecBackBeam[eB]->Draw("LEGO2");
      h3_arraySpecBackBeam[eB]->SetTitle("Background Spectrum for BeamErg " + s_BEAM_Erg_Low + "-" + s_BEAM_Erg_High + " MeV");

      c_corrSpecBeamBack[eB]->Write();

    }
  }

  // ***********************
  cout << "Writing CovEM results to .csv" << endl;

  mkdir("covResults", 0777);
  cout << "creating" << endl;

  // print the content of the matrices
  ofstream covMatFire;
  ofstream specMatFire;
  string fileCovFire;
  string fileSpecFire;

  ofstream covMatFireBack;
  ofstream specMatFireBack;
  string fileCovFireBack;
  string fileSpecFireBack;

  for(int eA = 0; eA < BA; eA++)
  {
    fileCovFire = "covResults/covEM_" + to_string(eA) + ".csv";
    covMatFire.open(fileCovFire);

    fileSpecFire = "covResults/specEM_" + to_string(eA) + ".csv";
    specMatFire.open(fileSpecFire);

    fileCovFireBack = "covResults/covEMBack_" + to_string(eA) + ".csv";
    covMatFireBack.open(fileCovFireBack);

    fileSpecFireBack = "covResults/specEMBack_" + to_string(eA) + ".csv";
    specMatFireBack.open(fileSpecFireBack);

    // cout << eA << endl;

    for(int eN = 0; eN < BN; eN ++)
    {
     for(int eP = 0; eP < BP; eP ++)
      {
         covMatFire << arrayCorrExp[eA][eN][eP];
         specMatFire << arraySpecExp[eA][eN][eP];
         covMatFireBack << arrayCorrBack[eA][eN][eP];
         specMatFireBack << arraySpecBack[eA][eN][eP];

         if(eP < BP - 1)
         {
           covMatFire << ",";
           specMatFire << ",";
           covMatFireBack << ",";
           specMatFireBack << ",";
         }
         else if(eP == BP - 1)
         {
           covMatFire << "\n";
           specMatFire << "\n";
           covMatFireBack << "\n";
           specMatFireBack << "\n";
         }
      }
    }

    covMatFire.close();
    specMatFire.close();
    covMatFireBack.close();
    specMatFireBack.close();

  }

  // *********************
  if (mode == BEAM_MODE)
  {
    ofstream covMatBeamFire;
    ofstream specMatBeamFire;
    string fileCovBeamFire;
    string fileSpecBeamFire;

    ofstream covMatBeamFireBack;
    ofstream specMatBeamFireBack;
    string fileCovBeamFireBack;
    string fileSpecBeamFireBack;

    string beamFolderName;

    for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
    {
      beamFolderName = "covResultsBeam" + to_string(eB);
      mkdir(beamFolderName.c_str(), 0777);

      for(int eA = 0; eA < BA; eA++)
      {

      fileCovBeamFire = beamFolderName + "/covEMbeam_" + to_string(eA) + "_" + to_string(eB) + ".csv";
      covMatBeamFire.open(fileCovBeamFire);

      fileSpecBeamFire = beamFolderName + "/specEMbeam_" + to_string(eA) + "_" + to_string(eB) + ".csv";
      specMatBeamFire.open(fileSpecBeamFire);

      fileCovBeamFireBack = beamFolderName + "/covEMBackbeam_" + to_string(eA) + "_" + to_string(eB) + ".csv";
      covMatBeamFireBack.open(fileCovBeamFireBack);

      fileSpecBeamFireBack = beamFolderName + "/specEMBackbeam_" + to_string(eA) + "_" + to_string(eB) + ".csv";
      specMatBeamFireBack.open(fileSpecBeamFireBack);

        for(int eN = 0; eN < BN; eN ++)
        {
         for(int eP = 0; eP < BP; eP ++)
          {
             covMatBeamFire << arrayCorrExpBeam[eB][eA][eN][eP];
             specMatBeamFire << arraySpecExpBeam[eB][eA][eN][eP];
             covMatBeamFireBack << arrayCorrBackBeam[eB][eA][eN][eP];
             specMatBeamFireBack << arraySpecBackBeam[eB][eA][eN][eP];

             if(eP < BP - 1)
             {
               covMatBeamFire << ",";
               specMatBeamFire << ",";
               covMatBeamFireBack << ",";
               specMatBeamFireBack << ",";
             }
             else if(eP == BP - 1)
             {
               covMatBeamFire << "\n";
               specMatBeamFire << "\n";
               covMatBeamFireBack << "\n";
               specMatBeamFireBack << "\n";
             }
          }
        }
        covMatBeamFire.close();
        specMatBeamFire.close();
        covMatBeamFireBack.close();
        specMatBeamFireBack.close();
      }
    }
  }
  // *********************



}

void readFiss::AnalyseCovEM()
{

     //  //open file
     // ofstream covFire;
     // string fileFire = "covFire" + to_string(BN) + "_" + to_string(BP) +  ".csv";
     // covFire.open(fileFire);


     //   // compute the marginals
     //   double ergN[BN] = {0};
     //   double ergP[BP] = {0};

     //   double covNmarg[BN] = {0};
     //   double covPmarg[BP] = {0};

     //   double specN[BN] = {0};
     //   double specP[BP] = {0};


     //   // compute the marginals for neutrons
     //   for(int eN = 0; eN < BN; eN ++)
     //   {
     //    ergN[eN] = (eN + 0.5)*sizeNerg + minNerg;

     //    for(int eP = 0; eP < BP; eP ++)
     //     {
     //      covNmarg[eN] += h2_arrayCorr[eN][eP];
     //      specN[eN] += h2_arraySpec[eN][eP];
     //     }

     //     covFire << covNmarg[eN] << "," << specN[eN] << endl;
     //   }

     //   // compute the marginals for photons
     //   for(int eP = 0; eP < BP; eP++)
     //   {
     //    ergP[eP] = (eP + 0.5)*sizePerg + minPerg;

     //    for(int eN = 0; eN < BN; eN++)
     //     {
     //      covPmarg[eP] += h2_arrayCorr[eN][eP];
     //      specP[eP] += h2_arraySpec[eN][eP];
     //     }
     //     covFire << covPmarg[eP] << "," << specP[eP] << endl;
     //   }


     //   double normCovN[BN] = {0};
     //   double normCovP[BP] = {0};

     //   for(int eN = 0; eN < BN; eN ++)
     //   {
     //    normCovN[eN] = covNmarg[eN]/specN[eN];
     //   }

     //   for(int eP = 0; eP < BP; eP++)
     //   {
     //    normCovP[eP] = covPmarg[eP]/specP[eP];
     //   }

     //   double sumNeut = 0;
     //   double sumPhot = 0;

     //   for(int eN = 0; eN < BN; eN ++)
     //   {
     //    sumNeut += specN[eN];
     //   }

     //   for(int eP = 0; eP < BP; eP++)
     //   {
     //    sumPhot += specP[eP];
     //   }

     //   cout << sumNeut << " " << sumPhot << endl;


     //   // save as graphs
     //   TGraph* g_margN = new TGraph(BN, ergN, covNmarg);
     //   TGraph* g_margP = new TGraph(BP, ergP, covPmarg);
     //   TGraph* g_specN = new TGraph(BN, ergN, specN);
     //   TGraph* g_specP = new TGraph(BP, ergP, specP);
     //   TGraph* g_normN = new TGraph(BN, ergN, normCovN);
     //   TGraph* g_normP = new TGraph(BP, ergP, normCovP);

     //   g_margN->SetName("neutCov");
     //   g_margP->SetName("photCov");
     //   g_specN->SetName("neutSpec");
     //   g_specP->SetName("photSpec");
     //   g_normN->SetName("neutNorm");
     //   g_normP->SetName("photNorm");

     //   g_normN->SetTitle("Neutron dependent correlations; neutron energy (MeV); cov");
     //   g_normP->SetTitle("Photon dependent correlations; photon energy (MeV); cov");

     //   // write the graphs to file
     //   g_margN->Write();
     //   g_margP->Write();
     //   g_specN->Write();
     //   g_specP->Write();
     //   g_normN->Write();
     //   g_normP->Write();

     //   //h2_arrayCorr.Write();

     //   fileCorr->Write();
     //   fileCorr->Close();

       // covMatFire->Close();
       // covFire->Close();
}
