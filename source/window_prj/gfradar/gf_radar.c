//
// Created by Administrator on 2022/3/15.
//
#include <sys/time.h>
#include <stdio.h>
#include <main.h>
#include <math.h>
#include <stdlib.h>
#include "gf_radar.h"
#include "recvdata.h"
#include "cmd_ctrl_fpga.h"


#if SAVE_DATA_EN
extern int  g_tx_num ;
extern int  g_rx_num ;
#endif

void get_gf_data()
{
    GF_frame_data  save_data;
    unsigned  char buff[2048];
    char txt_name[256];
    int last_frame_num = 0;
    int new_frame_num  = 0;
    int lost_frame     = 0;
    int cnt_frame_num  = 0;
    struct timeval tv_start, tv_end;
    struct timezone tz;
    int dif_time;
    gettimeofday(&tv_start, &tz);
#if SEND_TO_UDP

#endif

#if SAVE_DATA_EN
    FILE *p = fopen("data.txt","wb");
    if(p)printf("open p ok \n");

    save_data.bins = malloc(sizeof(char) * 1014 ) ;

   // printf("  bind:%d\n", (x4driver->frame_read_size / x4driver->bytes_per_counter) );
#endif

    //set_FPS(30);
    //set_SR(900);
    set_ant_num(0b1111,0b1111111111111111);
    set_gfenable();

    while (1) {
        // usleep(10);
        gettimeofday(&tv_end, &tz);
        // printf("time:%d\n", tv_end.tv_sec);
        dif_time = tv_end.tv_sec - tv_start.tv_sec;
        if (abs(dif_time) == 10) {

            // printf("last_frame_num : %d \n", last_frame_num);
            // printf("new_frame_num : %d \n", new_frame_num);
            //   last_time=localtime(&t);
            gettimeofday(&tv_start, &tz);
            printf("FPS:%d\n", (new_frame_num - last_frame_num) / 10);
            last_frame_num = new_frame_num;
#if   SAVE_DATA_EN
            fclose(p); //关闭文件
            sprintf(txt_name, "data_%d.txt", tv_end.tv_sec);
            p = fopen(txt_name,"wb");
#endif
        }

        if (fifo_get(buff, 490)) {
#if SEND_TO_UDP

#endif
             new_frame_num = get_frame_num(buff);
            //printf("frame_num : %d \n", new_frame_num);
            lost_frame = new_frame_num - cnt_frame_num;
            if (lost_frame != 1) {
                printf("lost data: %d   : %x %x \n", lost_frame, new_frame_num, cnt_frame_num);
            }
            cnt_frame_num = new_frame_num;


#if SAVE_DATA_EN

            save_data.ts_sec  = tv_end.tv_sec;
            save_data.ts_usec = tv_end.tv_usec;
            save_data.rxno    = g_rx_num;
            save_data.txno    = g_tx_num;
            save_data.fall    = 0;
            memcpy(save_data.bins, buff + 24,sizeof(char) * 992 );

           if(p)
           {

               fwrite( &save_data.ts_sec,4,1,p);
               fwrite( &save_data.ts_usec,4,1,p);
               fwrite( &save_data.txno,4,1,p);
               fwrite( &save_data.rxno,4,1,p);
               fwrite( &save_data.fall,4,1,p);
               fwrite( save_data.bins,4,276,p);

           }
           else
           {
               printf("open txt err \n");
           }
#endif

            //printf("\n");
//            if(p)
//            {
//                fprintf(p,"%d：  ", new_frame_num);
//                for(int i = 0; i < 276; i ++)
//                {
//                    fprintf(p," %d:%f  ", i, data_frame_normolized[i]);
//                }
//                fprintf(p,"\n");
//            }
//            else
//            {
//                printf("open txt err \n");
//            }
        }
    }
}