#include "readFiss.h"
#include <TLegend.h>

#include <iostream>

using namespace std;

TFile* beamFile = new TFile("beamInfo.root", "READ");

TH2D* h_alphaSpecAll = (TH2D*)beamFile->Get("alphaChanSpec");
macroPop = (TH1I*)beamFile->Get("macroPop");

for (int i = 0; i < PPAC_NUM; i++)
{
  h_alphaSpec[i] = alphaSpecAll->ProjectionY("proj", TRIGGER_CHANNEL[i] + 1, TRIGGER_CHANNEL[i] + 1);
}
