#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include <STC15F2K60S2.h>
#include <intrins.h>
extern void display(unsigned char position,unsigned char num);
extern void Go_display(const unsigned char *array);//����ȡ��time����ͨ��ָ�봫��

#endif