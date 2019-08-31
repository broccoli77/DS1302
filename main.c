/****************************************************************
��Ŀ���ڰ˽����ű�������������
��д�ߣ��ƶ���
*****************************************************************/
#include "ds1302.h"
#include "display.h"
#include "KeySan.h"
#include "onewire.h"
/************д��ʱ�����ݳ�ʼ��***************BCD��*******************/
void ds1302_init(void)	   //д��ʱ����
{
    Ds1302_Single_Byte_Write(ds1302_control_addr, 0x00);
    Ds1302_Single_Byte_Write(ds1302_sec_addr_write, 0x51); //��
	Ds1302_Single_Byte_Write(ds1302_min_addr_write, 0x59); //��
    Ds1302_Single_Byte_Write(ds1302_hor_addr_write, 0x23); //ʱ
	Ds1302_Single_Byte_Write(ds1302_control_addr,  0x80);
}
void ds1302_set(const unsigned char * t)
{
    Ds1302_Single_Byte_Write(ds1302_sec_addr_write, (((t[2]/10)<<4)|(t[2]%10))); //��
	Ds1302_Single_Byte_Write(ds1302_min_addr_write, (((t[1]/10)<<4)|(t[2]%10))); //��
    Ds1302_Single_Byte_Write(ds1302_hor_addr_write, (((t[0]/10)<<4)|(t[2]%10))); //ʱ
}
/*********************************************************************/
unsigned char h_m_s[]={0,1,2};
unsigned char time[]={0,0,0,0,0,0,0,0};	//ʱ������								   
/************************��ʾ*******************************************/
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
void ds1302_read(void)//��ȡʱ����
{
    h_m_s[2] = Ds1302_Single_Byte_Read(ds1302_sec_addr_read);
	h_m_s[1] = Ds1302_Single_Byte_Read(ds1302_min_addr_read);
    h_m_s[0] = Ds1302_Single_Byte_Read(ds1302_hor_addr_read);	
}
/******************************************************************************************************************************************************/
/*****************���Ӵ洢�ȶ�******************************/
unsigned char Led_Time=250;//5����     5000ms  200ms*25��
unsigned char Led_Interval=0;//0.2���� 200ms ��0.2����0.2��
unsigned char clock[]={0,0,10,0,0,10,0,0};	//ʱ������
unsigned char h_m_s_c[]={0,0,0};
bit successFlag=0;//�Ƚϳɹ���־λ
bit led_On_Off=0;//1��0��
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

/**********��ʱ�����ã�����ʱ��ɨ��**************************/
void Time_0_Init(void)
{  
    AUXR|= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD|= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-1000)/256;
    TL0 = (65536-1000)%256;  
    EA  = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��	
}
void close_Buzz()//������
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
void key_Select()//ʱ�䰴��ѡ��ʱ�����ж�
{
    unsigned char temp;
    switch(KeySan())
	{
	    case 1:
		    tempFlag=0;
			setTime_or_clock=0;//����ʱ���趨
			stop_read=1;
			if(keycount==2)keycount=0;
			else keycount++;
			break;
		case 2:
		    tempFlag=0;
			setTime_or_clock=1;//���������趨
			stop_read=1;
			if(keycount==2)keycount=0;
			else keycount++;
			break;

		case 3:
		    tempFlag=0;
			if(setTime_or_clock==0)
			{
			    h_m_s[keycount]=h_m_s[keycount]+1; //ʱ�Ӽ�
				ds1302_set(h_m_s);
			}
			else
			{ 
			    h_m_s_c[keycount]=h_m_s_c[keycount]+1;	//���Ӽ�		
			}
			stop_read=0;
			break;
		
		case 4:
		    tempFlag=1;	    
        	temp=rd_temperature();//�¶�
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
    ds1302_init();//��ʼ��ʱ��
	Time_0_Init();
	while(1)//ѭ����ȡ
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
//��ʱ���жϷ�����

void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1    ������1ms
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
    TL0 = (65536-1000)%256;  //��ʱ������ 
}
