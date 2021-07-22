/*
 * captain.h
 *
 *  Created on: 2021年6月25日
 *      Author: link
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
int addTaskNode(char taskNode, char preNode, task_info* taskInfo, task_flow* taskFlow);
//int addTaskOutput(char preNode, char* buf, int maxsize, task_flow* taskFlow);

// --- run / destroy ---  //
int runTaskFlow(task_flow* taskFlow, int maxTimeOut);
void destroyTaskFlow(task_flow* tf);
int printTaskFlow(task_flow* tf);


// --- util functions for internal use --- //
void* taskMemAlloc(int size);
void taskMemFree(void* ptr);

/**********
 * example *
createTaskFlow();
addRadarInput(0, "sig=iq, fps=40, tx=1, rx=1, winlen=800");
addTaskNode(0,1, task_info{"FIR"} );
addTaskNode(1,2, task_info{"FFT"} );
addTaskNode(2,3, task_info{"Vital"} );
addTaskNode(1,4, task_info{"CNN"} );
addTaskNode(4,5, task_info{"PPC"} );
addOutput(3, "dest=uart, dest=print"); //print vital
addOutput(5, "dest=uart, dest=print"); //print people counting

runTask();

**********/


#ifdef  __cplusplus
}
#endif

#endif /* INC_CAPTAIN_H_ */
