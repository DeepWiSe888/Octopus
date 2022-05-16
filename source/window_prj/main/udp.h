//
// Created by Administrator on 2022/5/7.
//

#ifndef MIMO_UDP_UDP_H
#define MIMO_UDP_UDP_H

#ifdef WIN32

#include <winsock2.h>


#else

#include <sys/socket.h>
#define INVALID_SOCKET   (SOCKET)(~0)
typedef int     SOCKET;

#endif

extern SOCKET fpga_udp_socket_fd;

int  fpga_udp_init(void);
void udp_disinit(void);
int udp_send(char * data, int len);
int udp_recv(char * data, int len);

int win_udp_init(void);
#endif //MIMO_UDP_UDP_H
