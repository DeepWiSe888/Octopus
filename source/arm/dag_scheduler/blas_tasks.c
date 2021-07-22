/*
 * blas_tasks.c
 *
 *  Created on: Jul 1, 2021
 *      Author: link
 */


#include "blas_tasks.h"
#include <string.h>
#include <captain.h>
#include <math.h>
#include "blas.h"
#include "VMD.h"

#define _FPGA_FIR
#define _FPGA_FFT
//#define _FPGA_VMD


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

    // lots of codes for fir window steps
    memcpy(mOut->mat_describe, mIn->mat_describe, sizeof(mIn->mat_describe));
    memcpy(mOut->dims, mIn->dims, sizeof(mIn->dims));
    mOut->dim_cnt = mIn->dim_cnt;
    int matsize = 1;
    int i;
    for(i=0;i<mIn->dim_cnt;i++)
        matsize = matsize*mIn->dims[i];
    //just copy data
    memcpy(mOut->data, mIn->data, matsize*sizeof(complex));

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
    ti.output = createSameMat(input);//taskMemAlloc(getMatSize(input));
    ti.params = 0;
#endif

    return ti;
}


int FFT1D_useFPGA(task_info *ti)
{
	// do nothing.

	// fpgaread(flag);

	//wait for samphore...

	return 0;
}

int FFT1D(task_info *ti)
{
    matc* matIn = (matc*)ti->input;
    int N = matIn->dims[0];

    // just copy data.
    return FIR(ti);
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
	ti.input = 0;
	ti.output = taskMemAlloc((VITAL_WIN_LEN+VITAL_FFT_N)*sizeof(complex)); //2*windowlen*complex, for fft/fir wave
	ti.params = 0;
	ti.func_ptr = vitalVMD;
}


//TODO : fix here
#define fpga_read(addr)	1
#define fpga_write(addr,data)

int vitalVMD(task_info *ti)
{
	// wait for i/o interrupt
    if( xSemaphoreTake( semFFT, portTICK_RATE_MS*1000*10 ) != pdTRUE )
    {
	    return 0;
    }
	//fpga read
    uint16_t fmcDataAddr = 101;
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
    fpga_write(53,0)
	return 0;
}





// --- utils --- //
