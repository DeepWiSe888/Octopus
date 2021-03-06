/*
 * Project Octopus
 *
 */



#include "radar_data.h"
#include "blas.h"
#include <math.h>
#include <stdlib.h>
#include "fpga_define.h"

#ifdef _X4DRIVER

#include "x4driver.h"
int radarInit()
{
	Radar();
	return 0;
}
#else

int radarInit()
{
    return 0;
}
#endif


float randData()
{
    return (1000.0+rand()%1000)/(1000.0*1000);
}
//create sim data
void* createSimData()
{
    srand(123);
    int fps = 40;
    int secs = 20;
    int rangebins = 138;
    //20*40*138
    matc *mc = createMat2C(fps*secs, rangebins);
    int i;
    for(i=0;i<40*20*138;i++)
    {
        mc->data[i].i = randData();
        mc->data[i].q = randData();
    }

    int vitalBin = 20;
    float freqRPM = 12/60.;
    float freqBPM = 70/60.;
    for(i=0;i<fps*secs;i++)
    {
        M2V(mc, i, vitalBin).i += 0.5*sin(2*3.14*freqRPM*i/fps);  //A*sin(wt);
        M2V(mc, i, vitalBin).i += 0.1*sin(2*3.14*freqBPM*i/fps);

        M2V(mc, i, vitalBin).q += 0.5*sin(2*3.14*freqRPM*i/fps);  //A*sin(wt);
        M2V(mc, i, vitalBin).q += 0.1*sin(2*3.14*freqBPM*i/fps);
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



