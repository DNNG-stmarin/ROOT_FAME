//
//

#include <fstream>
#include <TGraph.h>
#include <string.h>

#include "Constants.h"

using namespace std;

#ifndef DetectorClass_h
#define DetectorClass_h

class DetectorClass 
{
public: 
    TGraph LO_function; 
    // double X, Y, Z; 
    double dist; 
    double timeRes; 
    double energyRes; 
    int channel; 
    int cell; 

<<<<<<< Updated upstream
};
=======
}
>>>>>>> Stashed changes

#endif

