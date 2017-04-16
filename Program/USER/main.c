/*********************************************************************************************　
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

												点阵电子时钟
编写时间：　　2013.9.6
硬件支持：　　IAP15F2K61S2　内部22.1184晶振
接口说明：　　led_sca.h、I2C.H、ADC.C。。。等等
修改日志：　　
　　NO.1-20130906_17.09 开始制作
	NO.2-20130907_15.00 完成翻页时钟制作
	NO.3-20130914_12.13 完成滚动、时钟小数点、滚动停止制作
	NO.4-20130918_21.11 完成自动亮度调节制作
	NO.5-20130918_21.11 更改注释
	NO.6-20130923_10.44 完成PCF8563时钟制作，修正字库溢出、翻页完成溢出
	NO.7-20130923_11.52 删除橙颜色数组、显示,橙色放到红色和绿色数组显示,使整体运行速度更快.
	NO.8-20140716_20.00 差不多一年过去了，考完高考。祝我录取成功吧。。。增加了蓝牙串口改时间,把面包板拆了，用万用版制作了集成电路,所以IO口改了?
	NO.9-20140809_10.00 搞定了DH11的速度问题，温度湿度显示正常。修改ADC，使用官方函数库。
	NO.9-201701014_23.38 突然翻出这东西,开源到GITHUB。。。。
	作　者:  ownones
	
/*********************************************************************************************/


#include "main.h"

void SYSTEM_mode(void)
{
	Shift_Time();			
	Show_list();				//显示描绘
	I2C_TIME();					//显示输出
	Auto_Brightness();	//自动亮度	
//	dat_dalay = 0;
	DHT11_Start();			//温湿度传感
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

