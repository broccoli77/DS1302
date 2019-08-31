#include <display.h>
code unsigned char tab[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff,0xc6};//显示数
                                                                             /*-,null,c*/
unsigned char dspbuf[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  //显示位
void display(unsigned char position,unsigned char num)//参数1显示位置，参数2显示数
{
    //段选，消隐
	P2=((P2&0x1f)|0xe0);
	P0=0xff;
	P2&=0x1f;
    //位选
	P2=((P2&0x1f)|0xc0);
	P0=dspbuf[position];
	P2&=0x1f;
     //段码输入
	P2=((P2&0x1f)|0xe0);
	P0=tab[num];
	P2&=0x1f;	
}
unsigned char i=0;
void Go_display(unsigned char *array)//将获取的time数组通过指针传入
{
	if(i==8)  i=0;
	else {
		display(i,array[i]);
    	i++;
	}
}
