#ifndef __I2C_H_
#define __I2C_H_

sbit SDA = P2^6;
sbit SCL = P2^5;

void I2C_start()//I2C��ʼ
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

void I2C_stop()//I2Cֹͣ
{
	SDA = 0;
	Delay_us(3);
	SCL = 1;
	Delay_us(3);
	SDA = 1;
	Delay_us(3);
}

void I2C_respons()//Ӧ��
{
	int i=0;
	SCL = 1;
	Delay_us(3);
	while((SDA==1) && (i<1500))i++;
	SCL = 0;
	Delay_us(3);
}

void I2C_answer()//�ش�
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

void I2C_write_byte(uchar byte)//I2Cд��
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

uchar I2C_read_byte(void)//����I2C����
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

void I2C_init(void)//��ʼ��I2C����
{
	SCL = 1;
	Delay_us(2);
	SDA = 1;
	Delay_us(2);
}

/*
****************************************************************
�� �� ����I2C_write_Burst_mode(uchar *date, address, place)
�������ܣ�I2C����ַ����д��
��    �Σ�address ����ַ�� date ��Ҫ��������, place:д���λ��
��    �Σ���
�޸�ʱ�䣺2013/9/22
��    �壺i
ȫ�ֶ��壺��
��    ע��
****************************************************************/
void I2C_write_Burst_mode(uchar *date, address, place)
{
	uchar i;
	I2C_start();
	I2C_write_byte(0XA2);   //Device Addres
	I2C_respons();
	I2C_write_byte(address);//���ݵ�ַ
	I2C_respons();
	for(i=0; i<place; i++){//������ַ��д
	I2C_write_byte(date[i]);  //��������
	I2C_respons();}//Ӧ��
	I2C_stop();	
}

/*
****************************************************************
�� �� ����I2C_read_Burst_mode(uchar *date, address, place)
�������ܣ�I2C������ȡ
��    �Σ�address�����ĵ�һλ��ַ��place������λ��
�޸�ʱ�䣺2013/9/22
��    �壺i��������λ
ȫ�ֶ��壺��
��    ע��			ע�⣺��д���Ҫ��ʱ���ͷ�����
****************************************************************/
void I2C_read_Burst_mode(uchar *date, address, place)//address ����ַ�� date �����յ����ݣ�place��Ҫ����λ��
{
	uchar i;
	I2C_start();
	I2C_write_byte(0XA2);//Device Addres
	I2C_respons();
	I2C_write_byte(address);   //���ݵ�ַ
	I2C_respons();
	I2C_start();
	I2C_write_byte(0XA3);//Device Addres
	I2C_respons();
	for(i=0; i<place; i++){//������ַ�ٶ�
	date[i] = I2C_read_byte();//��������
	I2C_answer();}//Ӧ��
	I2C_stop();
}
#endif