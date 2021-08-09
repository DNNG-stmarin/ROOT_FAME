//
//

#include <fstream>
#include <TGraph.h>
#include <string.h>
#include <TF1.h>

#include "Constants.h"

using namespace std;

#ifndef DetectorClass_h
#define DetectorClass_h

class DetectorClass
{
public:
    TGraph* lightOutput;
    TGraph* kinLight;

    double x, y, z;
    double dist;

    double timeRes;

    TF1* energyRes;

    int channel;
    int cell;
};

#endif
