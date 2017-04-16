#include "ADC.h"

unsigned char  Init_R	= 10;

unsigned char Brightness;  //亮度缓存
extern unsigned char Delay_Red   ;//???????
extern unsigned char Delay_Green ;//??????
extern unsigned char Rspeed;
extern unsigned char Fspeed;
extern void  Delay_ms(unsigned int ms);

/*----------------------------
初始化ADC，STC的库
----------------------------*/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//结构定义
	ADC_InitStructure.ADC_Px        = ADC_P13;	//设置要做ADC的IO,	ADC_P10 ~ ADC_P17(或操作),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_90T;			//ADC速度			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC功率允许/关闭	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC结果调整,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//优先级设置	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//中断允许		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//初始化
	ADC_PowerControl(ENABLE);							//单独的ADC电源操作函数, ENABLE或DISABLE
}

/*----------------------------
自动亮度调整
----------------------------*/
void Auto_Brightness(void)
{

  if(Brightness == 0)
  {
    Brightness = (255-Get_ADC8bitResult(3))/2.55+6;
    
    if(Brightness>100){Brightness =100;Rspeed =1;}
    else {Rspeed = 2;}
  }
  /********亮度慢速加减********/
	if(Brightness+Init_R > Delay_Red)
  {
    Delay_Red++;
    Delay_Green++;
  }
  else if(Brightness+Init_R < Delay_Red)
  {
    Delay_Red--;
    Delay_Green--;
  }
  
}


