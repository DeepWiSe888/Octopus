/*
 * Project Octopus
 *
 */

#include "task_sync.h"

#include "cmsis_os.h"

#define _FREE_RTOS
//#define _LINUX



#define MAX_SEMAPHORE	(100)
SemaphoreHandle_t g_sem[MAX_SEMAPHORE] = {0};

#ifdef _FREE_RTOS

SemaphoreHandle_t getSemaphore(int no)
{
	if(no>MAX_SEMAPHORE)
		return 0;
	if(g_sem[no]==0)
		g_sem[no] = xSemaphoreCreateBinary();
	return g_sem[no];
}

int waitSemaphore(uint32_t semNo, int waitTime_ms)
{
	if(semNo>MAX_SEMAPHORE)
		return SEMAPHORE_ERR_TOOMANY;
	SemaphoreHandle_t sem = getSemaphore(semNo);

    if( xSemaphoreTake( sem, portTICK_RATE_MS*waitTime_ms ) != pdTRUE )
    {
	    return SEMAPHORE_ERR_TIMEOVER;
    }

	return 0;
}
int giveSamphore(uint32_t semNo)
{
	if(semNo>MAX_SEMAPHORE)
		return SEMAPHORE_ERR_TOOMANY;

	SemaphoreHandle_t sem = getSemaphore(semNo);
	static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( sem, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	return 0;
}


#endif


int getNextFreeSemaphoreNo()
{
	static int nextSemNO = 1;
	int semNo = nextSemNO;
	nextSemNO ++;
	return semNo;
}
