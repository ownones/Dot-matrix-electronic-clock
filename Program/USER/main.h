#define uint8 unsigned char 
#define uint16 unsigned int

//*************º¯ÊýÉùÃ÷*********************
extern void Check_bluetooth();
extern void Auto_Brightness();
extern void Init_Interrput();
extern void Shift_Time();
extern void Show_list();
extern void init_Time();
extern void ALL_Reset();
extern void set_mode();
extern void I2C_TIME();
extern void	ADC_config();
extern void I2C_init();
extern void Blue_Set();
extern void DHT11_Start();
extern void DHT11_Init();

#include <stc15f2k60s2.h>

sbit dat_dalay = P3^2;