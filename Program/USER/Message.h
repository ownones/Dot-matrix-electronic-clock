#ifndef __Message_H_
#define __Message_H_
/****************************************************************
						����ʱ�亯���ṹ��
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
}SYSTEMTIME;	//�����ʱ������

extern SYSTEMTIME  CurrentTime;//ʵʱͬ��ʱ�ӽṹ�嶨��


extern void Delay_us(unsigned int us);
extern void Delay_ms(unsigned int ms);


#endif