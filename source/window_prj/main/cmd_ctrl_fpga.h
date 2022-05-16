//
// Created by Administrator on 2021/10/26.
//

#ifndef CMD_CTRL_FPGA_H
#define CMD_CTRL_FPGA_H


typedef int(*callback_t)(char * data, int len); /* callback函数定义 */

void send_frame_read_size(int len);
void set_enable(void);
void set_disenable(void);
void set_init_disenable(void);
void set_init_enable(void);
void spi_write(unsigned char * data, unsigned  char len);
int  spi_read(unsigned char * data, int len);
int  spi_write_read(unsigned char * wdata, int wlen, unsigned char * rdata, int rlen);

void set_gfenable(void);
void set_ant_num(int tx_channel, int rx_channel);

int get_frame_num(unsigned char * data );
int find_head(unsigned char *buff, int len);


void cmd_ctrl_init(callback_t s_data, callback_t r_data);


#endif //CMD_CTRL_FPGA_H
