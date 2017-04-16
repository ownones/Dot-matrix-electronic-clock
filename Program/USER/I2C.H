#ifndef __I2C_H_
#define __I2C_H_

sbit SDA = P2^6;
sbit SCL = P2^5;

void I2C_start()//I2C开始
{
	SDA=1;
	Delay_us(3);
	SCL=1;
	Delay_us(3);
	SDA=0;
	Delay_us(3);
	SCL=0;
	Delay_us(3);
}

void I2C_stop()//I2C停止
{
	SDA = 0;
	Delay_us(3);
	SCL = 1;
	Delay_us(3);
	SDA = 1;
	Delay_us(3);
}

void I2C_respons()//应答
{
	int i=0;
	SCL = 1;
	Delay_us(3);
	while((SDA==1) && (i<1500))i++;
	SCL = 0;
	Delay_us(3);
}

void I2C_answer()//回答
{
	SDA = 0;
	Delay_us(3);
	SCL = 1;
	Delay_us(3);
	SCL = 0;
	Delay_us(3);
	SDA = 1;
	Delay_us(3);
}

void I2C_write_byte(uchar byte)//I2C写入
{
	uchar i,write;
	write = byte;
	for(i=0; i<8; i++)
	{
		write = write <<1;
		SCL = 0;
		Delay_us(3);
		SDA = CY;
		Delay_us(3);
		SCL = 1;
		Delay_us(3);
	}
	SCL = 0;
	Delay_us(3);
	SDA = 1;
	Delay_us(3);
	return;
}

uchar I2C_read_byte(void)//读出I2C数据
{
	uchar i, read;
	SCL = 0;
	Delay_us(2);
	SDA = 1;
	Delay_us(2);
	for(i=0; i<8; i++)
	{
		SCL = 1;
		Delay_us(2);
		read = (read<<1) | SDA;
		Delay_us(2);
		SCL = 0;
		Delay_us(2);
	}
	return read;
}

void I2C_init(void)//初始化I2C总线
{
	SCL = 1;
	Delay_us(2);
	SDA = 1;
	Delay_us(2);
}

/*
****************************************************************
函 数 名：I2C_write_Burst_mode(uchar *date, address, place)
函数功能：I2C按地址增量写入
入    参：address ：地址， date ：要发送数据, place:写入的位数
出    参：无
修改时间：2013/9/22
定    义：i
全局定义：无
备    注：
****************************************************************/
void I2C_write_Burst_mode(uchar *date, address, place)
{
	uchar i;
	I2C_start();
	I2C_write_byte(0XA2);   //Device Addres
	I2C_respons();
	I2C_write_byte(address);//数据地址
	I2C_respons();
	for(i=0; i<place; i++){//增量地址再写
	I2C_write_byte(date[i]);  //发送数据
	I2C_respons();}//应答
	I2C_stop();	
}

/*
****************************************************************
函 数 名：I2C_read_Burst_mode(uchar *date, address, place)
函数功能：I2C连续读取
入    参：address：读的第一位地址，place：读的位数
修改时间：2013/9/22
定    义：i；连读进位
全局定义：无
备    注：			注意：读写完成要有时间释放总线
****************************************************************/
void I2C_read_Burst_mode(uchar *date, address, place)//address ：地址， date ：接收的数据，place：要读的位数
{
	uchar i;
	I2C_start();
	I2C_write_byte(0XA2);//Device Addres
	I2C_respons();
	I2C_write_byte(address);   //数据地址
	I2C_respons();
	I2C_start();
	I2C_write_byte(0XA3);//Device Addres
	I2C_respons();
	for(i=0; i<place; i++){//增量地址再读
	date[i] = I2C_read_byte();//接收数据
	I2C_answer();}//应答
	I2C_stop();
}
#endif