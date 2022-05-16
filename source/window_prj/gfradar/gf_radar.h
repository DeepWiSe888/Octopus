//
// Created by Administrator on 2022/3/15.
//

#ifndef MIMO_UDP_GF_RADAR_H
#define MIMO_UDP_GF_RADAR_H

typedef struct _GFFRAMEDATA
{
    long ts_sec;
    long ts_usec;
    int txno;
    int rxno;
    int fall;//1=fall,0=no fall
    float* bins;
}GF_frame_data;

void get_gf_data();

#define SEND_TO_UDP 0

#endif //MIMO_UDP_GF_RADAR_H
