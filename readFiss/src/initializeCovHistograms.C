#include "readFiss.h"
#include "Parameters.h"
using namespace std;
void readFiss::InitializeCovHistograms()
{

  iN2 = 1;
  iN1 = iN2*MAX_MULT_DET;
  iE2 = iN1*MAX_MULT_DET;
  iE1 = iE2*BP;
  iD2 = iE1*BN;
  iD1 = iD2*NUM_DETECTORS;
  iTot = iD1*NUM_DETECTORS;

  arrayExp = new int [iTot];
  for(int i = 0; i < iTot; i++) arrayExp[i] = 0;

  arrayBack = new int [iTot];
  for(int i = 0; i < iTot; i++) arrayBack[i] = 0;




}
