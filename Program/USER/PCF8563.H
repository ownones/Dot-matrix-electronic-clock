#include <stc15f2k60s2.h>
#include "MESSAGE.h"


#define uchar unsigned char
#define I2C_Read_place 7        //I2C连续读个数
#define I2C_Write_Time_Place 3  //I2C连续写时间
#define I2C_Write_DAY_Place 4   //I2C连续写日期


unsigned char TEMP_UPDATE = 6;//时间刷新位 1-6
SYSTEMTIME  CurrentTime;//实时同步时钟
unsigned char nong_Day, nong_Month;//转换后农历时间
extern unsigned char Display_mode;

void  I2C_init(void); 
void  I2C_TIME(void);
void  init_Time(void);
void  I2C_DAY_Write(void);
void  I2C_TIME_Write(void);
void  I2C_read_Burst_mode(uchar *date, address, place);          //申明要用到的函数
void  I2C_write_Burst_mode(uchar *date, address, place);
unsigned char bcd_conver_hex(unsigned char bcd_data);
unsigned char hex_convert_bcd(unsigned char hex_data);
extern void GetChinaCalendar(unsigned char  year,	unsigned char *month_IO,	unsigned char *day_IO);


#include "I2C.h"