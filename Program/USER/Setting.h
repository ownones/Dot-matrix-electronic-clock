#define uchar unsigned char

#include "USART1.h"
#include "MESSAGE.h"

#define uint8  unsigned char 
#define uint16 unsigned int
#define uint32 unsigned long

sbit RESET = P5^4;
sbit BLUE_EN = P4^1;


extern unsigned char TEMP_UPDATE;//时间刷新位 1-6

extern void main(void);
extern void Check_bluetooth(void);
extern void Blue_Set(void);
extern void Menu_Init(void);
extern void Shift_Time(void);
extern void  I2C_DAY_Write(void);
extern void  I2C_TIME_Write(void);


extern void PrintString2(unsigned char *puts);