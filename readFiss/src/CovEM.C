#include "readFiss.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMatrixD.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void readFiss::CovEM()
{

  cout << "Beginning the great CovEM loop" << endl;
   // place to store all the result

   arrayCorr = new TH2D("CovMat", "Covariance Matrix; Neutron ToF Energy (MeV); Photon Deposited Energy (MeVee); counts", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);
   arraySpec = new TH2D("SpecMat", "Spectrum Matrix; Neutron ToF Energy (MeV); Photon Deposited Energy (MeVee); counts", BN, MIN_N_ERG, MAX_N_ERG, BP, MIN_P_ERG, MAX_P_ERG);

   // make sure tree is open
   if (expTree == 0) return;

   // and sizes
   double sizeNerg = (MAX_N_ERG - MIN_N_ERG)/(double)BN;
   double sizePerg = (MAX_P_ERG - MIN_P_ERG)/(double)BP;

   // updated event-by-event history
   int* listNerg;
   int* listPerg;
   listNerg = new int[BN];
   listPerg = new int[BP];

   // initialize the EM histograms
   TString t_nameH;
   TString t_rootName = "em_";
   TString titleH = "EnergyMode Correlations";
   TH2I*** emMode;
   emMode = new TH2I**[BN];
   for(int eN = 0; eN < BN; eN++)
   {
   	emMode[eN] = new TH2I*[BP];

      for(int eP = 0; eP < BP; eP++)
      {
         t_nameH = t_rootName + to_string(eN) + " " + to_string(eP);
         emMode[eN][eP] = new TH2I(t_nameH, titleH, 10,0,10, 10,0,10);
      }
   }


   int encN, encP;
   // read the entries
   Long64_t nentries = expTree->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {

      if(jentry%1000000 == 0)
      {
        cout << jentry << " entries done. " << endl;
      }

   	  // load the new entry
      Long64_t ientry = LoadExpTree(jentry);
      if (ientry < 0) break;
      nb = expTree->GetEntry(jentry);   nbytes += nb;

     // reset the event-by-event list to 0
     for(int eN = 0; eN < BN; eN ++)
     {
      listNerg[eN] = 0;
     }
     for(int eP = 0; eP < BP; eP ++)
     {
      listPerg[eP] = 0;
     }


     // Neutron event-by-event spectrum
     for(int nN = 0; nN < neutronMult; nN++)
     {

       if(neutronLightOut[nN] > THRESHOLD && neutronDetTimes[nN] < MAX_TIME_N)
       {
         encN = int((neutronToFErg[nN] - MIN_N_ERG)/sizeNerg);

         if(encN > BN)
         {
           encN = BN;
         }
         else if(encN < 0)
         {
           encN = 0;
         }

         listNerg[encN] += 1;
       }

     }

     // Photon event-by-event spectrum
     for(int nP = 0; nP < gammaMult; nP++)
     {
       encP = int((photonLightOut[nP] - MIN_P_ERG)/sizePerg);

       if(encP > BP)
       {
         encP = BP;
       }
       else if(encP < 0)
       {
         encP = 0;
       }

       listPerg[encP] += 1;
     }


     // populate the em histograms
     for(int eN = 0; eN < BN; eN ++)
     {
      for(int eP = 0; eP < BP; eP ++)
       {
        emMode[eN][eP]->Fill(listNerg[eN], listPerg[eP]);
        // cout << "populated histogram at " << eN << " " << eP << endl;
       }
     }

    }

   // find the covariance at each point
     for(int eN = 0; eN < BN; eN ++)
     {
      for(int eP = 0; eP < BP; eP ++)
       {
        arrayCorr->SetBinContent(eN, eP, emMode[eN][eP]->GetCovariance());
        arraySpec->SetBinContent(eN, eP, (emMode[eN][eP]->GetMean(1))*(emMode[eN][eP]->GetMean(2)));
       }
     }

}

// write the content of the matrices
void readFiss::WriteCovEM()
{
  analysisFile->cd();
  cd_FAME->cd();

  // save the matrices to the root file
  arrayCorr->SetOption("COLZ");
  arraySpec->SetOption("COLZ");
  arrayCorr->Write();
  arraySpec->Write();

  // print the content of the matrices
  ofstream covMatFire;
  string fileMatFire = "covEM" + to_string(BN) + "_" + to_string(BP) + ".csv";
  covMatFire.open(fileMatFire);

  ofstream specMatFire;
  string fileSpecFire = "specEM" + to_string(BN) + "_" + to_string(BP) +  ".csv";
  specMatFire.open(fileSpecFire);

  // print the covariance at each point
  for(int eN = 0; eN < BN; eN ++)
  {
   for(int eP = 0; eP < BP; eP ++)
    {
       covMatFire << arrayCorr->GetBinContent(eN, eP);
       specMatFire << arraySpec->GetBinContent(eN, eP);

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


}

void readFiss::analyseCovEM()
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
     //      covNmarg[eN] += arrayCorr[eN][eP];
     //      specN[eN] += arraySpec[eN][eP];
     //     }

     //     covFire << covNmarg[eN] << "," << specN[eN] << endl;
     //   }

     //   // compute the marginals for photons
     //   for(int eP = 0; eP < BP; eP++)
     //   {
     //    ergP[eP] = (eP + 0.5)*sizePerg + minPerg;

     //    for(int eN = 0; eN < BN; eN++)
     //     {
     //      covPmarg[eP] += arrayCorr[eN][eP];
     //      specP[eP] += arraySpec[eN][eP];
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

     //   //arrayCorr.Write();

     //   fileCorr->Write();
     //   fileCorr->Close();

       // covMatFire->Close();
       // covFire->Close();
}
