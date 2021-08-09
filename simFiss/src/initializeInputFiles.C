//
//

#include <iostream>
#include <fstream>
#include <sstream>


#include "SFAME.h"
#include "Constants.h"
#include "DetectorClass.h"

using namespace std;

void sfame::initializeInputFiles()
{
    const int MAX_DET_NUMBER = 1000;
    const int ENERGY_AXIS_LENGTH = 10000;

    // read the file and create a TGraph
    ifstream fileCell, filePos, fileLight, fileTimeRes;

    // detector axis
    double detNumArray[MAX_DET_NUMBER];

    // channel to cell
    double chanArray[MAX_DET_NUMBER];

    // positions
    double xPosArray[MAX_DET_NUMBER];
    double yPosArray[MAX_DET_NUMBER];
    double zPosArray[MAX_DET_NUMBER];

    // light output
    int numEnergyPoints = 0;
    double energyDepArray[ENERGY_AXIS_LENGTH];
    double** lightOutputArray;
    lightOutputArray = new double* [MAX_DET_NUMBER];
    LightOutList = new TGraph* [MAX_DET_NUMBER];
    KinLightList = new TGraph* [MAX_DET_NUMBER];
    for(int Dj = 0; Dj < MAX_DET_NUMBER; Dj++)
    {
      lightOutputArray[Dj] = new double [ENERGY_AXIS_LENGTH];
    }
    string line;
    double val;

    // time resolution values
    double timeResArray[MAX_DET_NUMBER];


    // open the files
    fileCell.open(CELL_NUMBERS_FILE);
    filePos.open(DET_DIST_FILE);
    fileLight.open(LIGHTOUTPUT_FILE);
    fileTimeRes.open(TIME_RESOLUTION_FILE);

    // start by reading the energy axis
    getline(fileLight, line);
    stringstream ss(line);
    bool readLineLight = true;
    int Ej = 0;
    while(readLineLight)
    {
      if(ss.peek() == ',') ss.ignore();
      ss >> val;

      if(val > 8.0)
      {
        readLineLight = false;
        break;
      }

      energyDepArray[Ej] = val;
      // cout << Ej << " " << energyDepArray[Ej] << endl;
      Ej++;
    }

    // loop through the files
    for(int i = 0; i < NUM_DETECTORS; i++)
    {
        detNumArray[i] = i;
        fileCell >> chanArray[i];
        filePos >> xPosArray[i] >> yPosArray[i] >> zPosArray[i];
        fileTimeRes >> timeResArray[i];

        getline(fileLight, line);
        stringstream ss(line);
        for(int jj = 0; jj < Ej; jj++)
        {
          if(ss.peek() == ',') ss.ignore();
          ss >> val;
          lightOutputArray[i][jj] = val;
        }
        LightOutList[i] = new TGraph(Ej, energyDepArray, lightOutputArray[i]);
        KinLightList[i] = new TGraph(Ej, lightOutputArray[i], energyDepArray);
    }

    // create the graphs
    chanToCellList = new TGraph(NUM_DETECTORS, detNumArray, chanArray);
    cellToChanList = new TGraph(NUM_DETECTORS, chanArray, detNumArray);
    posXList = new TGraph(NUM_DETECTORS, detNumArray, xPosArray);
    posYList = new TGraph(NUM_DETECTORS, detNumArray, yPosArray);
    posZList = new TGraph(NUM_DETECTORS, detNumArray, zPosArray);
    posZList = new TGraph(NUM_DETECTORS, detNumArray, zPosArray);
    timeResList = new TGraph(NUM_DETECTORS, detNumArray, timeResArray);

    // close the files
    fileCell.close();
    filePos.close();
    fileLight.close();
    fileTimeRes.close();

}
