//
// Created by Administrator on 2022/5/7.
//

#ifndef MIMO_UDP_GET_DATA_H
#define MIMO_UDP_GET_DATA_H

typedef struct _X4FRAMEDATA
{
    long ts_sec;
    long ts_usec;
    int txno;
    int rxno;
    int fall;//1=fall,0=no fall
    float* bins;
}x4_frame_data;

void *get_x4data(void);


#endif //MIMO_UDP_GET_DATA_H
