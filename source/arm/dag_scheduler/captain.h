/*
 * Project Octopus
 *
 */

#ifndef INC_CAPTAIN_H_
#define INC_CAPTAIN_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include "task_define.h"


// --- create task --- //
task_flow createTaskFlow(char* projName);


// --- add input add tasks --- //
int addRadarInput(radar_info *radarInfo, task_flow* taskFlow);
int addRadarInputDescirbe(char* radarInfo, task_flow* taskFlow); // "sig=iq, fps=40, tx=1, rx=1, winlen=800"
int addTaskNode(char taskNode, task_info* taskInfo, task_flow* taskFlow);
int setPreTask(char taskNo, char preTaskNo, task_flow* taskFlow);
#define TASK_DEPENDACE(_task_x, _pre_task_x, _task_flow_x)   setPreTask((_task_x).node,(_pre_task_x).node, (_task_flow_x));

// --- run / destroy ---  //
int runTaskFlow(task_flow* taskFlow, int maxTimeOut);
void destroyTaskFlow(task_flow* tf);
int printTaskFlow(task_flow* tf);


// --- util functions for internal use --- //
void* taskMemAlloc(int size);
void taskMemFree(void* ptr);


#ifdef  __cplusplus
}
#endif

#endif /* INC_CAPTAIN_H_ */
