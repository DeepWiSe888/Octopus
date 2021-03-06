/*
 * Project Octopus
 *
 */


#ifndef INC_BLAS_TASKS_H_
#define INC_BLAS_TASKS_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include "task_define.h"


#ifdef _FREERTOS
#include "cmsis_os.h"

SemaphoreHandle_t semFFT;
SemaphoreHandle_t semCNN;
#endif

#define VITAL_WIN_LEN	(800)
#define VITAL_FPS		(40)
#define VITAL_FFT_N		(1024)



//  --- FIR --- //
// order: 256 by default
// winMode: only "hamming" support as now
task_info createTaskFIR(void* input, int order, char* winMode, int lowStopHz, int highStopHz);




// --- FFT --- //
task_info createTask1DFFT(void* input);


// --- VMD --- //
task_info createTaskVMD(void* input);


// --- CNN People Counting --- //
task_info createTaskCNN_PeopleCounting(void* input);



// --- utils --- //



#ifdef  __cplusplus
}
#endif
#endif /* INC_BLAS_TASKS_H_ */
