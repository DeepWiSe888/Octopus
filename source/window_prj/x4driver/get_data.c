//
// Created by Administrator on 2022/5/7.
//

#include "get_data.h"
#include "recvdata.h"
#include "radar.h"
#include "cmd_ctrl_fpga.h"
#include <x4driver.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>

extern X4Driver_t *x4driver ;

void get_frame_data(unsigned char * data,int len, float32_t * data_frame_normolized, int fd_count)
{
    _x4driver_unpack_and_normalize_downconverted_frame(x4driver,  data_frame_normolized,   fd_count , data, len );
}

void *get_x4data(void)
{
    x4_frame_data  save_data;
    unsigned  char buff[2048];
    char txt_name[256];
    int last_frame_num = 0;
    int new_frame_num  = 0;
    int lost_frame     = 0;
    int cnt_frame_num  = 0;
    datatype data_frame_normolized[300];
    struct timeval tv_start, tv_end;
    struct timezone tz;
    int dif_time;
    gettimeofday(&tv_start, &tz);
#if SAVE_DATA_EN
    FILE *p = fopen("data.txt","wb");
    if(p)printf("open p ok \n");
    save_data.bins = malloc(sizeof(float) *( (x4driver->frame_read_size / x4driver->bytes_per_counter) ) ) ;
    printf("  bind:%d\n", (x4driver->frame_read_size / x4driver->bytes_per_counter) );
#endif
    while (1)
    {
        // usleep(10);
        gettimeofday(&tv_end, &tz);
        // printf("time:%d\n", tv_end.tv_sec);
        dif_time = tv_end.tv_sec - tv_start.tv_sec;
        if(abs(dif_time) == 10)
        {
            // printf("last_frame_num : %d \n", last_frame_num);
            // printf("new_frame_num : %d \n", new_frame_num);
            //   last_time=localtime(&t);
            gettimeofday(&tv_start, &tz);
            printf("FPS:%d\n", (new_frame_num - last_frame_num)/10);
            last_frame_num = new_frame_num;
#if   SAVE_DATA_EN
            fclose(p); //关闭文件
            sprintf(txt_name, "data_%d.txt", tv_end.tv_sec);
            p = fopen(txt_name,"wb");
#endif
        }

        if( fifo_get(buff, 1680) == 1)
        {

            new_frame_num = get_frame_num(buff);
            lost_frame = new_frame_num - cnt_frame_num;
            if( lost_frame != 1)
            {
//                for(int i = 0; i < 1681; i ++)
//                    printf("i2:%x  ",buff2[i]);
//                printf("\n");
//                for(int i = 0; i < 1681; i ++)
//                printf("i:%x  ",buff[i]);
//                printf("\n");
                printf("lost data: %d   : %x %x \n", lost_frame, new_frame_num, cnt_frame_num);
            }

            cnt_frame_num = new_frame_num;
            memset(data_frame_normolized, 0, sizeof (data_frame_normolized));
            get_frame_data(buff + 24, 1656, data_frame_normolized, 276);
            // for(int i = 0; i < 277; i ++)printf("data: %d: %f \n", i, data_frame_normolized[i]);
#if SAVE_DATA_EN

            save_data.ts_sec  = tv_end.tv_sec;
            save_data.ts_usec = tv_end.tv_usec;
            save_data.rxno    = g_rx_num;
            save_data.txno    = g_tx_num;
            save_data.fall    = 0;
            memcpy(save_data.bins, data_frame_normolized,sizeof(float) * 276 );

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
