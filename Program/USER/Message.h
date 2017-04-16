#ifndef __Message_H_
#define __Message_H_
/****************************************************************
						定义时间函数结构体
****************************************************************/
typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
}SYSTEMTIME;	//定义的时间类型

extern SYSTEMTIME  CurrentTime;//实时同步时钟结构体定义


extern void Delay_us(unsigned int us);
extern void Delay_ms(unsigned int ms);


#endif