#include "Interrput.h"

/********************************
    定时器0，自动重装1毫秒
**********************************/
void Timer0Init(void)		//1毫秒@22.1184MHz
{ 
	AUXR |= 0x80;    //定时器时钟1T模式
	TMOD &= 0xF0;   //设置定时器模式
	TL0 = 0x9A;		 //设置定时初值
	TH0 = 0xA9;		//设置定时初值
	TF0 = 0;		 //清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;
  EA  = 1;
}

/********************************
        初始化中断
**********************************/
void Init_Interrput(void)
{
  Timer0Init();
}


/********************************
   定时器0工作 1ms工作一次
**********************************/
void Timer0(void) interrupt 1 using 1
{
	Init_ms++;
  if(Init_ms >= 1000)
  {
    Brightness = 0;
		INT_DHT11 += 1;//温度读取间隔
    Init_ms = 0;
  }
	if(START_DHT11 > 0)START_DHT11++;
}
