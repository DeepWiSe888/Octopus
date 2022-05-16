#ifndef VPAS_RADAR_H
#define VPAS_RADAR_H
#include "stdint.h"

#define datatype float

typedef struct {
    datatype real;
    datatype imag;
}Complex;

int radar_start(void);

void radar_stop(void*);

int radar_enable();
uint32_t task_radar_init();
int radar_needrestart();
int x4driver_data_ready(void);
#endif // VPAS_RADAR_H
