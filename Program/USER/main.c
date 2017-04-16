/*********************************************************************************************��
Dot_matrix electronic clock 
Copyright (C) 2016 ownones <ownones@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

												�������ʱ��
��дʱ�䣺����2013.9.6
Ӳ��֧�֣�����IAP15F2K61S2���ڲ�22.1184����
�ӿ�˵��������led_sca.h��I2C.H��ADC.C�������ȵ�
�޸���־������
����NO.1-20130906_17.09 ��ʼ����
	NO.2-20130907_15.00 ��ɷ�ҳʱ������
	NO.3-20130914_12.13 ��ɹ�����ʱ��С���㡢����ֹͣ����
	NO.4-20130918_21.11 ����Զ����ȵ�������
	NO.5-20130918_21.11 ����ע��
	NO.6-20130923_10.44 ���PCF8563ʱ�������������ֿ��������ҳ������
	NO.7-20130923_11.52 ɾ������ɫ���顢��ʾ,��ɫ�ŵ���ɫ����ɫ������ʾ,ʹ���������ٶȸ���.
	NO.8-20140716_20.00 ���һ���ȥ�ˣ�����߿���ף��¼ȡ�ɹ��ɡ������������������ڸ�ʱ��,���������ˣ������ð������˼��ɵ�·,����IO�ڸ���?
	NO.9-20140809_10.00 �㶨��DH11���ٶ����⣬�¶�ʪ����ʾ�������޸�ADC��ʹ�ùٷ������⡣
	NO.9-201701014_23.38 ͻȻ�����ⶫ��,��Դ��GITHUB��������
	������:  ownones
	
/*********************************************************************************************/


#include "main.h"

void SYSTEM_mode(void)
{
	Shift_Time();			
	Show_list();				//��ʾ���
	I2C_TIME();					//��ʾ���
	Auto_Brightness();	//�Զ�����	
//	dat_dalay = 0;
	DHT11_Start();			//��ʪ�ȴ���
//	dat_dalay = 1;
}

void INIT_SYSTEM(void)
{
	DHT11_Init();
	ADC_config();
	I2C_init();
	init_Time();
  Init_Interrput();
	ALL_Reset();
}

void main(void)
{
	INIT_SYSTEM();
	while(1)
	{
    SYSTEM_mode();
    Check_bluetooth();
	}
}

