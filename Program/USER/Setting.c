#include "Setting.h"

/*****************************
   	 开机按键初始化
******************************/
void ALL_Reset (void)
{
  if(!RESET)
  {
    Delay_ms(3000);
    if (!INT0)
    {
      CurrentTime.Second = 0x00;
      CurrentTime.Minute = 0x01;
      CurrentTime.Hour   = 0x02;
      CurrentTime.Day    = 0x03;
      CurrentTime.Week   = 0x04;
      CurrentTime.Month  = 0x05;
      CurrentTime.Year   = 0x06;
      I2C_TIME_Write();
      I2C_DAY_Write();
    }
	}
}

/*************	功能说明	**************

以下是蓝牙串口发送英文提示设置时间

******************************************/

#define Input_Error_Text {PrintString1("    Input Error, please Re-enter.\r\n");while(COM1.B_TX_busy);}


void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P36_P37;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
}

void Check_bluetooth(void)
{
  if(BLUE_EN == 1)
  {
    Delay_ms(10);
    if(BLUE_EN == 1)
      {
      Blue_Set();
      TEMP_UPDATE = 6;
      //  IAP_CONTR = 0X20;//stc软复位
      //  IAP_CONTR = 0X60;
      }
  }
}

uint8 Ascll_to_Hex(uint8 Ascll)
{
  if(Ascll >=0x30 && Ascll<= 0x39)
    return Ascll-0x30;
  else 
    return 99;
}


/*************  蓝牙设置时间 ******************/
void Set_time_bluetooth(void)
{
  uint8 S_hour, S_min, S_sec, i;
  PrintString1("    Hour(0-23) - Min(0-59) - Sec(0-59)\r\n");while(COM1.B_TX_busy);
  PrintString1("    For Example '12-59-59','99' back\r\n");while(COM1.B_TX_busy);

  while (1)//等待输入
	{
    Delay_ms(1);
    if(!BLUE_EN) return;
    if(COM1.RX_TimeOut > 0)		//超时计数
      if(--COM1.RX_TimeOut == 0)
      {
        if(COM1.RX_Cnt > 0)
        {
          PrintString1("    Your Input: "); 
          for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);PrintString1("\r\n"); while(COM1.B_TX_busy);//收到的数据原样返回
          
          if(COM1.RX_Cnt == 8)
          {
            S_hour = Ascll_to_Hex(RX1_Buffer[0])*10 + Ascll_to_Hex(RX1_Buffer[1]);//把Ascall转为BCD并合并时间
            S_min  = Ascll_to_Hex(RX1_Buffer[3])*10 + Ascll_to_Hex(RX1_Buffer[4]);
            S_sec  = Ascll_to_Hex(RX1_Buffer[6])*10 + Ascll_to_Hex(RX1_Buffer[7]);
            if(S_hour >= 0 && S_hour <24)
              if(S_min >= 0 && S_min <60)
                if(S_sec >= 0 && S_sec <60)
                {
                  CurrentTime.Second = S_sec;
                  CurrentTime.Minute = S_min;
                  CurrentTime.Hour   = S_hour;
                  I2C_TIME_Write();
                  PrintString1("    OK\r\n\n\n "); while(COM1.B_TX_busy);
                  return;
                }
          }
          else if(COM1.RX_Cnt == 2)
                 if(RX1_Buffer[0] == 99)
                   return;
          else
            Input_Error_Text;
            COM1.RX_Cnt = 0;
        }
      }
  }
}

/*************  蓝牙设置日期 ******************/
void Set_Date_bluetooth(void)
{
  uint8 S_day, S_month, S_year, S_week, i;
  PrintString1("    Year(01-99)-Month(01-12)-Day(01-31)-Week(00-06)\r\n");while(COM1.B_TX_busy);
  PrintString1("    For Example '14-01-01-00','99' back\r\n");while(COM1.B_TX_busy);

  while (1)//等待输入
	{
    Delay_ms(1);
    if(!BLUE_EN) return;
    if(COM1.RX_TimeOut > 0)		//超时计数
      if(--COM1.RX_TimeOut == 0)
      {
        if(COM1.RX_Cnt > 0)
        {
          PrintString1("    Your Input: "); 
          for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);PrintString1("\r\n"); while(COM1.B_TX_busy);//收到的数据原样返回
          
          if(COM1.RX_Cnt == 11)
          {
            S_year   = Ascll_to_Hex(RX1_Buffer[0])*10 + Ascll_to_Hex(RX1_Buffer[1]);//把Ascall转为BCD并合两位数
            S_month  = Ascll_to_Hex(RX1_Buffer[3])*10 + Ascll_to_Hex(RX1_Buffer[4]);
            S_day    = Ascll_to_Hex(RX1_Buffer[6])*10 + Ascll_to_Hex(RX1_Buffer[7]);
            S_week   = Ascll_to_Hex(RX1_Buffer[10]);
            if(S_year > 0 && S_year <=99)
              if(S_month > 0 && S_month <=12)
                if(S_day > 0 && S_day <=31)
                  if(S_week >= 0 && S_week <=6)
                  {
                    CurrentTime.Year   = S_year;
                    CurrentTime.Month  = S_month;
                    CurrentTime.Day    = S_day;
                    CurrentTime.Week   = S_week;
                    I2C_DAY_Write();
                    PrintString1("    OK\r\n\n\n "); while(COM1.B_TX_busy);
                    return;
                  }
          }
          else if(COM1.RX_Cnt == 2)
                 if(RX1_Buffer[0] == 99)
                   return;
                 
          else
            Input_Error_Text;
          COM1.RX_Cnt = 0;
        }
      }
  }
}

/**********************************************
            进入蓝牙设置菜单
**********************************************/
void Blue_Set(void)
{
	uint8	i;
  
	UART_config();
	EA = 1;

	while(1)
  {
    PrintString1("                                     Clock set\r\n");
      while(COM1.B_TX_busy);
    PrintString1("    1.Set time\r\n"); 
    PrintString1("    2.Set date\r\n");
      while(COM1.B_TX_busy);
    PrintString1("                                                       Please input the keys\r\n\n\n\n\n");
      while(COM1.B_TX_busy);
     
    while (1)
    {
      if(!BLUE_EN) return;
      Delay_ms(1);
      if(COM1.RX_TimeOut > 0)		//等待输入//超时计数
      {
        if(--COM1.RX_TimeOut == 0)
        {
          if(COM1.RX_Cnt > 0)
          {
            PrintString1("    Your input: "); 
            for(i=0; i<COM1.RX_Cnt; i++)	TX1_write2buff(RX1_Buffer[i]);PrintString1("\b\r\n"); while(COM1.B_TX_busy);	//收到的数据原样返回
            
            if(COM1.RX_Cnt == 1)//判断输入位数
            {
              COM1.RX_Cnt = 0;
              switch(RX1_Buffer[0])//选择要更改的地方
              {
              case 0x31:
                Set_time_bluetooth();break;
              case 0x32:
                Set_Date_bluetooth();break;
              default:
                Input_Error_Text;break;
              }break;
            }
            else 
              Input_Error_Text;
            COM1.RX_Cnt = 0;
          }
        }
      }
    }
    COM1.RX_Cnt = 0;
  }
}
