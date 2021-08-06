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

            // distance

            // time resolution 

            // energy resolution 

            // cell
            detectors[i].cell = chanToCell(i);

            // channel
            detectors[i].channel = i;

         }
        
}