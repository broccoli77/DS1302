/****************************************************************
项目：第八届蓝桥杯福建赛区试题
编写者：黄东明
*****************************************************************/
#include "ds1302.h"
#include "display.h"
#include "KeySan.h"
#include "onewire.h"
/************写入时钟数据初始化***************BCD码*******************/
void ds1302_init(void)	   //写入时分秒
{
    Ds1302_Single_Byte_Write(ds1302_control_addr, 0x00);
    Ds1302_Single_Byte_Write(ds1302_sec_addr_write, 0x51); //秒
	Ds1302_Single_Byte_Write(ds1302_min_addr_write, 0x59); //分
    Ds1302_Single_Byte_Write(ds1302_hor_addr_write, 0x23); //时
	Ds1302_Single_Byte_Write(ds1302_control_addr,  0x80);
}
void ds1302_set(const unsigned char * t)
{
    Ds1302_Single_Byte_Write(ds1302_sec_addr_write, (((t[2]/10)<<4)|(t[2]%10))); //秒
	Ds1302_Single_Byte_Write(ds1302_min_addr_write, (((t[1]/10)<<4)|(t[2]%10))); //分
    Ds1302_Single_Byte_Write(ds1302_hor_addr_write, (((t[0]/10)<<4)|(t[2]%10))); //时
}
/*********************************************************************/
unsigned char h_m_s[]={0,1,2};
unsigned char time[]={0,0,0,0,0,0,0,0};	//时钟数组								   
/************************显示*******************************************/
void get_Time(unsigned char *array)
{
	array[7] = h_m_s[2] & 0x0f;
	array[6] = h_m_s[2] >>4;
	array[4] = h_m_s[1] & 0x0f;
	array[3] = h_m_s[1] >>4;
	array[1] = h_m_s[0] & 0x0f;
	array[0] = h_m_s[0] >>4;
	/**********hh-mm-ss*******1-4-7*******/
}
/*******************************************************************/
void ds1302_read(void)//读取时分秒
{
    h_m_s[2] = Ds1302_Single_Byte_Read(ds1302_sec_addr_read);
	h_m_s[1] = Ds1302_Single_Byte_Read(ds1302_min_addr_read);
    h_m_s[0] = Ds1302_Single_Byte_Read(ds1302_hor_addr_read);	
}
/******************************************************************************************************************************************************/
/*****************闹钟存储比对******************************/
unsigned char Led_Time=250;//5秒钟     5000ms  200ms*25次
unsigned char Led_Interval=0;//0.2秒钟 200ms 亮0.2秒灭0.2秒
unsigned char clock[]={0,0,10,0,0,10,0,0};	//时钟数组
unsigned char h_m_s_c[]={0,0,0};
bit successFlag=0;//比较成功标志位
bit led_On_Off=0;//1亮0灭
void Go_led()
{
    P2=((P2&0x1f)|0x80);
	P00=led_On_Off;
	P2&=0x1f;
}
void Led(void)
{
    if(Led_Time!=0)
	{
	    if(Led_Interval==200)
		{   
			Go_led();
			led_On_Off=~led_On_Off;
			Led_Time--;
			Led_Interval=0;	
		}		
	}
	else successFlag=0;
}  

void time_Contrast(void)
{
    unsigned char i;
    for(i=0;i<7;i++)
	{
	    if(time[i]!=clock[i])return;
	}
	successFlag=1;   
}

/**********定时器配置，用于时钟扫描**************************/
void Time_0_Init(void)
{  
    AUXR|= 0x80;		//定时器时钟1T模式
	TMOD|= 0x01;  //配置定时器工作模式
    TH0 = (65536-1000)/256;
    TL0 = (65536-1000)%256;  
    EA  = 1;
    ET0 = 1;  //打开定时器中断
    TR0 = 1;  //启动定时器	
}
void close_Buzz()//蜂鸣器
{
    P2=((P2&0x1f)|0xA0);
	P0=1;
	P2&=0x1f;
}
/********************************************************************************************************************************************/
unsigned char keycount=0;
bit setTime_or_clock=0;
bit stop_read=0;
bit tempFlag=0;
unsigned char temp_Num[]={11,11,11,11,11,2,2,12};
void key_Select()//时间按键选择时分秒判断
{
    unsigned char temp;
    switch(KeySan())
	{
	    case 1:
		    tempFlag=0;
			setTime_or_clock=0;//进入时钟设定
			stop_read=1;
			if(keycount==2)keycount=0;
			else keycount++;
			break;
		case 2:
		    tempFlag=0;
			setTime_or_clock=1;//进入闹钟设定
			stop_read=1;
			if(keycount==2)keycount=0;
			else keycount++;
			break;

		case 3:
		    tempFlag=0;
			if(setTime_or_clock==0)
			{
			    h_m_s[keycount]=h_m_s[keycount]+1; //时钟加
				ds1302_set(h_m_s);
			}
			else
			{ 
			    h_m_s_c[keycount]=h_m_s_c[keycount]+1;	//闹钟加		
			}
			stop_read=0;
			break;
		
		case 4:
		    tempFlag=1;	    
        	temp=rd_temperature();//温度
			temp_Num[6]=temp%10;
			temp_Num[5]=temp/10;
			break;/*
		    if(setTime_or_clock==0)
			{
		     	h_m_s[keycount]=h_m_s[keycount]-1;
				ds1302_set(h_m_s);
			}
			else{
			    h_m_s_c[keycount]=h_m_s_c[keycount]-1;
			}
			stop_read=0;*/
			
		case 0:tempFlag=0;break;
	}
}
/****************************************************************************************************************************************************/
void main(void)
{
    close_Buzz();
	/**********************/
	time[2] = 10; //-
	time[5] = 10; //-
    ds1302_init();//初始化时钟
	Time_0_Init();
	while(1)//循环读取
	{
		key_Select();
		if(successFlag==0)
		{
		    if(stop_read==0){ds1302_read();	get_Time(time);} 	
			time_Contrast();
		}
		else Led();
	}
} 
/**********************************************************/
//定时器中断服务函数

void isr_timer_0(void)  interrupt 1  //默认中断优先级 1    这里是1ms
{  
	if(successFlag==1)
	{
	    Led_Interval++;
	}
    else{
	    if(tempFlag==0)
		{
		    if(setTime_or_clock==0)Go_display(time);
			else Go_display(clock);
		}
		else  Go_display(temp_Num);
	}

    TH0 = (65536-1000)/256;
    TL0 = (65536-1000)%256;  //定时器重载 
}
