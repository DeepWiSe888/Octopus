/*
 * Project Octopus
 *
 */



#include "radar_data.h"
#include "blas.h"
#include <math.h>
#include <stdlib.h>
#include "fpga_define.h"


float randData()
{
    return (1000+rand()%1000)/(1000*1000);
}
//create sim data
void* createSimData()
{
    srand(123);
    int fps = 20;
    int secs = 20;
    int rangebins = 138;
    //20*40*138
    matc *mc = createMat2C(fps*secs, rangebins);
    int i;
    for(i=0;i<20*20*138;i++)
    {
        mc->data[i].i = randData();
        mc->data[i].q = randData();
    }

    int vitalBin = 20;
    float freqRPM = 12/60.;
    float freqBPM = 70/60.;
    for(i=0;i<fps*secs;i++)
    {
        M2V(mc, i, vitalBin).i = 0.5*sin(freqRPM*i/fps);  //A*sin(wt);
        M2V(mc, i, vitalBin).q += 0.1*cos(freqBPM*i/fps);
    }
    return mc;
}


void* getRadarDataAddr(radar_info* radarInfo)
{
#ifdef _FPGA_RAWDATA
	return 0;
#endif
    return createSimData();
}
