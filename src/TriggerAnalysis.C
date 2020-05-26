/*
Trigger analysis:
Author: Stefano Marin
Purpose: Performs an analysis of the triggering system, and outputs the main statistics
*/

#define TriggerAnalysis_cxx

#include "DetectorSystemClass.h"

#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TCut.h>
#include <TFitResult.h>
#include <THStack.h>

#include <stdlib.h>
#include <stdio.h>
#include <queue>

#include "ParticleEvent.h"
#include "TriggerEvent.h"

int DetectorSystemClass::TriggerAnalysis()
{
