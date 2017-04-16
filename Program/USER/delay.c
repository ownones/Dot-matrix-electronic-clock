
/*************	功能说明	**************

本文件为STC15xxx系列的端口初始化程序,用户几乎可以不修改这个程序.


******************************************/


#include	"delay.h"




//========================================================================
// 函数: void  delay_ms(unsigned int ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数, 这里只支持1~255ms. 自动适应主时钟.
// 返回: none.
// 版本: VER1.0
// 日期: 2013-4-1
// 备注: 
//========================================================================
void  Delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void Delay_us(unsigned int us)		//@22.1184MHz
{
	unsigned char i;
	do
	{
	i = 3;
	while (--i);
	}while(--us);
}

