#include <STC15F2K60S2.H>
#include<intrins.h>

#define uint8 unsigned char 
#define uint16 unsigned int

//*************IO口使用定义*****************
sbit DR1 = P0^0;//hc595数据
sbit DG1 = P1^0;
sbit DR2 = P0^4;
sbit DG2 = P0^5;

sbit RCK = P0^6;//74hc595输出
sbit SCK = P0^7;//74hc595时钟
sbit DAT = P0^1;//4094数据
sbit CLK = P0^2;//4094时钟
sbit EN  = P0^3;//4094锁存

//*************全局变量定义*****************
unsigned char Delay_Red    = 10;//红色亮度
unsigned char Delay_Green  = 40;//绿色亮度

unsigned char xdata show_R[16][16] = {0};
unsigned char xdata show_G[16][16] = {0};

//*****************宏定义*******************
#define HEF4094_EN_Enable()  {EN=0;}
#define HEF4094_EN_Disable() {EN=1;} 
#define HC595_out() {RCK=0;RCK=1;RCK=0;}

//****************函数声明******************
extern void  Delay_ms(unsigned int ms);
extern void  Delay_us(unsigned int us);

