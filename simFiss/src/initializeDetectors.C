//
//

#include "SFAME.h"
#include "Constants.h"
#include "DetectorClass.h"

void sfame::initializeDetectors()
{
// array of x detectors (40 here)
        detectors = new DetectorClass [NUM_DETECTORS];
        // fill all of the properties
        // ? I think this needed to potentially go somewhere else eventually
        for(int i = 0; i < NUM_DETECTORS; i++)
        {
            // LO_function
            detectors[i].lightOutput = LightOutList[i];
            detectors[i].kinLight = KinLightList[i];
            detectors[i].lightOutput->SetName("LightOut_" + (TString)to_string(i));
            detectors[i].kinLight->SetName("KinematicLight_" + (TString)to_string(i));


            // distance
            detectors[i].x = posXList->Eval(i)*100;
            detectors[i].y = posYList->Eval(i)*100;
            detectors[i].z = posZList->Eval(i)*100;
            detectors[i].dist = sqrt(pow(detectors[i].x, 2) + pow(detectors[i].y, 2) + pow(detectors[i].z, 2));

            // time resolution
            detectors[i].timeRes = timeResList->Eval(i);

            // energy resolution

            // cell
            detectors[i].cell = chanToCell(i);

            // channel
            detectors[i].channel = i;

         }

}
