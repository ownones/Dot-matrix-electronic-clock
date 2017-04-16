#include "ADC.h"

unsigned char  Init_R	= 10;

unsigned char Brightness;  //���Ȼ���
extern unsigned char Delay_Red   ;//???????
extern unsigned char Delay_Green ;//??????
extern unsigned char Rspeed;
extern unsigned char Fspeed;
extern void  Delay_ms(unsigned int ms);

/*----------------------------
��ʼ��ADC��STC�Ŀ�
----------------------------*/
void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P13;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_90T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}

/*----------------------------
�Զ����ȵ���
----------------------------*/
void Auto_Brightness(void)
{

  if(Brightness == 0)
  {
    Brightness = (255-Get_ADC8bitResult(3))/2.55+6;
    
    if(Brightness>100){Brightness =100;Rspeed =1;}
    else {Rspeed = 2;}
  }
  /********�������ټӼ�********/
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

