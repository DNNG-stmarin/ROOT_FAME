#define visualizeRun_cxx
#include "visualizeRun.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void visualizeRun::PlotWavePlots()
{
   const int RECORD_LENGTH = 1280;
   const int NUM_WAVES = 1000;
   const double timeSampleConversion = 2.00; 

   const int BEAM_CHAN = 0;
   const int CATHODE_CHAN = 6;
   const int ANODE1_CHAN = 2;
   const int ANODE2_CHAN = 3;
   const int GRID1_CHAN = 4;
   const int GRID2_CHAN = 5;
   const int UNKN = 7;

   int numBeam = 0;
   int numCathode = 0;
   int numAnode1 = 0;
   int numAnode2 = 0;
   int numGrid1 = 0;
   int numGrid2 = 0;
   int numUnk = 0;

   double timeAxis[RECORD_LENGTH] = {0};

   double** beamSignal;
   double** cathodeSignal;
   double** anode1Signal;
   double** anode2Signal;
   double** grid1Signal;
   double** grid2Signal;
   double** unkSignal;

   beamSignal = new double* [NUM_WAVES];
   cathodeSignal = new double* [NUM_WAVES];
   anode1Signal = new double* [NUM_WAVES];
   anode2Signal = new double* [NUM_WAVES];
   grid1Signal = new double* [NUM_WAVES];
   grid2Signal = new double* [NUM_WAVES];
   unkSignal = new double* [NUM_WAVES];


   for(int num = 0; num < NUM_WAVES; num++)
   {
	   beamSignal[num] = new double [RECORD_LENGTH];
	   cathodeSignal[num] = new double [RECORD_LENGTH];
	   anode1Signal[num] = new double [RECORD_LENGTH];
	   anode2Signal[num] = new double [RECORD_LENGTH];
	   grid1Signal[num] = new double [RECORD_LENGTH];
	   grid2Signal[num] = new double [RECORD_LENGTH];
	   unkSignal[num] = new double [RECORD_LENGTH];
   }

   cout << "initialized arrays" << endl;

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry < NUM_WAVES; jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // cout << (double)chnum << endl;

	  for(int tS = 0; tS < RECORD_LENGTH; tS++)
	  {
	  	timeAxis[tS] = tS*timeSampleConversion;
	  }

	  int chan = (int)chnum;

	  
	  	if(chan == BEAM_CHAN)
	  	{
		    //cout << (int)chnum <<  " beam" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		beamSignal[numBeam][tS] = wf[tS];
		  	}
		  	numBeam++;
	  	}

	  	else if(chan == CATHODE_CHAN)
	  	{
		   // cout << (int)chnum <<  " cathode" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		cathodeSignal[numCathode][tS] = wf[tS];
		  	}
		  	numCathode++;
		}

	  	else if(chan == ANODE1_CHAN)
		{
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{	
		  		anode1Signal[numAnode1][tS] = wf[tS];
		  	}
		  	numAnode1++;
	  	}

	  	else if(chan == ANODE2_CHAN)
		{
			// cout << (int)chnum <<  " cathode2" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		anode2Signal[numAnode2][tS] = wf[tS];
		  	}
		  	numAnode2++;
		}
	  	

	  	else if(chan == GRID1_CHAN)
		{
		    // cout << (int)chnum <<  " grid1" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		grid1Signal[numGrid1][tS] = wf[tS];
		  	}
		  	numGrid1++;
		}

	  	else if(chan == GRID2_CHAN)
		{
		  	// cout << (int)chnum <<  " grid2" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		grid2Signal[numGrid2][tS] = wf[tS];
		  	}
		  	numGrid2++;
	    }

	  	else if(chan == UNKN)
		{
			cout << "unknown channel encountered" << endl;
		  	// cout << (int)chnum <<  " unknown" << endl;
		  	for(int tS = 0; tS < RECORD_LENGTH; tS++)
		  	{
		  		unkSignal[numUnk][tS] = wf[tS];
		  	}
		  	numUnk++;
		}
      

   }

   TCanvas* c_waves = new TCanvas("c_waves", "Signals IC");
   c_waves->Divide(3,2);


   c_waves->cd(1);
   for(int nB = 0; nB < numBeam; nB++)
   {
	   	 TGraph* g_beam = new TGraph(RECORD_LENGTH, timeAxis, beamSignal[nB]);
	   	 g_beam->SetName("g_beam");
	     g_beam->SetTitle("Beam");
	     g_beam->GetYaxis()->SetRangeUser(0,pow(2,14));
	     if(nB == 0) g_beam->Draw(); 
	     else g_beam->Draw("SAME");
   }
  

   c_waves->cd(2);
   // cout << numCathode << "cathode channels received" << endl;
   for(int nB = 0; nB < numCathode; nB++)
   {
		TGraph* g_cathode = new TGraph(RECORD_LENGTH, timeAxis, cathodeSignal[nB]);
		g_cathode->SetName("g_cathode");
		g_cathode->SetTitle("Cathode");
		g_cathode->GetYaxis()->SetRangeUser(0,pow(2,14));
		if(nB == 0) g_cathode->Draw();
		else g_cathode->Draw("SAME");

    }

   c_waves->cd(3);
   for(int nB = 0; nB < numAnode1; nB++)
   {
	   TGraph* g_an1 = new TGraph(RECORD_LENGTH, timeAxis, anode1Signal[nB]);
	   g_an1->SetName("g_an1");
	   g_an1->SetTitle("Anode1");
	   g_an1->GetYaxis()->SetRangeUser(0,pow(2,14));
	   if(nB == 0) g_an1->Draw();
	   else g_an1->Draw("SAME");
   }

   c_waves->cd(4);
   for(int nB = 0; nB < numAnode2; nB++)
   {
	   TGraph* g_an2 = new TGraph(RECORD_LENGTH, timeAxis, anode2Signal[nB]);
	   g_an2->SetName("g_an2");
	   g_an2->SetTitle("Anode2");
	   g_an2->GetYaxis()->SetRangeUser(0,pow(2,14));
	   if(nB == 0) g_an2->Draw();
	   else g_an2->Draw("SAME");
	}

   c_waves->cd(5);
   for(int nB = 0; nB < numGrid1; nB++)
   {
	   TGraph* g_grid1 = new TGraph(RECORD_LENGTH, timeAxis, grid1Signal[nB]);
	   g_grid1->SetName("g_grid1");
	   g_grid1->SetTitle("Grid1");
	   g_grid1->GetYaxis()->SetRangeUser(0,pow(2,14));
	   if(nB == 0) g_grid1->Draw();
	   else g_grid1->Draw("SAME");
	}

   c_waves->cd(6);
   for(int nB = 0; nB < numGrid2; nB++)
   {
	   TGraph* g_grid2 = new TGraph(RECORD_LENGTH, timeAxis, grid2Signal[nB]);
	   g_grid2->SetName("g_grid2");
	   g_grid2->SetTitle("Grid2");
	   g_grid2->GetYaxis()->SetRangeUser(0,pow(2,14));
	   if(nB == 0) g_grid2->Draw();
	   else g_grid2->Draw("SAME");
	}

}
