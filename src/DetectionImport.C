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


}