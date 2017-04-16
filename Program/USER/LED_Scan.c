#include "LED_Scan.h"
/*
****************************************************************
关于show_R[16][16]; show_G[16][16];显示数组的定义

	本点阵是由8个一排，4个一列
	
			1*1代表8*4点阵中第一排第一列的一颗
			1*2代表8*4点阵中第一排第二列的一颗
			2*2代表8*4点阵中第二排第二列的一颗
			8*4代表8*4点阵中第八排第四列的一颗
			
			由于是列扫描，所以列代表数组的8bit
	
	show_R[0][0];是位置1*1点阵中第一排第一列
	show_R[1][0];是位置1*1点阵中第二排第一列
	
	show_R[0][1];是位置2*1点阵中第一排第一列
	show_R[0][2];是位置3*1点阵中第一排第一列
	
	show_R[8][0];是位置1*2点阵中第一排第一列
	show_R[16][0];是位置1*2点阵中第八排第一列
	
	show_R[8][1];是位置2*2点阵中第一排第一列
	show_R[9][1];是位置2*2点阵中第二排第一列
	show_R[9][2];是位置3*2点阵中第二排第一列
****************************************************************/
/*
****************************************************************
函 数 名：void Show_row(uint8 disrow)
函数功能：行扫描显示
入    参：disrow
控制函数：void Show_list(void)
出    参：无
修改时间：2013/8/30
定    义：disrow：扫描到的行；i：每行的一个595寄存器；
		：j：595当个字节写入；k：扫描的颜色；
全局定义：show_R,show_G,show_O:要显示的数组
备    注：
****************************************************************/
void Show_row(uint8 disrow)
{
	uint8 i, j, k;
	uint8 tmp_R1, tmp_R2,tmp_G1, tmp_G2;//定义写入缓存
	
	for(k=0; k<2; k++){
	for(i=0; i<8; i++){
		if(k==0)	{//写入红色并显示
		tmp_R1 = show_R[disrow][i];
		tmp_R2 = show_R[disrow][i+8];
		tmp_G1 = 0x00;tmp_G2 = 0x00;}
		
		else if(k==1){//写入绿色并显示
		tmp_G1 = show_G[disrow][i];
		tmp_G2 = show_G[disrow][i+8];
		tmp_R1 = 0x00;tmp_R2 = 0x00;}
		
		for (j=0; j<8; j++)
		{
			DR1 = (bit)(~tmp_R1&0x80);
			DR2 = (bit)(~tmp_R2&0x80);
			DG1 = (bit)(~tmp_G1&0x80);
			DG2 = (bit)(~tmp_G2&0x80);
      SCK = 0;
      SCK = 1;
			tmp_R1<<=1;
			tmp_R2<<=1;
			tmp_G1<<=1;
			tmp_G2<<=1;
		}}
	HC595_out();
	HEF4094_EN_Enable();//开显示
	if(k == 0)Delay_us(Delay_Red);//红色亮度
	else if(k == 1)Delay_us(Delay_Green);//绿色亮度
	HEF4094_EN_Disable();//关显示
	}}

/*
****************************************************************
函 数 名：void Show_list(void)
函数功能：列扫描
入    参：无
出    参：无
修改时间：2013/8/30
定    义：disrow：扫描到的行；j：移动显示行的位置
全局定义：无
备    注：
****************************************************************/
void Show_list(void)
{
	uint8 disrow, j;
	uint16 tmp;
	HEF4094_EN_Disable();

	for(disrow=0; disrow<16; disrow++)
	{
		tmp = 0x01;
		tmp <<= disrow;
		for(j=0; j<16; j++)
		{
			CLK = 0;
			DAT = (bit)(tmp&0x8000);
			CLK = 1;
			tmp<<=1;
		}
		Show_row(disrow);
	}
}