#include "LED_Shift.h"

void Shift_Time(void)
{
	uint8 i,j;
	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
		{
			show_R[i][j] = 0;
//			show_G[i][j] = 0;
		}
	
	
	
	Page_Show();
	if(Display_mode == 2 || Display_mode == 3)
	{
		Real_time_Static();
		Static_display();
	}
	else if(Display_mode == 1)
	{
		Roll_wait();
		Roll_Show();
	}
	
	dot_show();
	
}
/*
****************************************************************
函 数 名：void Page_Time(void)
函数功能：翻页翻页显示
入    参：无
出    参：无
修改时间：2013/9/23
定    义：row正在刷新的排,tmp要输出数字，i和j循环用。
全局定义：TEMP_UPDATE翻页标志位，Page_speed--翻页速度,
		：Fposition翻页中行走的位置，Colour_Time显示颜色；
备    注：
****************************************************************/
void Page_Show(void)
{
	uint8 tmp, row, i, j;
	if(Display_mode == 3 & TEMP_UPDATE < 3)return;//不翻秒
	else if(TEMP_UPDATE == 0)return;//是否需要翻页
	
//	if(Page_speed--  >0)return;//检测翻页延时
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	for(j=TEMP_UPDATE; j>0; j--){//要翻页的数量，循环翻页
	if(Display_mode == 3 & j < 3)break;//不翻秒
	switch(j)//确认行位置
	{	case 6: row =0;break;
		case 5: row =1;break;
		case 4: row =3;break;
		case 3: row =4;break;
		case 2: row =6;break;
		case 1: row =7;break;
	}
	switch (row)
	{	case 0: tmp = CurrentTime.Hour/10;  break;
		case 1: tmp = CurrentTime.Hour%10;  break;
		case 3: tmp = CurrentTime.Minute/10;break;
		case 4: tmp = CurrentTime.Minute%10;break;
		case 6: tmp = CurrentTime.Second/10;break;
		case 7: tmp = CurrentTime.Second%10;break;
	}
	switch (Colour_Time)//确定翻页颜色
	{
		case 3:
			for(i=0; i<16; i++){
				show_R[i][row] = show_R[i+1][row];//数组上移
				show_G[i][row] = show_G[i+1][row];}//数组上移
				show_R[15][row] = Num_Time[tmp][Fposition];//新数字补齐最后一行
				show_G[15][row] = Num_Time[tmp][Fposition];//新数字补齐最后一行
		break;
		case 2:
			for(i=0; i<16; i++)
				show_G[i][row] = show_G[i+1][row];
				show_G[15][row] = Num_Time[tmp][Fposition];
		break;
		case 1:
			for(i=0; i<16; i++)
				show_R[i][row] = show_R[i+1][row];
				show_R[15][row] = Num_Time[tmp][Fposition];
		break;
	}}
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	
	if(Fposition==15){//检测翻页是否完成
	TEMP_UPDATE = 0;//清楚翻页置位
	Fposition   = 0;//重置翻页
	return;}
	
	else{
	Fposition  += 1;}//继续翻页
//	Page_speed  = 1;//重新计时
}


/*
****************************************************************
函 数 名：
void dot_Write_bit(
		unsigned	char  Write_bit,	//要写多少位，站定最多8bit
		unsigned	char  color,			//写这个字的颜色		1：红色 2：绿色 3：黄色
		unsigned	char  *Datt,			//要写的字用一维数组指针指代
		unsigned	char  *Location,  //写到哪个点阵的哪个列
		unsigned	char  *start)			//写哪个点阵
函数功能：静态显示写入数组
入    参：无
出    参：无
修改时间：2014/9/3，09.24
备    注：
****************************************************************/


void dot_Write_bit(
		unsigned	char  Write_bit,
		unsigned	char  color,
		unsigned	char  *Datt,
		unsigned	char  *Location, 
		unsigned	char  *start)
{
	uint8 i, j, tmp_L, tmp_W, tmp, clear;
	uint8 temp[8];
	uint8 Data[8];	
	if(*start <7){tmp_L = *start;		tmp_W = 0;}		//判断位置，详细请看 显示数组的定义
	else if(*start >7 && *start <16){tmp_L = *start-7;		tmp_W = 8;}
	else if(*start >15 && *start <24){tmp_L = *start-8;		tmp_W = 0;}
	else if(*start >23 && *start <32){tmp_L = *start-16;		tmp_W = 8;}

	for(i=0; i<8; i++)		//由于指针指向的是xdata不是内存不可修改所以把里面的数据转移到内存
		Data[i] = Datt[i];


	//判断是否越格
	if(Write_bit + *Location > 8)
	{	
		tmp = 0xFF;
		j = 8 - *Location;
		tmp <<= *Location;

		for(i=0; i<8; i++)	//越格后把越格后的移到下一格，没越格的放在现在的那一格。
		{
			temp[i] = Data[i] &tmp;
			temp[i] >>= *Location;
			Data[i] <<= j;		//由于现在的那个格子前面已经写入了数据，所以写到后面的空位。
		}
/////////////////////////////////////////////////

		clear = 0XFF;
		clear >>= *Location;
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_R[i][tmp_L+1] &= clear;
			show_G[i][tmp_L+1] &= clear;
		}
	
	clear = 0XFF;
	clear <<= (8-*Location);
		
	j = 0;
	for(i=tmp_W; i<8+tmp_W; i++)
	{
		show_R[i][tmp_L] &= clear;
		show_G[i][tmp_L] &= clear;
		j++;
	}

///////////////////////////////////////
		
		j = 0;
		//写入数据
		if(color == 1 || color == 3)
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_R[i][tmp_L] |= temp[j];
			show_R[i][tmp_L+1] |= Data[j];
			j++;
		}
		
		j = 0;
		if(color == 2 || color == 3)
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_G[i][tmp_L] |= temp[j];
			show_G[i][tmp_L+1] |= Data[j];
			j++;
		}
		
			
		*start += 1;				//写完后格子+1，下次位置定好。
		*Location = Write_bit - (8 - *Location);
			
		return;
		
	}
	
	
			//判断是否写不满8位						//或者刚好写满点阵
	if(Write_bit + *Location < 8 || Write_bit + *Location == 8)
		for(i=0; i<8; i++)
			temp[i] = Data[i] >> *Location;
	
	clear = 0XFF;
	clear <<= (8-*Location);
		
	j = 0;
	for(i=tmp_W; i<8+tmp_W; i++)
	{
		show_R[i][tmp_L] &= clear;
		show_G[i][tmp_L] &= clear;
		j++;
	}
	
	
	//写入数据//颜色判定写入
	
	j = 0;
	if(color == 1)
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_R[i][tmp_L] |= temp[j];
			j++;
		}
	
	else if(color == 2)
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_G[i][tmp_L] |= temp[j];
			j++;
		}
		
	else if(color == 3)
		for(i=tmp_W; i<8+tmp_W; i++)
		{
			show_R[i][tmp_L] |= temp[j];
			show_G[i][tmp_L] |= temp[j];
			j++;
		}
	
	
	//地址自增
	
	if(Write_bit + *Location < 8)
		*Location += Write_bit;
	else if(Write_bit + *Location == 8)
		{*start += 1; 	*Location = 0;}
		

}


/*
****************************************************************
函 数 名：void Static_display(void)
函数功能：静态数组显示排版
入    参：无
出    参：无
修改时间：2014/9/3，09.24
备    注：
****************************************************************/
void Static_display(void)
{
//	unsigned	char i;
	unsigned	char Location;//偏移量
	unsigned	char start;		//点阵位置
	unsigned	char color_nong_Day = 1; 
	unsigned 	char color_new_Day  = 3;
	
	
////显示农历信息
	
	Location = 0;
	start = 16;
	dot_Write_bit(8, 1, &Static_word[4], &Location, &start);
	
	//农历月
	{
		if(nong_Month < 10)
		{
			dot_Write_bit(7, color_nong_Day, &Static_6bit[0], &Location, &start);
			dot_Write_bit(6, color_nong_Day, &Static_6bit[nong_Month], &Location, &start);//
		}
		
		else if(nong_Month == 10)
		{
			dot_Write_bit(6, color_nong_Day, &Static_6bit[0], &Location, &start);
			dot_Write_bit(7, color_nong_Day, &Static_word[2], &Location, &start);//
		}
		else 
		{
			dot_Write_bit(7, color_nong_Day, &Static_word[2], &Location, &start);
			dot_Write_bit(6, color_nong_Day, &Static_6bit[nong_Month%10], &Location, &start);//
		}
		dot_Write_bit(1, color_nong_Day, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(6, color_nong_Day, &Static_6bit[12], &Location, &start);//月
	}
	
	//农历日
	{
		if(nong_Day == 1){
			dot_Write_bit(8, 3, &Static_word[0], &Location, &start);}
		else if(nong_Day == 15)
			dot_Write_bit(7, 3, &Static_word[2], &Location, &start);
		else if(nong_Day > 1 && nong_Day <= 10)
			dot_Write_bit(8, color_nong_Day, &Static_word[0], &Location, &start);
		else if(nong_Day > 10 && nong_Day <= 19)
			dot_Write_bit(7, color_nong_Day, &Static_word[2], &Location, &start);
		else if(nong_Day > 19 && nong_Day <= 29)
			dot_Write_bit(8, color_nong_Day, &Static_word[3], &Location, &start);
		else if(nong_Day > 29 && nong_Day <= 31)
			dot_Write_bit(8, color_nong_Day, &Static_word[1], &Location, &start);
		
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		
		if(nong_Day == 10 || nong_Day == 20 || nong_Day == 30)
			dot_Write_bit(7, color_nong_Day, &Static_word[2], &Location, &start);//	
		else if(nong_Day == 1)
			dot_Write_bit(6, 3, &Static_6bit[1], &Location, &start);
		else if(nong_Day == 15)
			dot_Write_bit(6, 3, &Static_6bit[5], &Location, &start);
		else
			dot_Write_bit(6, color_nong_Day, &Static_6bit[nong_Day %10], &Location, &start);//		

	}
	
////显示新历信息
	Location = 0;
	start = 24;
	
	//新历年20xx
	dot_Write_bit(4, color_new_Day, &Static_4bit[2], &Location, &start);//2
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	dot_Write_bit(4, color_new_Day, &Static_4bit[0], &Location, &start);//0
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Year/10], &Location, &start);//X
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Year%10], &Location, &start);//X
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	dot_Write_bit(6, 1, &Static_6bit[11], &Location, &start);//年
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	
	//新历月
	
	if(CurrentTime.Month <10)
	{
		dot_Write_bit(5, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	}
	else 
	{
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month%10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	}
	dot_Write_bit(6, 1, &Static_6bit[12], &Location, &start);//月
	
	//新历日
	if(CurrentTime.Day < 10)
	{
		dot_Write_bit(5, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day], &Location, &start);//x
	}
	else
	{
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day%10], &Location, &start);//x
	}
	dot_Write_bit(6, 1, &Static_6bit[13], &Location, &start);//日
	
	
	//星期
	
	if(CurrentTime.Week == 0)
		dot_Write_bit(6, 3, &Static_6bit[14], &Location, &start);//x
	else
		dot_Write_bit(6, 3, &Static_6bit[CurrentTime.Week], &Location, &start);//x
	
	
	
}

void Real_time_Static(void)
{
	unsigned	char Location;//偏移量
	unsigned	char start;		//点阵位置
	
	Location = 7;
	start = 21;
	
	//显示温度，2秒跟新
	if(Temperature > 10)
	{
		dot_Write_bit(5, 2, &Static_5bit[Temperature/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(5, 2, &Static_5bit[Temperature%10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	}
	else
	{
		dot_Write_bit(6, 1, &Static_6bit[0], &Location, &start);//空格
		dot_Write_bit(5, 2, &Static_5bit[Temperature], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
	}
	
	dot_Write_bit(4, 2, &Static_5bit[10], &Location, &start);//度
	
	//显示湿度，2秒更新
	if(Display_mode == 3)
	{
		Location = 7;
		start = 12;
		
		if(Humidity > 10)
		{
			dot_Write_bit(5, 2, &Static_5bit[Humidity/10], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
			dot_Write_bit(5, 2, &Static_5bit[Humidity%10], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		}
		else 
		{
			dot_Write_bit(6, 1, &Static_6bit[0], &Location, &start);//空格
			dot_Write_bit(5, 2, &Static_5bit[Humidity], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//空格
		}
	
	dot_Write_bit(4, 2, &Static_5bit[11], &Location, &start);//x
	}
		
}

/*
****************************************************************
函 数 名：void Roll_Show(void)
函数功能：滚动显示
入    参：无
出    参：无
修改时间：2013/9/13，17.27
定    义：temp：输出使用的缓存
备    注：
****************************************************************/
void Roll_Show(void)
{
	uint16 temp;
	if(Roll_Stop ==0)return;//停止滚动标志位
	if(Roll_speed-- >0)return;//滚动速度
	
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓滚动下一个字符↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	if(Rposition_n ==0)
	{
		Rposition +=1;
		if(Rposition>sizeof(queue)/3-1)//数组的滚动个数
			Rposition=0;
		Rposition_n = queue[Rposition][2];
	}
	
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓更改的日期↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	if(queue[Rposition][0] ==0)
	{
		if(queue[Rposition][1] != 0)
			D_Roll(queue[Rposition][1]);}
	
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓滚动显示一排↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	if(queue[Rposition][2] == 0)temp = 0;//空数据
	else if(queue[Rposition][2] == 16)//此长度是显示数字
	{
		temp = Nun_Roll[queue[Rposition][1]][16-Rposition_n];
		temp <<=8;
		temp |= Nun_Roll[queue[Rposition][1]][17-Rposition_n];
	}
	else if(queue[Rposition][2] == 32)//此长度是显示文字
	{
		temp = zhif_song[queue[Rposition][1]][32-Rposition_n];
		temp <<=8;
		temp |= zhif_song[queue[Rposition][1]][33-Rposition_n];
	}
	Rposition_n -=2;//缓存制作完成后准备制作下个
	Roll_char(queue[Rposition][0],temp);//输出显示文字
	Roll_speed = Rspeed;//速度控制
}
/*
****************************************************************
函 数 名：void Roll_char(uint8 Coloer, uint16 zhifu16)
函数功能：滚动单个字符输出
入    参：Coloer，zhifu16
出    参：无
控制函数：void Roll_Show(void)
修改时间：2013/9/8，10.20
定    义：Coloer：颜色，zhifu16最右边准备移出字符
全局定义：show_R;show_G;show_O;	
备    注：
****************************************************************/

void Roll_char(uint8 Coloer, uint16 zhifu16)
{
	uint8 i, j;	
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓整列位左移↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

		for(i=0;i<16;i++){//一列16位
		show_R[i][8]<<=1;//移出显示
		show_G[i][8]<<=1;
		for(j=0;j<7;j++)//一排8位
		{
			if((bit)(show_R[i][j+9]&0x80))
				show_R[i][j+8] |= 0x01;
			if((bit)(show_G[i][j+9]&0x80))
				show_G[i][j+8] |= 0x01;
			show_R[i][j+9] <<=1;
			show_G[i][j+9] <<=1;
		}}
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓写入最后一排↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	
	switch(Coloer)//颜色检测
	{
	case 1:
		for(i=0;i<16;i++){
		if((bit)(zhifu16&0x8000))
		show_R[i][15] |= 0x01;
		zhifu16 <<=1;
		}break;
	case 2:
		for(i=0;i<16;i++){
		if((bit)(zhifu16&0x8000))
		show_G[i][15] |= 0x01;
		zhifu16 <<=1;
		}break;
	case 3:
		for(i=0;i<16;i++){
		if((bit)(zhifu16&0x8000)){
		show_R[i][15] |= 0x01;
		show_G[i][15] |= 0x01;}
		zhifu16 <<=1;
		}break;}
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
}
/*
****************************************************************
函 数 名：void D_Roll(uint8 i)
函数功能：改变滚动输出数据（相当于日期改变）
入    参：i
出    参：无
控制函数：void Roll_Show(void)
修改时间：2013/9/8，10.20
定    义：
全局定义：Rposition：输出的数字，Rposition_n正在输出排的位置；
备    注：
****************************************************************/
void D_Roll(uint8 i)
{
	Rposition += 1;
	Rposition_n = 16;
	switch(i)
	{	
		case 1:
			queue[Rposition][1] = CurrentTime.Year/10;
			queue[Rposition+1][1] = CurrentTime.Year%10;break;
		case 2:
			queue[Rposition][1] = CurrentTime.Month/10;
			queue[Rposition+1][1] = CurrentTime.Month%10;break;
		case 3:
			queue[Rposition][1] = CurrentTime.Day/10;
			queue[Rposition+1][1] = CurrentTime.Day%10;break;
    case 4:
      Rposition_n = 32;
      if(CurrentTime.Week == 0)
        queue[Rposition][1] = 11;
      else
        queue[Rposition][1] = CurrentTime.Week-1;break;
		case 5:
			queue[Rposition][1] = Temperature/10;
			queue[Rposition+1][1] = Temperature%10;break;
		case 6:
			queue[Rposition][1] = Humidity/10;
			queue[Rposition+1][1] = Humidity%10;break;
		case 7:
			Rposition_n = 32;
			if(nong_Month < 11){
				Rposition += 1;
				queue[Rposition][1] = nong_Month-1;}
			else{
				queue[Rposition][1] = 9;
				queue[Rposition+1][1] = nong_Month-10-1;}break;
		case 8:
		{
			Rposition_n = 32;
			if(nong_Day < 11){
				queue[Rposition][1] = 24;
				queue[Rposition+1][1] = nong_Day;}
			
			else if(nong_Day > 10 && nong_Day < 20){
				queue[Rposition][1] = 9;
				queue[Rposition+1][1] = nong_Day-10;}
			
			else if(nong_Day == 20){
				queue[Rposition][1] = 25;
				queue[Rposition+1][1] = 9;}
			
			else if(nong_Day > 19 && nong_Day < 30){
				queue[Rposition][1] = 25;
				queue[Rposition+1][1] = nong_Day-20-1;}
			
			else if(nong_Day == 30){
				queue[Rposition][1] = 2;
				queue[Rposition+1][1] = 9;}
			
			else{
				queue[Rposition][1] = 2;
				queue[Rposition+1][1] = nong_Day-30-1;}
			}break;
	}

}
/*
****************************************************************
函 数 名：void Roll_wait(void)
函数功能：滚动显示等待
入    参：无
出    参：无
修改时间：2013/9/13
定    义：无
全局定义：Rposition;Roll_Stop;Stop_wait;	
备    注：
****************************************************************/
void Roll_wait(void)
{	

	if(Stop_wait ==0)//判断是否等待中
	{
		if(Rposition ==19 && Rposition_n ==2){//在哪等待
			Stop_wait = Swait;
			Roll_Stop = 0;}
		else if(Rposition ==27 && Rposition_n ==4){
			Stop_wait = Swait;
			Roll_Stop = 0;}
	return;}

	else if(--Stop_wait >1)return;//等待中
	else if(Stop_wait ==1){Roll_Stop =1;Stop_wait =0;}//等待结束，恢复运行
}
/*
****************************************************************
函 数 名：void dot_show(void)
函数功能：时间小数点显示
入    参：无
出    参：无
修改时间：2013/9/14
定    义：i：8*16显示 j：两个小点 k：小点位置缓存 temp：小点输出缓存
全局定义：dot_modle：显示模式;dot：显示数组;Dposition：显示位置
备    注：
****************************************************************/

void dot_show(void)
{
	uint8 i, j, k, temp =0;
	
	//判断小数点动作
	if(dot_delay  >0)dot_delay--;//检测翻页延时
	else
	{
		Dposition++;

		if(Dposition >10 && dot_modle == 1)
			Dposition = 0;
		if(Dposition >15 && dot_modle == 2)
			Dposition = 0;
		dot_delay = 3;
	}
	
	for(i=0; i<16; i++){
	//↓↓↓↓↓↓↓↓↓↓↓↓↓显示模式↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	if(dot_modle == 1){//显示模式一
		if(Dposition <8)
			temp = dot[Dposition][i];
		else
			temp = dot[0][i];
	}
	
	else if(dot_modle == 2){//显示模式二
		if(Dposition <6)
			temp = dot[Dposition+8][i];
		else if(Dposition >=6 && Dposition <11)
			temp = dot[18-Dposition][i];
		else temp = dot[8][i];
	}
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
	//↓↓↓↓↓↓↓↓↓↓↓↓↓显示输出↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	for(j=0; j<2; j++){
	if(j==0)k = 2;
	else k = 5;	
	if(Display_mode ==3 & j==1)break;
	
	switch(dot_colour)
	{
	case 1:
		show_R[i][k] = temp;break;
	case 2:
		show_G[i][k] = temp;break;
	case 3:
		show_R[i][k] = temp;
		show_G[i][k] = temp;break;
	}}}
	//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
}