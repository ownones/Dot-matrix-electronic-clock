
#include "font.h"
#include "MESSAGE.h"

#define uint8 unsigned char 
#define uint16 unsigned int

void Page_Show(void);
void Roll_Show(void);
void Roll_wait(void);
void Shift_Time(void);
void Static_display(void);
void dot_show(void);
void Real_time_Static(void);
void D_Roll(uint8);
void Roll_char(uint8, uint16);



//unsigned char Fspeed =1;
unsigned char Rspeed =2;
#define Swait  40;

unsigned char Fposition   = 0;//现在上移的位置
unsigned char Colour_Time = 2;//翻页时间颜色， 1：红色 2：绿色 3：黄色
unsigned char Page_speed  = 3;//定义翻页速度
unsigned char Rposition   = 0;//现在滚动的位置
unsigned char Rposition_n = 0;//滚动n字符位清零
unsigned char Colour_Roll = 1;//滚动时间颜色， 1：红色 2：绿色 3：黄色
unsigned char Roll_speed  = 3;//定义滚动速度
unsigned char Stop_wait	  = 0;//定义静止时间
bit			  Roll_Stop   = 1;//0：滚动停止，1：滚动开始
unsigned char dot_modle   = 2;//时钟小数点显示方式
unsigned char Dposition	  = 0;//小数点行走位
unsigned char dot_colour  = 2;//小数点颜色
unsigned char Display_mode = 3;//显示方式： 1：滚动翻页显示 2：静态翻页显示 3：静态翻页12小时制

extern unsigned char Temperature;//读取的温度
extern unsigned char Humidity;//读取的湿度  

extern unsigned char TEMP_UPDATE;//时间刷新位 1-6

extern unsigned char nong_Day, nong_Month;//转换后农历时间

extern unsigned char xdata show_R[16][16];
extern unsigned char xdata show_G[16][16];

//以下是修改测试定义

unsigned char dot_delay = 0x00;