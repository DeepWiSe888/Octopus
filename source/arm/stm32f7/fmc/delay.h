#ifndef _DELAY_H
#define _DELAY_H
#include <sys.h>

//#include <stdint.h>

//typedef uint8_t u8;
//typedef uint32_t u32;

void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_xms(u32 nms);
#endif

