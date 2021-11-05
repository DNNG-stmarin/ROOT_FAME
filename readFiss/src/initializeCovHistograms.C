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

  cout << "initializing covariance arrays" << endl;

  // neutron gamma correlations
  arrayExp  = new int***** [NUM_DETECTORS];
  arrayBack = new int***** [NUM_DETECTORS];

  for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
  {
    arrayExp[d1] = new int **** [NUM_DETECTORS];
    arrayBack[d1] = new int **** [NUM_DETECTORS];

    for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
    {
      arrayExp[d1][d2] = new int *** [BN];
      arrayBack[d1][d2] = new int *** [BN];
      for(int e1 = 0; e1 < BN; e1++)
      {
        arrayExp[d1][d2][e1] = new int ** [BP];
        arrayBack[d1][d2][e1] = new int ** [BP];
        for(int e2 = 0; e2 < BP; e2++)
        {
          arrayExp[d1][d2][e1][e2] = new int * [MAX_MULT_DET];
          arrayBack[d1][d2][e1][e2] = new int * [MAX_MULT_DET];
          for(int n1 = 0; n1 < MAX_MULT_DET; n1++)
          {
            arrayExp[d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
            arrayBack[d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
            for(int n2 = 0; n2 < MAX_MULT_DET; n2++)
            {
              arrayExp[d1][d2][e1][e2][n1][n2] = 0;
              arrayBack[d1][d2][e1][e2][n1][n2] = 0;
            }
          }
        }
      }

    }
  }


  // neutron-gamma-beam events
  if(mode == BEAM_MODE)
  {
    arrayExpBeam = new int****** [BEAM_ERG_BINNUM];
    arrayBackBeam = new int****** [BEAM_ERG_BINNUM];
    for(int b = 0; b < BEAM_ERG_BINNUM; b++)
    {
      arrayExpBeam[b] = new int***** [NUM_DETECTORS];
      arrayBackBeam[b] = new int***** [NUM_DETECTORS];

      for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
      {
        arrayExpBeam[b][d1] = new int **** [NUM_DETECTORS];
        arrayBackBeam[b][d1] = new int **** [NUM_DETECTORS];

        for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
        {
          arrayExpBeam[b][d1][d2] = new int *** [BN];
          arrayBackBeam[b][d1][d2] = new int *** [BN];
          for(int e1 = 0; e1 < BN; e1++)
          {
            arrayExpBeam[b][d1][d2][e1] = new int ** [BP];
            arrayBackBeam[b][d1][d2][e1] = new int ** [BP];
            for(int e2 = 0; e2 < BP; e2++)
            {
              arrayExpBeam[b][d1][d2][e1][e2] = new int * [MAX_MULT_DET];
              arrayBackBeam[b][d1][d2][e1][e2] = new int * [MAX_MULT_DET];
              for(int n1 = 0; n1 < MAX_MULT_DET; n1++)
              {
                arrayExpBeam[b][d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
                arrayBackBeam[b][d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
                for(int n2 = 0; n2 < MAX_MULT_DET; n2++)
                {
                  arrayExpBeam[b][d1][d2][e1][e2][n1][n2] = 0;
                  arrayBackBeam[b][d1][d2][e1][e2][n1][n2] = 0;
                }
              }
            }
          }
        }
      }
    }
  }



}
