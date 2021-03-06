/*
 * Project Octopus
 *
 */

#ifndef INC_TASK_SYNC_H_
#define INC_TASK_SYNC_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>


// --- SEMAPHORE --- //
#define	SEMAPHORE_ERR_OK         (0)
#define	SEMAPHORE_ERR_TIMEOVER   (1)
#define	SEMAPHORE_ERR_TOOMANY    (2)

#define SEMAPHORE_WAIT_TIME_MAX	(9999)

int waitSemaphore(uint32_t semNo, int waitTime_ms);
int giveSemaphore(uint32_t semNo);

// if you don't want to maintain your semaphore number, use this self increase one.
int getNextFreeSemaphoreNo();


// --- Thread Function --- //
typedef void(*THREAD_FUN)(void const *);
int createTaskThread(const char* taskName, THREAD_FUN threadFun, void* param);

// --- UTILS --- //
void taskSleep(int milliseconds);



#ifdef  __cplusplus
}
#endif
#endif /* INC_TASK_SYNC_H_ */
