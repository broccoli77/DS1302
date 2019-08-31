#include "KeySan.h"

sbit s7=P3^0;// ±÷”
sbit s6=P3^1;//ƒ÷÷”
sbit s5=P3^2;//++
sbit s4=P3^3;//--

void Delay200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/*********************************
i=1 -s7
i=2 -s6
i=3 -s5
i=4 -s4
********************************/
unsigned char KeySan(void)
{
	if(s7==0)
    {
		Delay200ms();
		return 1;
	}
	else if(s6==0)
    {
		Delay200ms();
		return 2;
	}
	else if(s5==0)
    {
		Delay200ms();
		return 3;
	}
	else if(s4==0)
    {
		Delay200ms();
		return 4;
	}
	return 0;
}

