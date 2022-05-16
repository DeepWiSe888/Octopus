//
// Created by Administrator on 2022/2/14.
//

#ifndef MIMO_UDP_RECVDATA_H
#define MIMO_UDP_RECVDATA_H

#define FIFO_LEN  4096*16000

int udprecv(void);
int fifo_get(unsigned char * data, int len);



#endif //MIMO_UDP_RECVDATA_H
