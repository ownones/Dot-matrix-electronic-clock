#include "DHT11.H"
uint8 Er1 = 0;
sbit led = P3^2;



void DHT11_Init(void)
{
   DAT_DHT11 = 1;
}


void DHT11_Read(uint8 *TEMP) //��ȡ��
{
  uint8 i, j, FLAG, TMP;
  
//	for(j=0; j<5; j++)  //��Ϊ��ȡȫ����Ҫ4ms,�ٶ�ȡ2λ8bit���ݿ��Լ���1.5msʱ��
  for(j=0; j<3; j++)
    for(i=0; i<8; i++)
    {
      while((!DAT_DHT11)&&FLAG++)
        if(FLAG>100)return; //��ȡ��ʱ
      FLAG = 1;
      TMP = 0;
			
      Delay_us(30);
			
      if(DAT_DHT11 == 1)
      {
        TMP = 1;
        while(DAT_DHT11&&FLAG++)
        if(FLAG>100)return; //��ȡ��ʱ
        FLAG = 1;
      }
      TEMP[j] <<= 1;
      TEMP[j] |= TMP;
    }
		DAT_DHT11 = 1;
}

void DHT11_Start(void) //��ȡ��ʼ
{
	uint8 TEMP[5];		//����ָ����ֵ
	uint8 FLAG = 0;
	if(INT_DHT11 < 2)return;//��ȡƵ�� 10S/��
	
	
	
	if(START_DHT11 == 0){START_DHT11 = 1;	DAT_DHT11 = 0;}
	if(START_DHT11 < 18)return; //�ж���ʱ18MS
  START_DHT11 = 0;//ֹͣ��ʱ
	
  DAT_DHT11 = 1;


  Delay_us(50);
  while((!DAT_DHT11) && FLAG++)
    if(FLAG>100)return;//��ȡ��ʱ
  FLAG = 1;
  
  Delay_us(40);
  while(DAT_DHT11 && FLAG++)
    if(FLAG>100)return; //��ȡ��ʱ
  FLAG = 1;
	
	
  DHT11_Read(&TEMP);
	
/*	//��Ϊ��ȡȫ����Ҫ4ms,�ٶ�ȡ2λ8bit���ݿ��Լ���1.5msʱ��,����Ҫ��ȡ�����Σ������޸ĺ��
	if(TEMP[4] == TEMP[0] + TEMP[2])//return; //У���������Ϊ����������λ������Ч
	{
		Humidity = TEMP[0];
		Temperature = TEMP[2];
	}
	
	else 
		Er1 += 1;
*/
	

	if(Humidity < 0 && Humidity > 100 && Temperature >50)//�ж���ʪ����ȷ
		return;
	
	Humidity = TEMP[0];
	Temperature = TEMP[2];

	INT_DHT11 = 0;  //��ȡƵ�����¼���
}