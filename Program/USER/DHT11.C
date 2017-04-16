#include "DHT11.H"
uint8 Er1 = 0;
sbit led = P3^2;



void DHT11_Init(void)
{
   DAT_DHT11 = 1;
}


void DHT11_Read(uint8 *TEMP) //读取中
{
  uint8 i, j, FLAG, TMP;
  
//	for(j=0; j<5; j++)  //因为读取全部需要4ms,少读取2位8bit数据可以减少1.5ms时间
  for(j=0; j<3; j++)
    for(i=0; i<8; i++)
    {
      while((!DAT_DHT11)&&FLAG++)
        if(FLAG>100)return; //读取超时
      FLAG = 1;
      TMP = 0;
			
      Delay_us(30);
			
      if(DAT_DHT11 == 1)
      {
        TMP = 1;
        while(DAT_DHT11&&FLAG++)
        if(FLAG>100)return; //读取超时
        FLAG = 1;
      }
      TEMP[j] <<= 1;
      TEMP[j] |= TMP;
    }
		DAT_DHT11 = 1;
}

void DHT11_Start(void) //读取开始
{
	uint8 TEMP[5];		//后期指针数值
	uint8 FLAG = 0;
	if(INT_DHT11 < 2)return;//读取频率 10S/次
	
	
	
	if(START_DHT11 == 0){START_DHT11 = 1;	DAT_DHT11 = 0;}
	if(START_DHT11 < 18)return; //中断延时18MS
  START_DHT11 = 0;//停止计时
	
  DAT_DHT11 = 1;


  Delay_us(50);
  while((!DAT_DHT11) && FLAG++)
    if(FLAG>100)return;//读取超时
  FLAG = 1;
  
  Delay_us(40);
  while(DAT_DHT11 && FLAG++)
    if(FLAG>100)return; //读取超时
  FLAG = 1;
	
	
  DHT11_Read(&TEMP);
	
/*	//因为读取全部需要4ms,少读取2位8bit数据可以减少1.5ms时间,如需要请取消屏蔽，屏蔽修改后的
	if(TEMP[4] == TEMP[0] + TEMP[2])//return; //校验读出，因为不读出后两位所以无效
	{
		Humidity = TEMP[0];
		Temperature = TEMP[2];
	}
	
	else 
		Er1 += 1;
*/
	

	if(Humidity < 0 && Humidity > 100 && Temperature >50)//判断温湿度正确
		return;
	
	Humidity = TEMP[0];
	Temperature = TEMP[2];

	INT_DHT11 = 0;  //读取频率重新计数
}
