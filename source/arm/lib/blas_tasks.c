/*
 * Project Octopus
 *
 */



#include "blas_tasks.h"
#include <string.h>
#include <captain.h>
#include <math.h>
#include "blas.h"
#include "VMD.h"
#include "fpga_define.h"
#include "filter.h"


// FIR
typedef struct _filter_info{
    int steps;
    char mode[32];
    int lowStopHz;
    int highStopHz;
}filter_info;
int FIR(task_info *ti);
int FIR_useFPGA(task_info* ti);
task_info createTaskFIR(void* input, int steps, char* mode, int lowStopHz, int highStopHz)
{
    //build task
	task_info ti;
	bzero(&ti, sizeof(ti));
	strcpy(ti.func_name, "FIR");
#ifdef _FPGA_FIR
	ti.input = 0;
	ti.output = 0;
	ti.params = 0;
	ti.func_ptr = FIR_useFPGA;

#else
	ti.func_ptr = FIR;
	ti.input = input;
	ti.output = createSameMat(input);//taskMemAlloc(getMatSize(input));
	ti.params = taskMemAlloc(sizeof(filter_info));

	// task param <-- filter info
    filter_info* filterInfo = (filter_info*)ti.params;
    filterInfo->steps = steps;
    strncpy(filterInfo->mode, mode, 32);
    filterInfo->lowStopHz = lowStopHz;
    filterInfo->highStopHz = highStopHz;
#endif

	return ti;

}

int FIR_useFPGA(task_info *ti)
{
    // do nothing

	return 0;
}

int FIR(task_info *ti)
{
    //filter_info* filterInfo = (filter_info*)ti->params;
    matc * mIn = (matc*)ti->input;
    matc * mOut = (matc*)ti->output;

    filter_info* filterInfo = (filter_info*)ti->params;

    complex *x = &(M1V(mIn,0));
    complex *y = &(M1V(mOut,0));


    fir_bandpass(VITAL_FPS, FIR_N, filterInfo->lowStopHz , filterInfo->highStopHz , mIn->dims[0], x, y);
    printMat(mIn, "before_fir");
    printMat(mOut, "after_fir");
	return 0;
}




// FFT
int FFT1D(task_info *ti);
int FFT1D_useFPGA(task_info *ti);
task_info createTask1DFFT(void* input)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "FFT1D");
#ifdef _FPGA_FFT
	ti.input = 0;
	ti.output = 0;
	ti.params = 0;
	ti.func_ptr = FFT1D_useFPGA;
#else
    ti.func_ptr = FFT1D;
    ti.input = input;
    //ti.output = createSameMat(input);
    ti.output = createMat1C(VITAL_FFT_N);
    ti.params = 0;
#endif

    return ti;
}


int FFT1D_useFPGA(task_info *ti)
{
	// do nothing.

	return 0;
}

int FFT1D(task_info *ti)
{
    matc* mIn = (matc*)ti->input;
    matc* mOut = (matc*)ti->output;
    int N = mOut->dims[0];
    if(N!=VITAL_FFT_N)
    {
        //something wrong.
        return -1;
    }

    complex *x = &(M1V(mIn,0));
    complex *y = &(M1V(mOut,0));

    fftc2c(x, mIn->dims[0], y, N);
    y[0].i = 0; y[0].q=0;
    y[1].i = 0; y[1].q=0;
    y[2].i = 0; y[2].q=0;
    printMat(mIn, "before_fft");
    printMat(mOut, "after_fft");

	return 0;
}



// --- VMD --- //
int vitalVMD(task_info *ti);
task_info createTaskVMD(void* input)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "VMD");
	ti.input = input;
	ti.output = createMat2C(2, VITAL_WIN_LEN); // one rpm, one bpm
	ti.params = 0;
	ti.func_ptr = vitalVMD;

	return ti;
}


#ifdef _USE_FPGA
int vitalVMD(task_info *ti)
{
	// wait for i/o interrupt
    if( xSemaphoreTake( semFFT, portTICK_RATE_MS*1000*10 ) != pdTRUE )
    {
	    return 1;
    }
	//fpga read
    uint16_t fmcDataAddr = FMC_ADDR_DATA;
    int antID = fpga_read(fmcDataAddr++);
    int frameNo = fpga_read(fmcDataAddr++);
    complex* firdata = (complex*)ti->output;
    complex* fftdata = (complex*)(firdata+VITAL_WIN_LEN);
    float srcAbs[VITAL_WIN_LEN];
    int i,inx=0;
    float f=0;
    //read fir data
    for(i=0;i<VITAL_WIN_LEN;i++)
    {
    	*((uint32_t*)&f)=fpga_read(fmcDataAddr++);
    	*(((uint32_t*)&f)+1)=fpga_read(fmcDataAddr++);
    	firdata[inx].i = f;
    	srcAbs[inx] = f*f;

    	*((uint32_t*)&f)=fpga_read(fmcDataAddr++);
    	*(((uint32_t*)&f)+1)=fpga_read(fmcDataAddr++);
    	firdata[inx].q = f;
    	srcAbs[inx] += f*f;

    	srcAbs[inx] =sqrt(srcAbs[inx]);

    	inx++;
    }
    //read fft data
    inx = 0;
    for(i=0;i<VITAL_FFT_N;i++)
    {
    	*((uint32_t*)&f)=fpga_read(fmcDataAddr++);
    	*(((uint32_t*)&f)+1)=fpga_read(fmcDataAddr++);
    	fftdata[inx].i = f;

    	*((uint32_t*)&f)=fpga_read(fmcDataAddr++);
    	*(((uint32_t*)&f)+1)=fpga_read(fmcDataAddr++);
    	fftdata[inx].q = f;
    	inx++;
    }

    //do VMD
    float rpmWave[VITAL_WIN_LEN];
    float bpmWave[VITAL_WIN_LEN];
    VMD_vital(srcAbs, VITAL_WIN_LEN, rpmWave, bpmWave);

    //rpm/bpm wave plot or print usart

	// clear flag
    fpga_write(FMC_ADDR_RADAR_DATA_FLAG,0);
	return 0;
}
#else
int vitalVMD(task_info *ti)
{
    float srcAbs[VITAL_WIN_LEN];
    float rpmWave[VITAL_WIN_LEN];
    float bpmWave[VITAL_WIN_LEN];

    matc* mIn = (matc*)ti->input;
    matc* mOut = (matc*)ti->output;
    int i;
    for(i=0;i<VITAL_WIN_LEN;i++)
        srcAbs[i] = complex_abs(&M1V(mIn, i));

    VMD_vital(srcAbs, VITAL_WIN_LEN, rpmWave, bpmWave);

    for(i=0;i<VITAL_WIN_LEN;i++)
    {
        M2V(mOut, 0, i).i = rpmWave[i];
        M2V(mOut, 0, i).q = 0;
        M2V(mOut, 1, i).i = bpmWave[i];
        M2V(mOut, 1, i).q = 0;
    }
    return 0;
}
#endif


// --- CNN People Counting --- //
int occupancyCNN_useFPGA(task_info *ti);
task_info createTaskCNN_PeopleCounting(void* input)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "CNN");
#ifdef _FPGA_FFT
	ti.input = 0;
	ti.output = 0;
	ti.params = 0;
	ti.func_ptr = occupancyCNN_useFPGA;
#else
    ti.func_ptr = 0;
    ti.input = 0;
    ti.output = 0;
    ti.params = 0;
#endif

    return ti;

}


int occupancyCNN_useFPGA(task_info *ti)
{
	// do nothing.


	return 0;
}



// --- utils --- //
