//
//

#include <iostream>
#include <fstream>

#include "SFAME.h"
#include "Constants.h"
#include "DetectorClass.h"

using namespace std; 

void sfame::initializeInputFiles()
{
    // read the file and create a TGraph 
    ifstream fileIn;

    double detNumArray[1000]; 
    double chanArray[1000];

    fileIn.open(CELL_NUMBERS_FILE);

    for(int i = 0; i < NUM_DETECTORS; i++)
    {
        detNumArray[i] = i;
        fileIn >> chanArray[i];
    }

    chanToCellList = new TGraph(NUM_DETECTORS, detNumArray, chanArray);
    cellToChanList = new TGraph(NUM_DETECTORS, chanArray, detNumArray);

    fileIn.close();
}




