/*
 * Project Octopus
 *
 */

#include "task_sync.h"
#include "captain.h"


//#define _FREE_RTOS
//#define _LINUX




#define MAX_SEMAPHORE	(100)



int getNextFreeSemaphoreNo()
{
	static int nextSemNO = 1;
	int semNo = nextSemNO;
	nextSemNO ++;
	return semNo;
}


///////////////////////////////////////////////////////////////////////////
//
//     Free RTOS codes
//
///////////////////////////////////////////////////////////////////////////

#ifdef _FREE_RTOS

#include "cmsis_os.h"

SemaphoreHandle_t g_sem[MAX_SEMAPHORE] = {0};

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
int giveSemaphore(uint32_t semNo)
{
	if(semNo>MAX_SEMAPHORE)
		return SEMAPHORE_ERR_TOOMANY;

	SemaphoreHandle_t sem = getSemaphore(semNo);
	static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( sem, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	return 0;
}




int createTaskThread(const char* taskName, THREAD_FUN threadFun, void* param)
{
    const osThreadDef_t thread1 = { taskName, threadFun, osPriorityIdle, 0, 512, NULL, NULL };

    osThreadCreate(&thread1, param);
	return 0;
}


void taskSleep(int milliseconds)
{
    osDelay(milliseconds);
}

#endif





///////////////////////////////////////////////////////////////////////////
//
//     Linux codes
//
///////////////////////////////////////////////////////////////////////////

#ifdef _LINUX

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

// why sem_t* : sem_init not works at osx
sem_t* g_sem[MAX_SEMAPHORE] = {0};

sem_t* getSemaphore(int no)
{
    if(no>MAX_SEMAPHORE)
        return 0;
    if(g_sem[no]==0)
    {
        char szSem[32];
        sprintf(szSem, "sem%d", no);
        g_sem[no] = sem_open("/mysem", O_CREAT, S_IRUSR | S_IWUSR, 10);
    }
    return g_sem[no];
}


int waitSemaphore(uint32_t semNo, int waitTime_ms)
{
    if(semNo>MAX_SEMAPHORE)
        return SEMAPHORE_ERR_TOOMANY;
    sem_t* sem = getSemaphore(semNo);

    sem_wait(sem);

    return 0;
}
int giveSemaphore(uint32_t semNo)
{
    if(semNo>MAX_SEMAPHORE)
        return SEMAPHORE_ERR_TOOMANY;

    sem_t* sem = getSemaphore(semNo);
    sem_post(sem);
    return 0;
}


// construct a linux-style thread function //
//    rtos sytle: void fun(void*)
// -> linux style: void* fun(void*)
// these code sucks but no better idea
typedef  struct _linux_thread_param
{
    THREAD_FUN funptr;
    void* param;
} linux_thread_param;
void* linux_thread_fun(void* param)
{
    linux_thread_param* ltp = (linux_thread_param*)param;

    THREAD_FUN threadFun = ltp->funptr;
    void* funParm = ltp->param;
    if(threadFun)
        threadFun(funParm);

    taskMemFree(ltp);

    return 0;
}

int createTaskThread(const char* taskName, THREAD_FUN threadFun, void* param)
{
    linux_thread_param* ltp = (linux_thread_param*)taskMemAlloc(sizeof(linux_thread_param));
    ltp->funptr = threadFun;
    ltp->param = param;
    pthread_t p;
    pthread_create(&p, 0, linux_thread_fun, ltp);
    return 0;
}


#include <unistd.h>
void taskSleep(int milliseconds)
{
    usleep(milliseconds*1000);
}

#endif







