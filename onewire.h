#ifndef __ONEWIRE_H_
#define __ONEWIRE_H_
#include <STC15F2K60S2.h>
#define uchar unsigned char
#define uint unsigned int
sbit DQ = P1^4;  //单总线接口
extern uchar rd_temperature(void);   
#endif