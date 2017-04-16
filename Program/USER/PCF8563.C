#include "PCF8563.H"

void init_Time(void)
{
	uchar date[I2C_Read_place];
	I2C_read_Burst_mode(&date, 0x02, I2C_Read_place);
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓时间初始化↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	CurrentTime.Second = bcd_conver_hex(date[0]);
	CurrentTime.Minute = bcd_conver_hex(date[1]);
	CurrentTime.Hour   = bcd_conver_hex(date[2]&0x3F);
	CurrentTime.Day    = bcd_conver_hex(date[3]&0x3F);
	CurrentTime.Week   = bcd_conver_hex(date[4]&0x07);
	CurrentTime.Month  = bcd_conver_hex(date[5]&0x1F);
	CurrentTime.Year   = bcd_conver_hex(date[6]);
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	
	nong_Month = CurrentTime.Month;
	nong_Day = CurrentTime.Day;
	GetChinaCalendar(CurrentTime.Year,	&nong_Month,	&nong_Day);
}

//读取判别翻页，TEMP_UPDATE控制
void I2C_TIME(void)
{
	uchar date[I2C_Read_place];
	if(Display_mode == 3 & TEMP_UPDATE >3)return;
	else if(Display_mode != 3 & TEMP_UPDATE !=0)return;//当翻页速度慢过1s的话需要判断进位完成
	I2C_read_Burst_mode(&date, 0x02, I2C_Read_place);
	
	//↓↓↓↓↓↓↓↓↓↓↓转换成16进制↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	date[0] = bcd_conver_hex(date[0]);
	date[1] = bcd_conver_hex(date[1]);
	date[2] = bcd_conver_hex(date[2]&0x3F);
	date[3] = bcd_conver_hex(date[3]&0x3F);
	date[4] = bcd_conver_hex(date[4]&0x07);
	date[5] = bcd_conver_hex(date[5]&0x1F);
	date[6] = bcd_conver_hex(date[6]);
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	
	//**********判断秒进位*********
	if(CurrentTime.Second%10 != date[0]%10)
	{
		if(CurrentTime.Second/10 != date[0]/10)
		{
			CurrentTime.Second = date[0];
			TEMP_UPDATE = 2;
			//**********判断分进位*********	
			if(CurrentTime.Minute%10 != date[1]%10)
			{ 		
				if(CurrentTime.Minute/10 != date[1]/10)
				{
					CurrentTime.Minute = date[1];
					TEMP_UPDATE = 4;
					//**********判断小时进位*********
					if(CurrentTime.Hour%10 != (date[2])%10)
					{	
						if(CurrentTime.Hour/10 != date[2]/10)
						{
							CurrentTime.Hour = date[2];
							TEMP_UPDATE = 6;
							//**********判断天进位*********
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

//写入时钟数据
void I2C_TIME_Write(void)
{
	uchar date[I2C_Write_Time_Place];

	date[0] = hex_convert_bcd(CurrentTime.Second);
	date[1] = hex_convert_bcd(CurrentTime.Minute);
	date[2] = hex_convert_bcd(CurrentTime.Hour);

	I2C_write_Burst_mode(&date, 0x02, I2C_Write_Time_Place);
}

//写入日期数据
void I2C_DAY_Write(void)
{
	uchar date[I2C_Write_DAY_Place];

	date[0] = hex_convert_bcd(CurrentTime.Day);
	date[1] = hex_convert_bcd(CurrentTime.Week);
	date[2] = hex_convert_bcd(CurrentTime.Month);
	date[3] = hex_convert_bcd(CurrentTime.Year);

	I2C_write_Burst_mode(&date, 0x05, I2C_Write_DAY_Place);
}

//*功能: BCD码转16进制                
//*参数: bcd_data  要转换的BCD码数据(0-100)       
//*返回: 转换后的16进制数据  
uchar bcd_conver_hex(uchar bcd_data)
{
	return  ((bcd_data>>4)*10)+(bcd_data&0x0F);
}

//*名称：  unsigned char hex_convert_bcd(unsigned char hex_dataa)      
//*功能：  16进制转BCD码               
//*参数：  hex_data  要转换的16进制数据(0-100)      
//*返回：  转换后的BCD码数据   
unsigned char hex_convert_bcd(unsigned char hex_data)  
{	
	unsigned char temp;  
	temp=hex_data%100;
	return (temp/10<<4)|(temp%10);
}       