/*
 * Project Octopus
 *
 */


#include "vital_sign.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "captain.h"
#include "blas_tasks.h"
#include "radar_data.h"
#include "blas.h"
#include "linear_math.h"


typedef struct _get_vital_param
{
    matc*   mFFT;
    matc*   mVMD;
} get_vital_param;
int getVital(task_info * ti)
{
    get_vital_param * param = (get_vital_param*)ti->input;
    matc* mFFT = param->mFFT;
    matc* mVMD = param->mVMD;
    taskMemFree(param);

    matc *mOut = (matc*)ti->output;
    float rpm = 0;
    float bpm = 0;

    complex* vmdRPM = (complex*)(&M2V(mVMD, 0, 0));
    complex* vmdBPM = (complex*)(&M2V(mVMD, 1, 0));
    complex* fftdata = (complex*)(&M1V(mFFT, 0));

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


    mOut->data[0].i = rpm;
    mOut->data[0].q = bpm;
    printf("rpm=%f, bpm=%f\n", rpm, bpm);
    return 0;
}

task_info createTaskGetRpmBpm( void* mat1c_vmd, void* mat1c_fft)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "getVital");
    ti.func_ptr = getVital;
    get_vital_param * param = (get_vital_param*)taskMemAlloc(sizeof(get_vital_param));
    param->mFFT = mat1c_fft;
    param->mVMD = mat1c_vmd;
    ti.input = param;

    //out complex data as rpm + 1j*bpm
    ti.output = createMat1C(1);

    return ti;
}


/*****
 *  getVitalBin
 *  algorithm : find max std one.
 */
int getViatalBin(task_info* ti)
{
    //get radar iq mat
    matc* iqmat = (matc*)ti->input;
    if(iqmat->dim_cnt!=2)
    {
        return -1;
    }

    int frames = iqmat->dims[0];
    int rangebins = iqmat->dims[1]; // continuous memory at one frame
    if(rangebins>100)
        rangebins = 100;

    // do 2d std
    int i;
    float binVals[100]={0};
    stdc2d(iqmat, 0, binVals);

    // find vital bin
    int binInx = findMax(binVals, rangebins);
    if(binInx<0||binInx>=rangebins)
        return -2;

    // write output.
    matc* val = (matc*)ti->output;
    for(i=0;i<frames;i++)
    {
        M1V(val, i).i = M2V(iqmat, i, binInx).i;
        M1V(val, i).q = M2V(iqmat, i, binInx).q;
    }
    return 0;
}

task_info createTaskGetVitalBin(void* input)
{
    task_info ti;
    bzero(&ti, sizeof(ti));

    strcpy(ti.func_name, "findVitalBin");
    ti.func_ptr = getViatalBin;
    ti.input = input;

    //out complex data as bin_inx + 1j*0
    ti.output = createMat1C(VITAL_WIN_LEN);;

    return ti;
}


int vitalSignDetect()
{
	printf("create vital sign task flow...\n");
	task_flow tf = createTaskFlow("vs");

	// - use simulate radar data - //
	void* radarDataPtr = getRadarDataAddr(0);

	// - taskFindVitalBin:   input = radar iq mat;   output = vital bin iq - //
    task_info taskFindVitalBin = createTaskGetVitalBin(radarDataPtr);

    // - taskFIR:    input = iq vector;    output = iq vector- //
	task_info taskFIR = createTaskFIR(taskFindVitalBin.output, 256, "Hamming",1,2);

    // - taskFFT:    input = iq vector;    output = fft complex vector- //
	task_info taskFFT = createTask1DFFT(taskFIR.output);

    // - taskVMD:    input = iq vector;    output = iq vector- //
	task_info taskVMD = createTaskVMD(taskFindVitalBin.output);

    // - taskGetVital:    input = iq vector+fft vector;    output = complex value - //
	task_info taskGetVital = createTaskGetRpmBpm(taskVMD.output, taskFFT.output);


    addTaskNode(1,&taskFindVitalBin, &tf);
	addTaskNode(2,&taskFIR, &tf);
	addTaskNode(3,&taskFFT, &tf);
	addTaskNode(4,&taskVMD, &tf);
	addTaskNode(5,&taskGetVital, &tf);
	// FindVitalBin --> FIR --> FFT --> |
	//             |--> VMD ----------> | ---> GetVital
    TASK_DEPENDACE(taskFIR,      taskFindVitalBin, &tf)
    TASK_DEPENDACE(taskFFT,      taskFIR,          &tf)
    TASK_DEPENDACE(taskVMD,      taskFindVitalBin, &tf)
    TASK_DEPENDACE(taskGetVital, taskFFT,          &tf)
    TASK_DEPENDACE(taskGetVital, taskVMD,          &tf)

	printf("run vital sign tasks...\n");

	printTaskFlow(&tf);
	runTaskFlow(&tf, 10);
	destroyTaskFlow(&tf);


	printf("vital sign tasks done.\n");
	return 0;
}

