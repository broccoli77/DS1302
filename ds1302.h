
#ifndef  __DS1302_H__
#define  __DS1302_H__

#include <STC15F2K60S2.h>
#include<intrins.h>
/********************************************************************/ 
sbit SCK=P1^7;		
sbit SD =P2^3;		
sbit RST=P1^3;
/********************************************************************/ 
/*复位脚*/
#define RST_CLR	RST=0	/*电平置低*/
#define RST_SET	RST=1	/*电平置高*/
/*双向数据*/
#define SDA_CLR	SD=0	/*电平置低*/
#define SDA_SET	SD=1	/*电平置高*/
#define SDA_R	SD	/*电平读取*/	
/*时钟信号*/
#define SCK_CLR	SCK=0	/*时钟信号*/
#define SCK_SET	SCK=1	/*电平置高*/
/*************************写地址*******************************************/ 
#define ds1302_sec_addr_write		0x80		//秒数据地址
#define ds1302_min_addr_write		0x82		//分数据地址
#define ds1302_hor_addr_write		0x84		//时数据地址
#define ds1302_date_addr_write		0x86		//日数据地址
#define ds1302_month_addr_write		0x88		//月数据地址
#define ds1302_day_addr_write		0x8A		//星期数据地址
#define ds1302_year_addr_write		0x8C		//年数据地址
/***************************读地址**************************************/
#define ds1302_sec_addr_read		0x81		//秒数据地址
#define ds1302_min_addr_read		0x83		//分数据地址
#define ds1302_hor_addr_read	    0x85		//时数据地址
#define ds1302_date_addr_read		0x87		//日数据地址
#define ds1302_month_addr_read		0x89		//月数据地址
#define ds1302_day_addr_read		0x8B		//星期数据地址
#define ds1302_year_addr_read		0x8D		//年数据地址
/***************************************************************/
#define ds1302_control_addr		0x8E		//写保护命令字单元地址
#define ds1302_charger_addr		0x90 		//涓电流充电命令字地址			 
#define ds1302_clkburst_addr	0xBE		//日历、时钟突发模式命令字地址
/********************************************************************/ 

/********************************************************************/ 
/*单字节写入一字节数据*/
extern void Write_Ds1302_Byte(unsigned char dat);
/********************************************************************/ 
/*单字节读出一字节数据*/
extern unsigned char Read_Ds1302_Byte(void);
  
/********************************************************************/ 
/********************************************************************/ 
/*向DS1302单字节写入一字节数据*/
extern void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat);
/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
extern unsigned char Ds1302_Single_Byte_Read(unsigned char addr);

#endif	 
/********************************************************************/
//		　　　　　	END FILE
/********************************************************************/
