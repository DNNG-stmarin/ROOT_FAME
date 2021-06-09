/*
Author: Stefano Marin, Isabel Hernandez
Purpose: Perform the analyisis of the collected data on a detector-by-detector basis.
The result of this code will be a series of cuts to clean the data.

So far, we look at:
- PSD characterization
- Timing delays
*/

#define DetectionAnalysis_cxx

#include "DetectorSystemClass.h"

#include <TH3.h>
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <TProfile.h>
#include <TGraph.h>
#include <TLine.h>
#include <TFormula.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

#include "ParticleEvent.h"
#include "TriggerEvent.h"
#include "ProcessingConstants.h"

using namespace std;

int DetectorSystemClass::DetectionImport()
{
cout << "Detection Import Start" << endl;

// how do I set it up so I can test it, I can't run it seperate from everything to test 

// defines varibles for the file name of the canvases being accessed 
// PSD 
TString fileNameBasePsd= "PSD/PSDErg_discrimination/psdErg";
TString fileNamePsd;

// TOF
TString fileNameStartTOF= "TOF/TOF_individual/ToFDiscrimination";
TString fileNameEndTOF = "trig";
TString fileNameTOF;

// loops through all of the detectors
for(int det = 0; det < NUM_DETS; ++det){

    // discrmPSD
    // create a canvas to store the data being accessed 
    TCanvas* c_psdErg;
    TF1* f_psdDisc;
    // sets the file name to the base name and the detector 
    fileNamePsd =  fileNameBasePsd + (TString)to_string(det);
    //sets the canvas equal to the psdErg one for that detector 
    c_psdErg = (TCanvas*)detFile->Get(fileNamePsd);
    f_psdDisc = (TF1*)c_psdErg->GetPrimitive("psdDisc");
    detectors[det].discPSD = (TF1*)f_psdDisc->Clone();  
    

    // TOF
    for(int trig = 0; trig < NUM_TRIGGERS; ++trig){
        // create a canvas to store the data being accessed 
        TCanvas* c_TOF;
        TF1* f_TOF;
        // sets the file name to the base name and the detector 
        fileNameTOF =  fileNameStartTOF + (TString)to_string(det) + fileNameEndTOF + (TString)to_string(trig);
        //sets the canvas equal to the psdErg one for that detector 
        c_TOF = (TCanvas*)detFile->Get(fileNameTOF);
        f_TOF = (TF1*)c_TOF->GetPrimitive("peakFit"); // check this 
        //time delay 
        detectors[det].timeDelay[trig] = f_TOF->GetParameter(1) - detectors[det].distance/LIGHT_C; 
        //time resolution 
        detectors[det].timeResolution[trig] = f_TOF->GetParameter(2);
    }
}

return 1; 

}