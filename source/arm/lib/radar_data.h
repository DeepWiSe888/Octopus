/*
 * Project Octopus
 *
 */


#ifndef INC_RAAR_DATA_H_
#define INC_RAAR_DATA_H_

#ifdef  __cplusplus
extern "C"
{
#endif
#include "task_define.h"

//void setupRadarInput(radar_info* radarInfo);
void* getRadarDataAddr(radar_info* radarInfo);

#ifdef  __cplusplus
}
#endif
#endif
