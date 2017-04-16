#include "PCF8563.H"

void init_Time(void)
{
	uchar date[I2C_Read_place];
	I2C_read_Burst_mode(&date, 0x02, I2C_Read_place);
	//����������������������������ʱ���ʼ������������������������������������
	CurrentTime.Second = bcd_conver_hex(date[0]);
	CurrentTime.Minute = bcd_conver_hex(date[1]);
	CurrentTime.Hour   = bcd_conver_hex(date[2]&0x3F);
	CurrentTime.Day    = bcd_conver_hex(date[3]&0x3F);
	CurrentTime.Week   = bcd_conver_hex(date[4]&0x07);
	CurrentTime.Month  = bcd_conver_hex(date[5]&0x1F);
	CurrentTime.Year   = bcd_conver_hex(date[6]);
	//��������������������������������������������������������������������������������
	
	nong_Month = CurrentTime.Month;
	nong_Day = CurrentTime.Day;
	GetChinaCalendar(CurrentTime.Year,	&nong_Month,	&nong_Day);
}

//��ȡ�б�ҳ��TEMP_UPDATE����
void I2C_TIME(void)
{
	uchar date[I2C_Read_place];
	if(Display_mode == 3 & TEMP_UPDATE >3)return;
	else if(Display_mode != 3 & TEMP_UPDATE !=0)return;//����ҳ�ٶ�����1s�Ļ���Ҫ�жϽ�λ���
	I2C_read_Burst_mode(&date, 0x02, I2C_Read_place);
	
	//����������������������ת����16���ơ���������������������������������
	date[0] = bcd_conver_hex(date[0]);
	date[1] = bcd_conver_hex(date[1]);
	date[2] = bcd_conver_hex(date[2]&0x3F);
	date[3] = bcd_conver_hex(date[3]&0x3F);
	date[4] = bcd_conver_hex(date[4]&0x07);
	date[5] = bcd_conver_hex(date[5]&0x1F);
	date[6] = bcd_conver_hex(date[6]);
	//��������������������������������������������������������������������������������
	
	//**********�ж����λ*********
	if(CurrentTime.Second%10 != date[0]%10)
	{
		if(CurrentTime.Second/10 != date[0]/10)
		{
			CurrentTime.Second = date[0];
			TEMP_UPDATE = 2;
			//**********�жϷֽ�λ*********	
			if(CurrentTime.Minute%10 != date[1]%10)
			{ 		
				if(CurrentTime.Minute/10 != date[1]/10)
				{
					CurrentTime.Minute = date[1];
					TEMP_UPDATE = 4;
					//**********�ж�Сʱ��λ*********
					if(CurrentTime.Hour%10 != (date[2])%10)
					{	
						if(CurrentTime.Hour/10 != date[2]/10)
						{
							CurrentTime.Hour = date[2];
							TEMP_UPDATE = 6;
							//**********�ж����λ*********
							if(CurrentTime.Day  != date[3])
							{
								CurrentTime.Day   =date[3];
								CurrentTime.Week  =date[4];
								CurrentTime.Month =date[5];
								CurrentTime.Year  =date[6];
								return;
							}}
					else{
					CurrentTime.Hour = date[2];
						TEMP_UPDATE = 5;}}}
			else{
			CurrentTime.Minute = date[1];
				TEMP_UPDATE = 3;}}}
	else{
	CurrentTime.Second = date[0];
		TEMP_UPDATE = 1;}}
}

//д��ʱ������
void I2C_TIME_Write(void)
{
	uchar date[I2C_Write_Time_Place];

	date[0] = hex_convert_bcd(CurrentTime.Second);
	date[1] = hex_convert_bcd(CurrentTime.Minute);
	date[2] = hex_convert_bcd(CurrentTime.Hour);

	I2C_write_Burst_mode(&date, 0x02, I2C_Write_Time_Place);
}

//д����������
void I2C_DAY_Write(void)
{
	uchar date[I2C_Write_DAY_Place];

	date[0] = hex_convert_bcd(CurrentTime.Day);
	date[1] = hex_convert_bcd(CurrentTime.Week);
	date[2] = hex_convert_bcd(CurrentTime.Month);
	date[3] = hex_convert_bcd(CurrentTime.Year);

	I2C_write_Burst_mode(&date, 0x05, I2C_Write_DAY_Place);
}

//*����: BCD��ת16����                
//*����: bcd_data  Ҫת����BCD������(0-100)       
//*����: ת�����16��������  
uchar bcd_conver_hex(uchar bcd_data)
{
	return  ((bcd_data>>4)*10)+(bcd_data&0x0F);
}

//*���ƣ�  unsigned char hex_convert_bcd(unsigned char hex_dataa)      
//*���ܣ�  16����תBCD��               
//*������  hex_data  Ҫת����16��������(0-100)      
//*���أ�  ת�����BCD������   
unsigned char hex_convert_bcd(unsigned char hex_data)  
{	
	unsigned char temp;  
	temp=hex_data%100;
	return (temp/10<<4)|(temp%10);
}       