//
// Created by Administrator on 2022/5/7.
//

#ifdef WIN32

#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

#else
#include <arpa/inet.h>
#define closesocket close
#include <unistd.h>
#include <stdio.h>
#include <string.h>



#endif

#include "udp.h"
#include "cmd_ctrl_fpga.h"

SOCKET fpga_udp_socket_fd;
SOCKET win_udp_socket_fd;

int udp_send(char * data, int len)
{
    send(fpga_udp_socket_fd, data, len, 0);
}

int udp_recv(char * data, int len)
{

    int nRecv = recv(fpga_udp_socket_fd, data, len, 0);
    if(nRecv < 0)
    {
        printf("fpga_udp_socket_fd nRecv err\n");
        return -1;
    }
    return nRecv;
}



int fpga_udp_init(void)
{
#ifdef WIN32
    WSADATA wsa_data;
    if(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        printf("WSAStartup() error!");
        return -1;
    }
    SOCKADDR_IN  serv_addr;
    SOCKADDR_IN  bindAddr;
#else
    struct  sockaddr_in serv_addr;
    struct  sockaddr_in bindAddr;
#endif
    fpga_udp_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(fpga_udp_socket_fd == INVALID_SOCKET)
    {
        printf("UDP socket create error");
        return -1;
    }
    //UDP客户端也可以绑定端口，与服务端并没有明显界限

    memset(&bindAddr, 0, sizeof(bindAddr));
    bindAddr.sin_addr.s_addr =  inet_addr("192.168.0.3");
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(6000);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.2");
    serv_addr.sin_port = htons(5000);

    int bind_cnt =  bind(fpga_udp_socket_fd, (struct sockaddr*)&bindAddr, sizeof(bindAddr));
    if(bind_cnt)
    {
        printf("bind err \n");
        return -1;
    }
#ifdef WIN32
    int ret = connect(fpga_udp_socket_fd, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
    if(ret < 0)
    {
        printf("connect err \n");
        return -1;
    }
#else
    int ret = connect(fpga_udp_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret < 0)
    {
        printf("connect err \n");
        return -1;
    }
#endif


    return  1;
}

void udp_disinit(void)
{
    closesocket(fpga_udp_socket_fd);
#ifdef WIN32
    WSACleanup();
#endif
}




#ifdef WIN32
SOCKADDR_IN  serv_addr;
#else
struct sockaddr_in serv_addr;
#endif
int win_udp_init(void)
{

    win_udp_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(win_udp_socket_fd == INVALID_SOCKET)
    {
        printf("UDP2 socket create error\n");
        return -1;
    }
    else
    {
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("192.168.137.1");
        serv_addr.sin_port = htons(5000);

    }

}
int win_udp_send(char * data, int len)
{
    sendto(win_udp_socket_fd, data, 1020, 0,(struct sockaddr *) &serv_addr, sizeof (serv_addr));
}