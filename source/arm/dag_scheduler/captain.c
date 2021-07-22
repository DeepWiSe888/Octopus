/*
 * captain.c
 *
 *  Created on: 2021年6月27日
 *      Author: link
 */


#include "captain.h"
#include <memory.h>
#include <stdlib.h>


int runTask(task_info* ti)
{
    typedef int(*TASK_FUN)(task_info*);
    TASK_FUN pf = (TASK_FUN)(ti->func_ptr);
    pf(ti);
    return 0;
}

radar_info* createRadarInfo(char* infostr)
{
	radar_info *radarInfo = (radar_info*)malloc(sizeof(radar_info));
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


int addTaskNode(char taskNode, char preNode, task_info* taskInfo, task_flow* tf)
{
	// first one
	if(tf->taskList.taskInfo==0)
	{
		tf->taskList.taskInfo = taskInfo;
		//taskInfo->input = //radar data;
		return 0;
	}

	// add to tail
	task_list* plast = &tf->taskList;
	if(plast->taskInfo->node == preNode)
		taskInfo->input = plast->taskInfo->output;
	while(plast->next)
	{
		plast = plast->next;
		if(plast->taskInfo->node == preNode)
			taskInfo->input = plast->taskInfo->output;
	}
	task_list* pnew = (task_list*)malloc(sizeof(task_list));
	pnew->taskInfo = taskInfo;
	pnew->next = 0;
	plast->next = pnew;
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
        runTask(taskList->taskInfo);
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
