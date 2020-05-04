/*
Author: Stefano Marin, Isabel Hernandez
Purpose: This code loops through the processed tree and using the applied cuts and corrections computes the histograms of interest
*/

#define SystemAnalysis_cxx

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <stdlib.h>
#include <stdio.h>

#include "SystemAnalysis.h"

#include "InfoSystem.h"
#include "mappingFunctions.h"

using namespace std;

void SystemAnalysis::Loop()
{

/*
   _____      _               
  / ____|    | |              
 | (___   ___| |_ _   _ _ __  
  \___ \ / _ \ __| | | | '_ \ 
  ____) |  __/ |_| |_| | |_) |
 |_____/ \___|\__|\__,_| .__/ 
                       | |    
                       |_|    
*/

   //    // openFile and create tree
   // TString fileName = "FissionOutput.root";
   // TTree* tree;
   // TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName);
   //  if (!file || !file->IsOpen()) 
   //  {
   //      file = new TFile(fileName, "UPDATE");
   //  }
   // file->GetObject("FissionTree", tree);

   // important axes to have
   TString timeAxis = "time (ns)";
   TString energyAxis = "energy (MeVee)";
   TString countAxis = "counts";

/*
  __  __      _ _   _      _ _    _ _        
 |  \/  |_  _| | |_(_)_ __| (_)__(_) |_ _  _ 
 | |\/| | || | |  _| | '_ \ | / _| |  _| || |
 |_|  |_|\_,_|_|\__|_| .__/_|_\__|_|\__|\_, |
                     |_|                |__/ 
*/

   // file output and directory list
   //TFile* f = new TFile("multHist.root", "RECREATE");
   sysFile->cd();

   TDirectory* cdMult = sysFile->mkdir("Multiplicity");
   TDirectory* cdCoinc = sysFile->mkdir("Coincidences");
   TDirectory* cdFigCoinc = sysFile->mkdir("CoincFigs");
   TDirectory* cdBicorr = sysFile->mkdir("Bicorr");
   TDirectory* cdRef = sysFile->mkdir("Reflections");

   // create a two dimensional multiplicity distribution
   TH2I* hMult = new TH2I("mult", "mult", 10, 0, 10, 10, 0, 10);

/*
  ___ _                    _      _   _          
 | _ |_)__ ___ _ _ _ _ ___| |__ _| |_(_)___ _ _  
 | _ \ / _/ _ \ '_| '_/ -_) / _` |  _| / _ \ ' \ 
 |___/_\__\___/_| |_| \___|_\__,_|\__|_\___/_||_|
                                                 
*/

   // create a two bicorrelation distribution
   TH2F* hBicorr[NUM_DETS][NUM_DETS];
   TString bicorrT = "bicorr_det_";
   TString bicorrHistName, bicorrHistTitle;

   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         bicorrHistName = bicorrT + to_string(det1) + "_" + to_string(det2);
         bicorrHistTitle = bicorrHistName + ";" + timeAxis + ";" + timeAxis;

         hBicorr[det1][det2] = new TH2F(bicorrHistName, bicorrHistTitle, 100, -10, 100, 100, -10, 100);
         // hBicorr[det1][det2]->SetOption("COLZ");
      }
   }

   // create histograms of doubles and singles
   TH1I* hSingles = new TH1I("singles", "singles; detector; counts", NUM_DETS + 1, 0, NUM_DETS);
   TH2I* hDoubles = new TH2I("doubles", "doubles; detector; detector", NUM_DETS + 1, 0, NUM_DETS, NUM_DETS + 1, 0, NUM_DETS);
/*
         _         _    _                    
  __ ___(_)_ _  __(_)__| |___ _ _  __ ___ ___
 / _/ _ \ | ' \/ _| / _` / -_) ' \/ _/ -_|_-<
 \__\___/_|_||_\__|_\__,_\___|_||_\__\___/__/
                                             
*/                                                    

   // create array of histograms
   TH1F* nnMult[NUM_DETS][NUM_DETS];
   TH1F* ggMult[NUM_DETS][NUM_DETS];
   TH1F* gnMult[NUM_DETS][NUM_DETS];
   TH1F* ngMult[NUM_DETS][NUM_DETS];
   THStack* allCoinc[NUM_DETS][NUM_DETS];

   // create titles
   TString nnCoincT = "nn_det_";
   TString ngCoincT = "ng_det_";
   TString gnCoincT = "gn_det_";
   TString ggCoincT = "gg_det_";
   TString allCoincT = "all_det_";

   TString nnHistName, ngHistName, gnHistName, ggHistName, allCoincName;
   TString nnHistTitle, ngHistTitle, gnHistTitle, ggHistTitle, allCoincTitle;

   // populate histograms with pointers
   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         allCoincName = allCoincT + to_string(det1) + "_" + to_string(det2);
         allCoincTitle = allCoincName + ";" + timeAxis + ";" + countAxis;
         allCoinc[det1][det2] = new THStack(allCoincName, "");

         nnHistName = nnCoincT + to_string(det1) + "_" + to_string(det2);
         nnHistTitle = nnHistName + ";" + timeAxis + ";" + countAxis;
         nnMult[det1][det2] = new TH1F(nnHistName, nnHistTitle, 100, -50, 50);
         nnMult[det1][det2]->SetLineColor(kBlue);

         allCoinc[det1][det2]->Add(nnMult[det1][det2]);

         ngHistName = ngCoincT + to_string(det1) + "_" + to_string(det2);
         ngHistTitle = ngHistName + ";" + timeAxis + ";" + countAxis;
         ngMult[det1][det2] = new TH1F(ngHistName, ngHistTitle, 100, -50, 50);
         ngMult[det1][det2]->SetLineColor(kOrange);

         allCoinc[det1][det2]->Add(ngMult[det1][det2]);

         gnHistName = gnCoincT + to_string(det1) + "_" + to_string(det2);
         gnHistTitle = gnHistName + ";" + timeAxis + ";" + countAxis;
         gnMult[det1][det2] = new TH1F(gnHistName, gnHistTitle, 100, -50, 50);
         gnMult[det1][det2]->SetLineColor(kOrange);

         allCoinc[det1][det2]->Add(gnMult[det1][det2]);

         ggHistName = ggCoincT + to_string(det1) + "_" + to_string(det2);
         ggHistTitle = ggHistName + ";" + timeAxis + ";" + countAxis;
         ggMult[det1][det2] = new TH1F(ggHistName, ggHistTitle, 100, -50, 50);
         ggMult[det1][det2]->SetLineColor(kRed);

         allCoinc[det1][det2]->Add(ggMult[det1][det2]);
         allCoinc[det1][det2]->SetDrawOption("nostack");

      }
   }



 //  ___      __ _        _   _             
 // | _ \___ / _| |___ __| |_(_)___ _ _  ___
 // |   / -_)  _| / -_) _|  _| / _ \ ' \(_-<
 // |_|_\___|_| |_\___\__|\__|_\___/_||_/__/
                                         
     TH2F* reflections[NUM_DETS][NUM_DETS]; 

     TString refT = "ref_det_";
     TString refHistName, refHistTitle;

     for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         refHistName = refT + to_string(det1) + "_" + to_string(det2);
         refHistTitle = refHistName + ";" + timeAxis + ";" + energyAxis + ";" + countAxis;
         reflections[det1][det2] = new TH2F(refHistName, refHistTitle, 100, -50, 50, 10000, 0, 10);
      }
   }




   //cout << "Now starting main loop. " << endl;

   /*
  __  __      _        _                  
 |  \/  |__ _(_)_ _   | |   ___  ___ _ __ 
 | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
 |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
                                    |_|   
   */
   
   int nMult, pMult;

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   //cout << nentries << endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {

      // load tree
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;


      // fill multiplicity histograms
      nMult = 0;
      pMult = 0;

   	for(int j = 0; j < tMult; j++)
   	{
   		if(totPSP[j] > 0.15)
         {
            nMult++;
         }
         else if(totPSP[j] < 0.15)
         {
            pMult++;
         }
   	}
      hMult->Fill(nMult, pMult);



      // fill coincidence histograms
      for(int j = 0; j < tMult; j++)
      {
         hSingles->Fill(isDetector(totChan[j]));

         for(int k = 0; k < tMult; k++)
         {


            // find the reflections
            reflections[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[k] - totToF[j], totDep[k]);

            // find the doubles
            if(j != k)
            {
               hDoubles->Fill(isDetector(totChan[j]), isDetector(totChan[k]));
            }


            // compute the coincidences
            if((totDep[j] > DETECTOR_THRESHOLD) & (totDep[k] > DETECTOR_THRESHOLD))
            {
               hBicorr[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[j], totToF[k]);


               if((totPSP[j] > 0.15) & (totPSP[k] > 0.15)) // neutron - neutron
               {
                  nnMult[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[j] - totToF[k]);
               }

               if((totPSP[j] > 0.15) & (totPSP[k] < 0.15)) // neutron - photon
               {
                  ngMult[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[j] - totToF[k]);
               }

               if((totPSP[j] < 0.15) & (totPSP[k] > 0.15)) // photon - neutron
               {
                  gnMult[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[j] - totToF[k]);
               }

               if((totPSP[j] < 0.15) & (totPSP[k] < 0.15)) // photon - photon
               {
                  ggMult[isDetector(totChan[j])][isDetector(totChan[k])]->Fill(totToF[j] - totToF[k]);
               } 
            }
            
         }
      }
   }


   //cout << "Saving data to file. " << endl;

   // don't plot on the display
   gROOT->SetBatch(kTRUE);

   //cout << "Saving Multiplicities. " << endl;

   // multiplicity 
   cdMult->cd();
   TCanvas* canvMult = new TCanvas("multC", "multC", 800, 500);
   canvMult->cd();
   hMult->Draw("COLZ");
   canvMult->Write();
   hMult->Write();

   // hBicorr->Draw("COLZ");
   // hBicorr->Write();

   //cout << "Saving Coincidences. " << endl;

   // coincidences saving
   cdCoinc->cd();
   TCanvas* canvCoinc = new TCanvas("coincC", "coincC", 800, 500);
   canvCoinc->Divide(NUM_DETS, NUM_DETS);

   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         // select position on canvas
         canvCoinc->cd(det1*NUM_DETS + det2 + 1);

         // draw histograms
         allCoinc[det1][det2]->Draw("nostack");


         nnMult[det1][det2]->Write();
         ngMult[det1][det2]->Write();
         gnMult[det1][det2]->Write();
         ggMult[det1][det2]->Write();

         allCoinc[det1][det2]->Write();  

      }
   }

   canvCoinc->Write();

   //cout << "Saving Coincidences Fig. " << endl;

   // Now save the figures related to coincidences
   cdFigCoinc->cd();
   TCanvas* canvEach[NUM_DETS][NUM_DETS];
   TString titlePlot, namePlot;
   TString nameCoincT = "coincidence_";

   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         namePlot = nameCoincT + to_string(det1) + "_" + to_string(det2);
         titlePlot = namePlot;

         canvEach[det1][det2] = new TCanvas(namePlot, titlePlot, 800, 500); 

         canvEach[det1][det2]->cd();
         allCoinc[det1][det2]->Draw("nostack");

         // legend
         TLegend* legendTT = new TLegend(0.1,0.7,0.48,0.9);
         legendTT->AddEntry(nnMult[det1][det2], "NN", "l");
         legendTT->AddEntry(ngMult[det1][det2], "NG", "l");
         legendTT->AddEntry(gnMult[det1][det2], "GN", "l");
         legendTT->AddEntry(ggMult[det1][det2], "GG", "l");
         legendTT->Draw();

         // save the figures
         canvEach[det1][det2]->Write();
      }
   }


   //cout << "Saving Bicorrelations. " << endl;
   cdBicorr->cd();
   TCanvas* canvBicorr[NUM_DETS][NUM_DETS];
   TString nameBicorrT = "bicorr_";

   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         namePlot = nameBicorrT + to_string(det1) + "_" + to_string(det2);
         titlePlot = namePlot;

         canvBicorr[det1][det2] = new TCanvas(namePlot, titlePlot, 800, 500); 

         canvBicorr[det1][det2]->cd();
         hBicorr[det1][det2]->Draw();

         // save the figures
         canvBicorr[det1][det2]->Write();

         hBicorr[det1][det2]->Write();
      }
   }

   hSingles->Write();
   hDoubles->Write();


   // saving reflection plots
   cdRef->cd();

   for(int det1 = 0; det1 < NUM_DETS; det1++)
   {
      for(int det2 = 0; det2 < NUM_DETS; det2++)
      {
         reflections[det1][det2]->Write();
      }
   }

   sysFile->Close();

   // and reopen the display
   gROOT->SetBatch(kTRUE);
}
