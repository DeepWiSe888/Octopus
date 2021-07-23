/*
 * Project Octopus
 *
 */

#include "occupancy.h"

#include <string.h>
#include <stdio.h>
#include "captain.h"
#include "blas_tasks.h"
#include "radar_data.h"
#include "blas.h"
#include "fpga_define.h"


// --- VMD --- //
int getPeopleCount(task_info *ti);
task_info createTaskGetPPC(void* input)
{
    //build task
    task_info ti;
    bzero(&ti, sizeof(ti));
    strcpy(ti.func_name, "getPeopleCount");
	ti.input = 0;
	ti.output = taskMemAlloc(sizeof(uint32_t));
	ti.params = 0;
	ti.func_ptr = getPeopleCount;

	return ti;
}

int getPeopleCount(task_info *ti)
{
	// wait for i/o interrupt
    if( xSemaphoreTake( semCNN, portTICK_RATE_MS*1000*10 ) != pdTRUE )
    {
	    return 1;
    }

    uint16_t fmcDataAddr = FMC_ADDR_DATA;
    uint32_t peopleCount = fpga_read(fmcDataAddr);

    *((uint32_t*)(ti->output)) = peopleCount;


	return 0;
}



int occupancyDetect()
{
	task_flow tf = createTaskFlow("ppc"); //people counting
	void* radarDataPtr = getRadarDataAddr(0);

	task_info taskCNN = createTaskCNN_PeopleCounting(radarDataPtr);
	task_info taskGetPPC = createTaskGetPPC(taskCNN.output);

	addTaskNode(1,0, &taskCNN, &tf);
	addTaskNode(2,1,&taskGetPPC, &tf);

	runTaskFlow(&tf, 10);

	printf("result: people count = %d\n", *((uint32_t*)(taskGetPPC.output)));

	destroyTaskFlow(&tf);

	return 0;
}
