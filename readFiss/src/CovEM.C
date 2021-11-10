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
  cout << "extracting cov " << endl;

  arrayCorrExp = new double** [BA];
  arraySpecExp = new double** [BA];
  // *****************
  if (mode == BEAM_MODE)
  {
    arrayCorrExpBeam = new double*** [BEAM_ERG_BINNUM];
    arraySpecExpBeam = new double*** [BEAM_ERG_BINNUM];
    for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
    {
      arrayCorrExpBeam[bb] = new double** [BA];
      arraySpecExpBeam[bb] = new double** [BA];
    }
  }
  // *******************
  for(int ba = 0; ba < BA; ba++)
  {
    arrayCorrExp[ba] = new double* [BN];
    arraySpecExp[ba] = new double* [BN];

    // *******************
    if (mode == BEAM_MODE)
    {
      for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
      {
        arrayCorrExpBeam[bb][ba] = new double* [BN];
        arraySpecExpBeam[bb][ba] = new double* [BN];
      }
    }
    // *********************
    for(int bn = 0; bn < BN; bn++)
    {
      arrayCorrExp[ba][bn] = new double [BP];
      arraySpecExp[ba][bn] = new double [BP];

      // *********************
      if (mode == BEAM_MODE)
      {
        for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
        {
        arrayCorrExpBeam[bb][ba][bn] = new double [BP];
        arraySpecExpBeam[bb][ba][bn] = new double [BP];
        }
      }
      // *********************
      for(int bp = 0; bp < BP; bp++)
      {
        arrayCorrExp[ba][bn][bp] = 0.;
        arraySpecExp[ba][bn][bp] = 0.;

        // *********************
        if (mode == BEAM_MODE)
        {
          for (int bb = 0; bb < BEAM_ERG_BINNUM; bb++)
          {
          arrayCorrExpBeam[bb][ba][bn][bp] = 0.;
          arraySpecExpBeam[bb][ba][bn][bp] = 0.;
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
  int* multPosBeam;
  double* covValBeam;
  double* specValNBeam;
  double* specValPBeam;
  double* specValBeam;
  // int* listPerg;
  //    listNerg = new int[BN];
  multPosBeam = new int[BEAM_ERG_BINNUM];
  covValBeam = new double[BEAM_ERG_BINNUM];
  specValNBeam = new double[BEAM_ERG_BINNUM];
  specValPBeam = new double[BEAM_ERG_BINNUM];
  specValBeam = new double[BEAM_ERG_BINNUM];
  // **************************
  for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
  {
    for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
    {
      if(d1 == d2) continue;
      ngEnc = int ((angles[d1][d2] - MIN_THETA)/sizeNgAng);
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
              if (mode == BEAM_MODE)
              {
                for (int eB = 0; eB < BEAM_ERG_BINNUM; eB++)
                {
                  multPosBeam[eB] = arrayExpBeam[eB][d1][d2][e1][e2][n1][n2];
                  covValBeam[eB] += multPosBeam[eB]*n1*n2;
                  specValNBeam[eB] += multPosBeam[eB]*n1;
                  specValPBeam[eB] += multPosBeam[eB]*n2;
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
            }
          }
          // ******************

          // cout << arrayCorrExp[ngEnc][e1][e2] << endl;
        }
      }
    }
  }

  cout << "Cov macro checks:" << endl;
  cout << "cov = "<< totCovCheck << ", spec =  " << totSpecCheck << endl;
  cout << "corrected ratio: " << totCovCheck/totSpecCheck << endl;
}


// write the content of the matrices
void readFiss::WriteCovEM()
{
  writeFile->cd();
  cd_FAME->cd();
  // ************************
  cout <<  "wirting covEm resutls to root"  << endl;

  h3_arrayCorrExp = new TH3D("h3_arrayCorrExp", "Experimental Corr Matrix; Neutron-Gamma Angle; Neutron Energy; Gamma Light Out; Counts", BA, MIN_THETA, MAX_THETA, BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
  h3_arraySpecExp = new TH3D("h3_arraySpecExp", "Experimental Spec Matrix; Neutron-Gamma Angle; Neutron Energy; Gamma Light Out; Counts", BA, MIN_THETA, MAX_THETA, BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
  h3_arrayCorrExpBack = new TH3D("h3_arrayCorrExpBack", "Experimental Corr Matrix Background; Neutron-Gamma Angle; Neutron Energy; Gamma Light Out; Counts", BA, MIN_THETA, MAX_THETA, BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
  h3_arraySpecExpBack = new TH3D("h3_arraySpecExpBack", "Experimental Spec Matrix Background; Neutron-Gamma Angle; Neutron Energy; Gamma Light Out; Counts", BA, MIN_THETA, MAX_THETA, BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);

  // ***********************
  cout << "Writing CovEM results to .csv" << endl;

  mkdir("covResults", 0777);
  cout << "creating" << endl;

  // print the content of the matrices
  ofstream covMatFire;
  ofstream specMatFire;
  string fileCovFire;
  string fileSpecFire;

  for(int eA = 0; eA < BA; eA++)
  {
    fileCovFire = "covResults/covEM_" + to_string(eA) + ".csv";
    covMatFire.open(fileCovFire);

    fileSpecFire = "covResults/specEM_" + to_string(eA) + ".csv";
    specMatFire.open(fileSpecFire);

    // cout << eA << endl;

    for(int eN = 0; eN < BN; eN ++)
    {
     for(int eP = 0; eP < BP; eP ++)
      {
         covMatFire << arrayCorrExp[eA][eN][eP];
         specMatFire << arraySpecExp[eA][eN][eP];

         if(eP < BP - 1)
         {
           covMatFire << ",";
           specMatFire << ",";
         }
         else if(eP == BP - 1)
         {
           covMatFire << "\n";
           specMatFire << "\n";
         }
      }
    }

    covMatFire.close();
    specMatFire.close();

  }

  // *********************
  if (mode == BEAM_MODE)
  {
    ofstream covMatBeamFire;
    ofstream specMatBeamFire;
    string fileCovBeamFire;
    string fileSpecBeamFire;
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

        for(int eN = 0; eN < BN; eN ++)
        {
         for(int eP = 0; eP < BP; eP ++)
          {
             covMatBeamFire << arrayCorrExpBeam[eB][eA][eN][eP];
             specMatBeamFire << arraySpecExpBeam[eB][eA][eN][eP];

             if(eP < BP - 1)
             {
               covMatBeamFire << ",";
               specMatBeamFire << ",";
             }
             else if(eP == BP - 1)
             {
               covMatBeamFire << "\n";
               specMatBeamFire << "\n";
             }
          }
        }
        covMatBeamFire.close();
        specMatBeamFire.close();
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
