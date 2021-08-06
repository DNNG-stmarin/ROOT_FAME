//
//

#include "SFAME.h"
#include "Constants.h"
#include <sstream>

// functions in simParameters 

// converts from the cell value to the channel value
int sfame::cellToChannel(int cell)
{
<<<<<<< Updated upstream
    int channel = (int)(cellToChanList->Eval(cell));
    return channel; 
}
int sfame::chanToCell(int chan)
{
    int cell = (int)(chanToCellList->Eval(chan));
    return cell; 
=======
    int channel1 = ((cell - 1048)/100);
    int channel2 = channel1 + (channel1/15.5);
    return channel2;
>>>>>>> Stashed changes
}

// assigns a lightoutput value
double sfame::birks(int channel, double energy, int typeP, int zaid)
{
<<<<<<< Updated upstream
   /* if((energy < MIN_EXP) || (energy > MAX_EXP))
=======
    if((energy < MIN_EXP) || (energy > MAX_EXP))
>>>>>>> Stashed changes
    {
        lightout = 0.0;
    } 
    // neutrons 
    else if(typeP == NEUTRON & zaid == HYDROGEN) 
    {
        // TGraph eval at (energy) for a certain detector 
        lightout = lightoutFunctions[channel][energy]; // ? lightoutput functions is a weird mess
    }
    else if(typeP == NEUTRON & zaid == CARBON)
    {
        lightout = 0.02 * energy;
    }
    // photons   
    else if(typeP == tp.PHOTON)
    {
        lightout = energy;
    }  
    // edge cases 
    else
    {
        lightout = energy;
    }
    if(lightout < 0.0) 
    {
        lightout = 0.0;
    }    
    return lightout;
<<<<<<< Updated upstream
*/
=======

>>>>>>> Stashed changes
}

// broadens the energy 
double sfame::findBroad(double erg)
{
<<<<<<< Updated upstream
  /*  double broadening = (RESOLUTION_COEFFICIENTS[0] * erg + RESOLUTION_COEFFICIENTS[1] * sqrt(erg) + RESOLUTION_COEFFICIENTS[2]) / (2.35 * 100);
    return broadening;*/
=======
    double broadening = (RESOLUTION_COEFFICIENTS[0] * erg + RESOLUTION_COEFFICIENTS[1] * sqrt(erg) + RESOLUTION_COEFFICIENTS[2]) / (2.35 * 100);
    return broadening;
>>>>>>> Stashed changes
}

// V cuts 
// cuts using energy 
int sfame::cutLightOut(double height, int typeP)
{
<<<<<<< Updated upstream
   /*int newType = 0;
=======
   int newType = 0;
>>>>>>> Stashed changes
   // cuts out the particles that have energies outside of the boundry 
   if((height < ENERGYCUT) || (height > CLIPPED))
    {
       if(typeP == NEUTRON){
           newType = CUTNEUTRON;
       }    
       elif(typeP == PHOTON){
           newType = CUTPHOTON;
       }

    }
    // otherwise the type stays the same
    else
    {
       newType = typeP;
    }
<<<<<<< Updated upstream
    return newType;*/
=======
    return newType;
>>>>>>> Stashed changes

}

// cuts using time 
int sfame::cutTimePsd(double times, int typeP)
{
<<<<<<< Updated upstream
   /* int newType = 0;
=======
    int newType = 0;
>>>>>>> Stashed changes
    if((typeP == NEUTRON) & (times < MINTIMEN || times > MAXTIMEN))
    {
         newType = BADNEUTRON;
    }
      
    else if((typeP == PHOTON) & (times < MINTIMEP || times > MAXTIMEP))
    {
        newType = BADPHOTON;
    }
    else
    {
        newType = typeP;
    }
<<<<<<< Updated upstream
    return newType;*/
=======
    return newType;
>>>>>>> Stashed changes
}

/* non lightoutput functions

double sfame::findTn(double times, int channel):
        double v = ((DET_DIST[channel])/times);
        double Tn = 0.5*cn.MASS_NEUTRON* ( v/cn.C )**2;
        // cout << DETECTOR_DISTANCE[channel]) << endl; 
        return Tn;
*/


