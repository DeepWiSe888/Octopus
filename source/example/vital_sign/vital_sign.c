/*
 * Project Octopus
 *
 */


#include "vital_sign.h"
#include <string.h>
#include <stdio.h>
#include "captain.h"
#include "blas_tasks.h"
#include "radar_data.h"
#include "blas.h"

int getVital(task_info * ti)
{
    matc *mOut = (matc*)ti->output;
    float rpm = 0;
    float bpm = 0;

    complex* firdata = (complex*)ti->output;
    complex* fftdata = (complex*)(firdata+VITAL_WIN_LEN);

    //freq = p*fps/fftn
    int beginFreqRpm = floor((8.0/60)*VITAL_FFT_N/VITAL_FPS);
    int endFreqRpm = ceil((8.0/60)*VITAL_FFT_N/VITAL_FPS);
    int beginFreqBpm = floor((60.0/60)*VITAL_FFT_N/VITAL_FPS);
    int endFreqBpm = ceil((120.0/60)*VITAL_FFT_N/VITAL_FPS);
    int iMax = 0;
    int i;
    float maxVal = 0;
    for(i=beginFreqRpm;i<=endFreqRpm;i++)
    {
    	float tmpVal = fftdata[i].i*fftdata[i].i + fftdata[i].q*fftdata[i].q;
    	if(tmpVal>maxVal)
    	{
    		iMax = i;
    		maxVal = tmpVal;
    	}
    }
    rpm = 1.0*iMax*VITAL_FPS/VITAL_FFT_N;
    maxVal = 0;
	for(i=beginFreqBpm;i<=endFreqBpm;i++)
	{
		float tmpVal = fftdata[i].i*fftdata[i].i + fftdata[i].q*fftdata[i].q;
		if(tmpVal>maxVal)
		{
			iMax = i;
			maxVal = tmpVal;
		}
	}
	bpm = 1.0*iMax*VITAL_FPS/VITAL_FFT_N;


    mOut->data[0].i = rpm;   //rpm=12
    mOut->data[0].q = bpm;   //bpm=70;
    //printf("rpm=%f, bpm=%f\n", 12.0, 70.0);
    return 0;
}

task_info createTaskGetRpmBpm(void* input)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "getVital");
    ti.func_ptr = getVital;
    ti.input = input;

    //out complex data as rpm + 1j*bpm
    ti.output = taskMemAlloc(sizeof(matc));
    matc* mOut = (matc*)ti.output;
    mOut->dim_cnt = 1;
    strcpy(mOut->mat_describe, "real");
    mOut->dims[0]=1;
    mOut->data = taskMemAlloc(sizeof(complex));

    return ti;
}


int vitalSignDetect()
{
	printf("create vital sign task flow...\n");
	task_flow tf = createTaskFlow("vs");
	//addRadarInput("{,}");
	void* radarDataPtr = getRadarDataAddr(0);
	task_info taskFIR = createTaskFIR(radarDataPtr, 256, "Hamming",1,2);
	task_info taskFFT = createTask1DFFT(taskFIR.output);
	task_info taskVMD = createTaskVMD(0);
	task_info taskGetVital = createTaskGetRpmBpm(taskFFT.output);

	addTaskNode(1,0, &taskFIR, &tf);
	addTaskNode(2,1, &taskFFT, &tf);
	addTaskNode(3,2,&taskVMD, &tf);
	addTaskNode(4,3,&taskGetVital, &tf);


	printf("run vital sign tasks...\n");

	printTaskFlow(&tf);
	runTaskFlow(&tf, 10);
	destroyTaskFlow(&tf);


	printf("vital sign tasks done.\n");
	return 0;
}

