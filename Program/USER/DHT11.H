#include <stc15f2k60s2.h>

sbit DAT_DHT11 = P5^4;

unsigned char Temperature = 0;//温度
unsigned char Humidity = 0;//湿度 
unsigned char START_DHT11 = 0;//开始读取温度那个18MS使用中断计时
unsigned char INT_DHT11 = 11;//读取频率

#define uint8 unsigned char 
#define uint16 unsigned int



extern void  Delay_ms(unsigned int ms);
extern void  Delay_us(unsigned int us);

extern void DHT11_Start(void);
extern void DHT11_Init(void);
   
  
  
  
  
  
  
  
