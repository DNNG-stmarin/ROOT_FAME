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
  cout << "  initializing 4d array" << endl;
  arrayExp  = new int****** [NUM_TRIGGERS];
  arrayBack = new int****** [NUM_TRIGGERS];
  for (int indexChannel = 0; indexChannel < NUM_TRIGGERS; indexChannel++)
  {
    cout << "   initializing trigger " << indexChannel << endl;
    arrayExp[indexChannel]  = new int***** [NUM_DETECTORS];
    arrayBack[indexChannel] = new int***** [NUM_DETECTORS];

    for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
    {
      arrayExp[indexChannel][d1] = new int **** [NUM_DETECTORS];
      arrayBack[indexChannel][d1] = new int **** [NUM_DETECTORS];

      for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
      {
        arrayExp[indexChannel][d1][d2] = new int *** [BN];
        arrayBack[indexChannel][d1][d2] = new int *** [BN];
        for(int e1 = 0; e1 < BN; e1++)
        {
          arrayExp[indexChannel][d1][d2][e1] = new int ** [BP];
          arrayBack[indexChannel][d1][d2][e1] = new int ** [BP];
          for(int e2 = 0; e2 < BP; e2++)
          {
            arrayExp[indexChannel][d1][d2][e1][e2] = new int * [MAX_MULT_DET];
            arrayBack[indexChannel][d1][d2][e1][e2] = new int * [MAX_MULT_DET];
            for(int n1 = 0; n1 < MAX_MULT_DET; n1++)
            {
              arrayExp[indexChannel][d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
              arrayBack[indexChannel][d1][d2][e1][e2][n1] = new int  [MAX_MULT_DET];
              for(int n2 = 0; n2 < MAX_MULT_DET; n2++)
              {
                arrayExp[indexChannel][d1][d2][e1][e2][n1][n2] = 0;
                arrayBack[indexChannel][d1][d2][e1][e2][n1][n2] = 0;
              }
            }
          }
        }
      }
    }
  }
  // ********************
  arrayExpErg = new int*** [BN];
  for (int e1 = 0; e1 < BN; e1++)
  {
    arrayExpErg[e1] = new int** [BP];
    for (int e2 = 0; e2 < BP; e2++)
    {
      arrayExpErg[e1][e2] = new int* [NUM_DETECTORS];
      for (int n1 = 0; n1 < NUM_DETECTORS; n1++)
      {
        arrayExpErg[e1][e2][n1] = new int [NUM_DETECTORS];
        for (int n2 = 0; n2 < NUM_DETECTORS; n2++)
        {
          arrayExpErg[e1][e2][n1][n2] = 0;
        }
      }
    }
  }
  // *******************


  // neutron-gamma-beam events
  cout << "  initializing 6d array" << endl;

  if(mode == BEAM_MODE)
  {
    arrayExpBeam = new int******* [NUM_TRIGGERS];
    arrayBackBeam = new int******* [NUM_TRIGGERS];
    // *************
    // arrayExpErg = new int*** [BN];
    // **************
    for (int indexChannel = 0; indexChannel < NUM_TRIGGERS; indexChannel++)
    {
      cout << "   initializing trigger " << indexChannel << endl;
      arrayExpBeam[indexChannel] = new int****** [BEAM_ERG_BINNUM];
      arrayBackBeam[indexChannel] = new int****** [BEAM_ERG_BINNUM];
      // *************
      // arrayExpErg[indexChannel] = new int**** [BEAM_ERG_BINNUM];
      // **************

      for(int b = 0; b < BEAM_ERG_BINNUM; b++)
      {
        arrayExpBeam[indexChannel][b] = new int***** [NUM_DETECTORS];
        arrayBackBeam[indexChannel][b] = new int***** [NUM_DETECTORS];
        // *************
        // arrayExpErg[indexChannel][b] = new int*** [BN];
        // **************

        for(int d1 = 0; d1 < NUM_DETECTORS; d1++)
        {
          arrayExpBeam[indexChannel][b][d1] = new int**** [NUM_DETECTORS];
          arrayBackBeam[indexChannel][b][d1] = new int**** [NUM_DETECTORS];

          for(int d2 = 0; d2 < NUM_DETECTORS; d2++)
          {
            arrayExpBeam[indexChannel][b][d1][d2] = new int*** [BN];
            arrayBackBeam[indexChannel][b][d1][d2] = new int*** [BN];

            for(int e1 = 0; e1 < BN; e1++)
            {
              arrayExpBeam[indexChannel][b][d1][d2][e1] = new int** [BP];
              arrayBackBeam[indexChannel][b][d1][d2][e1] = new int** [BP];
              // *************
              // arrayExpErg[e1] = new int** [BP];
              // arrayExpErg[indexChannel][b][e1] = new int** [BP];
              // **************

              for(int e2 = 0; e2 < BP; e2++)
              {
                arrayExpBeam[indexChannel][b][d1][d2][e1][e2] = new int* [MAX_MULT_DET];
                arrayBackBeam[indexChannel][b][d1][d2][e1][e2] = new int* [MAX_MULT_DET];
                // *************
                // arrayExpErg[e1][e2] = new int* [MAX_MULT_DET];
                // arrayExpErg[indexChannel][b][e1][e2] = new int* [MAX_MULT_DET];
                // **************

                for(int n1 = 0; n1 < MAX_MULT_DET; n1++)
                {
                  arrayExpBeam[indexChannel][b][d1][d2][e1][e2][n1] = new int [MAX_MULT_DET];
                  arrayBackBeam[indexChannel][b][d1][d2][e1][e2][n1] = new int [MAX_MULT_DET];
                  // *************
                  // arrayExpErg[e1][e2][n1] = new int [MAX_MULT_DET];
                  // arrayExpErg[indexChannel][b][e1][e2][n1] = new int [MAX_MULT_DET];
                  // **************

                  for(int n2 = 0; n2 < MAX_MULT_DET; n2++)
                  {
                    arrayExpBeam[indexChannel][b][d1][d2][e1][e2][n1][n2] = 0;
                    arrayBackBeam[indexChannel][b][d1][d2][e1][e2][n1][n2] = 0;
                    // *************
                    // arrayExpErg[e1][e2][n1][n2] = 0;
                    // arrayExpErg[indexChannel][b][e1][e2][n1][n2] = 0;
                    // **************
                  }
                }
              }
            }
          }
        }
      }
    }
  }




}
