#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <x4driver/get_data.h>
#include "radar.h"
#include "recvdata.h"
#include "gf_radar.h"
#include "main.h"
#include "udp.h"
#include "cmd_ctrl_fpga.h"

int main(int argc, char *argv[])
{
    if(fpga_udp_init() < 0)
    {
        printf("creat udp err \n ");
    }
    cmd_ctrl_init(udp_send, udp_recv);
#if X4_ENABLE
    pthread_t id1;
    sleep(1);
    set_disenable();
    sleep(1);
    set_enable();
    sleep(1);
    if(radar_start() < 0)
    {

        printf("radar_start err \n");
        while(1)
        {
            sleep(1);
        }

    }
    printf("create get_x4data \n");
    int ret1= pthread_create(&id1,NULL,(void*)get_x4data,NULL ); //创建线程
    if(ret1)
    {
        printf("create get_x4data error\n");
        return 1;
    }

#endif

#if GF_ENABLE
    pthread_t id2;
    printf("create get_gfdata \n");
    int ret2= pthread_create(&id2,NULL,(void*)get_gf_data,NULL );
    if(ret2)
    {
        printf("create get_x4data error\n");
        return 1;
    }
#endif

    printf("create udprecv \n");
    pthread_t id3;
    int ret3= pthread_create(&id3,NULL,(void*)udprecv,NULL );
    if(ret3)
    {
        printf("create udprecv error\n");
        return 1;
    }
    set_init_enable();
  //  set_init_enable();
   // set_init_enable();
    while(1)
    {

        sleep(1);
    }
    udp_disinit();
    return 0;
}
