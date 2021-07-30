/*
 * Project Octopus
 *
 */


#include "captain.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "task_sync.h"
#include "cmsis_os.h"

typedef struct _run_task_param
{
	task_info* taskInfo;
	task_sem_list* semList;
} run_task_param;

void startTaskThread(void const * argument)
{
	run_task_param * runParam = (run_task_param*)argument;

	task_info *ti = runParam->taskInfo;
	int taskNode = ti->node;

	// --- check semaphore take --- //
	task_sem_list *si = runParam->semList;
	while(si)
	{
		if(si->semInfo.nextTask == taskNode)
			waitSemaphore(si->semInfo.semNo, SEMAPHORE_WAIT_TIME_MAX);
		si = si->next;
	}

	// --- run task function --- //
    typedef int(*TASK_FUN)(task_info*);
    TASK_FUN pf = (TASK_FUN)(ti->func_ptr);
    pf(ti);

    // --- check give semaphore --- //
    si = runParam->semList;
	while(si)
	{
		if(si->semInfo.preTask == taskNode)
			giveSemaphore(si->semInfo.semNo);
		si = si->next;
	}

}



int runTask(task_info* ti, task_flow* tf)
{
	run_task_param runParam = {ti, tf->semList};

    char szTaskThread[32];
    sprintf(szTaskThread, "taskThread%d", ti->node);
    const osThreadDef_t thread1 = { szTaskThread, startTaskThread, osPriorityIdle, 0, 512, NULL, NULL };

    osThreadCreate(&thread1, &runParam);

	//startTaskThread(&runParam);
    return 0;
}



radar_info* createRadarInfo(char* infostr)
{
	radar_info *radarInfo = (radar_info*)taskMemAlloc(sizeof(radar_info));
	bzero(radarInfo, sizeof(radar_info));
	return radarInfo;
}


int addRadarInput(radar_info *radarInfo, task_flow* taskFlow)
{
	taskFlow->radarInfo = radarInfo;
	return 0;
}
task_flow createTaskFlow(char* projName)
{
	task_flow tf;
	memset(&tf, 0, sizeof(tf));
	return tf;
}

int addRadarInputDescribe(char* radarDescrib, task_flow* taskFlow) // "sig=iq, fps=40, tx=1, rx=1, winlen=800"
{
	radar_info* radarInfo = createRadarInfo(radarDescrib);
	taskFlow->radarInfo = radarInfo;
    return 0;
}


int addTaskNode(char taskNode, task_info* taskInfo, task_flow* tf)
{
	taskInfo->node = taskNode;
	// first one
	if(tf->taskList.taskInfo==0)
	{
		tf->taskList.taskInfo = taskInfo;
		//taskInfo->input = //radar data;
		return 0;
	}

	// add to tail
	task_list* plast = &tf->taskList;
	//if(plast->taskInfo->node == preNode)
	//	taskInfo->input = plast->taskInfo->output;
	while(plast->next)
	{
		plast = plast->next;
		//if(plast->taskInfo->node == preNode)
		//	taskInfo->input = plast->taskInfo->output;
	}
	task_list* pnew = (task_list*)taskMemAlloc(sizeof(task_list));
	pnew->taskInfo = taskInfo;
	pnew->next = 0;
	plast->next = pnew;
	return 0;
}


int setPreTask(char taskNo, char preTaskNo, task_flow* tf)
{
	int semNo = getNextFreeSemaphoreNo();
	task_sem_list* newSemList = (task_sem_list*)taskMemAlloc(sizeof(task_sem_list));
	newSemList->semInfo.nextTask = taskNo;
	newSemList->semInfo.preTask = preTaskNo;
	newSemList->semInfo.semNo = semNo;
	newSemList->next = 0;
	if(tf->semList==0)
		tf->semList = newSemList;
	else
	{
		task_sem_list* semlist = tf->semList;
        while(semlist->next)
        	semlist = semlist->next;
        semlist->next = newSemList;
	}

	return 0;
}

//int addTaskOutput(char preNode, char* buf, int maxsize, task_flow* taskFlow)
//{
//	return 0;
//}

int runTaskFlow(task_flow* taskFlow, int maxTimeOut)
{
    //run tasks
    task_list* taskList = &taskFlow->taskList;
    void *resultData = 0;
    while(taskList)
    {
        runTask(taskList->taskInfo, taskFlow);
        resultData = taskList->taskInfo->output;
        taskList = taskList->next;
    }
    //outputs

	return 0;
}

void destroyTaskFlow(task_flow* tf)
{

}

int printTaskFlow(task_flow* tf)
{
    return 0;
}

// --- util functions for internal use --- //

// -- use a list for dynamic memory recycle -- //
typedef struct _dynamic_mem_list{
    void* ptr;
    int size;
    struct _dynamic_mem_list* next;
} dynamic_mem_list;
static dynamic_mem_list* s_mem_list = 0;

void* taskMemAlloc(int size)
{
#ifdef _PHY_MEM
	return 0;
#endif

//#ifdef _X86
    // create dynamic memory at PC env
    void* newMem = malloc(size);

    if(s_mem_list==0)
    {
    	s_mem_list = (dynamic_mem_list*)malloc(sizeof(dynamic_mem_list));
    	s_mem_list->ptr = 0;
    	s_mem_list->size = 0;
    	s_mem_list->next = 0;
    }

    if(s_mem_list->ptr==0)
    {
    	s_mem_list->ptr = newMem;
    	s_mem_list->size = size;
        return newMem;
    }
    dynamic_mem_list *pLast = s_mem_list;
    while(pLast->next)
    {
        pLast = pLast->next;
    }
    pLast->next = (dynamic_mem_list*)malloc(sizeof(dynamic_mem_list));
    pLast = pLast->next;
    pLast->next = 0;
    pLast->ptr = newMem;
    pLast->size = size;
    return newMem;
//#endif

	return 0;
}


void taskMemFree(void* ptr)
{
#ifdef _PHY_MEM
	return 0;
#endif


	dynamic_mem_list *pLast = 0;
    dynamic_mem_list *pCur = s_mem_list;
    while(pCur)
    {
		if(pCur->ptr == ptr)
		{
			free(pCur->ptr);
			pCur->ptr = 0;
			pCur->size = 0;
			if(pLast)
			{
				pLast->next = pCur->next;
			}
			else
			{
				s_mem_list = pCur->next;
			}
			free(pCur);
			return;
		}
		pLast = pCur;
		pCur = pCur->next;
    }

    // no ptr found;
    // TODO: assert false;

}
