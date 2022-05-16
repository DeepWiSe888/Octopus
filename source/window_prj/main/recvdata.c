//
// Created by Administrator on 2022/2/14.
//
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "recvdata.h"
#include "udp.h"
#include "cmd_ctrl_fpga.h"

unsigned  char fifo[FIFO_LEN];
int  fifo_len;
int  fifo_used;


void fifo_init(void)
{
    memset(fifo, 0, sizeof(fifo));
    fifo_len  = 0;
    fifo_used = 0;
}

int fifo_data_in(unsigned  char * data, int len)
{
    while (fifo_used)
    {
        usleep(100);
    }
    fifo_used = 1;

    if(fifo_len + len > FIFO_LEN)
    {
        printf("fifo  full ............................\n");
        fifo_used  = 0;
        return 0;
    }

    memcpy(fifo + fifo_len, data, len);
    fifo_len  += len;
   // printf("fifo_len_in : %d \n", fifo_len);
    fifo_used  = 0;
}

int fifo_get(unsigned char * data, int len)
{
    if(fifo_len <=  len)
    {
        // printf("fifo  empty ............................\n");
        return 0;
    }
    while (fifo_used)
    {
        usleep(100);
    }

    fifo_used = 1;
    int pos = find_head(fifo, fifo_len);
    if(pos < 0)
    {
        fifo_len = 0;
    }
    else if((fifo_len - pos ) >= len)
    {
        memset(data, 0 , sizeof (data));
        memcpy(data, fifo + pos , len);
        fifo_len = fifo_len - len - pos;
        memmove(fifo, fifo + len + pos , fifo_len );
        fifo_used = 0;
        return 1;
    }
    fifo_used = 0;
    return 0;
}

int udprecv(void)
{
    printf("udprecv\n");
    fifo_init();
    unsigned char rdata[2560];
    while (1)
    {
        int nRecv = udp_recv(rdata, sizeof(rdata));
        if(nRecv < 0) return -1;
        else fifo_data_in(rdata, nRecv);
        if(nRecv < 1000) printf(" nRecv:%d \n",nRecv);
    }
    return 1;
}
