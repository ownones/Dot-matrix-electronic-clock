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
�� �� ����void Page_Time(void)
�������ܣ���ҳ��ҳ��ʾ
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2013/9/23
��    �壺row����ˢ�µ���,tmpҪ������֣�i��jѭ���á�
ȫ�ֶ��壺TEMP_UPDATE��ҳ��־λ��Page_speed--��ҳ�ٶ�,
		��Fposition��ҳ�����ߵ�λ�ã�Colour_Time��ʾ��ɫ��
��    ע��
****************************************************************/
void Page_Show(void)
{
	uint8 tmp, row, i, j;
	if(Display_mode == 3 & TEMP_UPDATE < 3)return;//������
	else if(TEMP_UPDATE == 0)return;//�Ƿ���Ҫ��ҳ
	
//	if(Page_speed--  >0)return;//��ⷭҳ��ʱ
	//����������������������������������������������������������������������������������������������������������������������
	for(j=TEMP_UPDATE; j>0; j--){//Ҫ��ҳ��������ѭ����ҳ
	if(Display_mode == 3 & j < 3)break;//������
	switch(j)//ȷ����λ��
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
	switch (Colour_Time)//ȷ����ҳ��ɫ
	{
		case 3:
			for(i=0; i<16; i++){
				show_R[i][row] = show_R[i+1][row];//��������
				show_G[i][row] = show_G[i+1][row];}//��������
				show_R[15][row] = Num_Time[tmp][Fposition];//�����ֲ������һ��
				show_G[15][row] = Num_Time[tmp][Fposition];//�����ֲ������һ��
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
	//����������������������������������������������������������������������������������������������������������������������
	
	if(Fposition==15){//��ⷭҳ�Ƿ����
	TEMP_UPDATE = 0;//�����ҳ��λ
	Fposition   = 0;//���÷�ҳ
	return;}
	
	else{
	Fposition  += 1;}//������ҳ
//	Page_speed  = 1;//���¼�ʱ
}


/*
****************************************************************
�� �� ����
void dot_Write_bit(
		unsigned	char  Write_bit,	//Ҫд����λ��վ�����8bit
		unsigned	char  color,			//д����ֵ���ɫ		1����ɫ 2����ɫ 3����ɫ
		unsigned	char  *Datt,			//Ҫд������һά����ָ��ָ��
		unsigned	char  *Location,  //д���ĸ�������ĸ���
		unsigned	char  *start)			//д�ĸ�����
�������ܣ���̬��ʾд������
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2014/9/3��09.24
��    ע��
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
	if(*start <7){tmp_L = *start;		tmp_W = 0;}		//�ж�λ�ã���ϸ�뿴 ��ʾ����Ķ���
	else if(*start >7 && *start <16){tmp_L = *start-7;		tmp_W = 8;}
	else if(*start >15 && *start <24){tmp_L = *start-8;		tmp_W = 0;}
	else if(*start >23 && *start <32){tmp_L = *start-16;		tmp_W = 8;}

	for(i=0; i<8; i++)		//����ָ��ָ�����xdata�����ڴ治���޸����԰����������ת�Ƶ��ڴ�
		Data[i] = Datt[i];


	//�ж��Ƿ�Խ��
	if(Write_bit + *Location > 8)
	{	
		tmp = 0xFF;
		j = 8 - *Location;
		tmp <<= *Location;

		for(i=0; i<8; i++)	//Խ����Խ�����Ƶ���һ��ûԽ��ķ������ڵ���һ��
		{
			temp[i] = Data[i] &tmp;
			temp[i] >>= *Location;
			Data[i] <<= j;		//�������ڵ��Ǹ�����ǰ���Ѿ�д�������ݣ�����д������Ŀ�λ��
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
		//д������
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
		
			
		*start += 1;				//д������+1���´�λ�ö��á�
		*Location = Write_bit - (8 - *Location);
			
		return;
		
	}
	
	
			//�ж��Ƿ�д����8λ						//���߸պ�д������
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
	
	
	//д������//��ɫ�ж�д��
	
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
	
	
	//��ַ����
	
	if(Write_bit + *Location < 8)
		*Location += Write_bit;
	else if(Write_bit + *Location == 8)
		{*start += 1; 	*Location = 0;}
		

}


/*
****************************************************************
�� �� ����void Static_display(void)
�������ܣ���̬������ʾ�Ű�
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2014/9/3��09.24
��    ע��
****************************************************************/
void Static_display(void)
{
//	unsigned	char i;
	unsigned	char Location;//ƫ����
	unsigned	char start;		//����λ��
	unsigned	char color_nong_Day = 1; 
	unsigned 	char color_new_Day  = 3;
	
	
////��ʾũ����Ϣ
	
	Location = 0;
	start = 16;
	dot_Write_bit(8, 1, &Static_word[4], &Location, &start);
	
	//ũ����
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
		dot_Write_bit(1, color_nong_Day, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(6, color_nong_Day, &Static_6bit[12], &Location, &start);//��
	}
	
	//ũ����
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
		
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		
		if(nong_Day == 10 || nong_Day == 20 || nong_Day == 30)
			dot_Write_bit(7, color_nong_Day, &Static_word[2], &Location, &start);//	
		else if(nong_Day == 1)
			dot_Write_bit(6, 3, &Static_6bit[1], &Location, &start);
		else if(nong_Day == 15)
			dot_Write_bit(6, 3, &Static_6bit[5], &Location, &start);
		else
			dot_Write_bit(6, color_nong_Day, &Static_6bit[nong_Day %10], &Location, &start);//		

	}
	
////��ʾ������Ϣ
	Location = 0;
	start = 24;
	
	//������20xx
	dot_Write_bit(4, color_new_Day, &Static_4bit[2], &Location, &start);//2
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	dot_Write_bit(4, color_new_Day, &Static_4bit[0], &Location, &start);//0
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Year/10], &Location, &start);//X
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Year%10], &Location, &start);//X
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	dot_Write_bit(6, 1, &Static_6bit[11], &Location, &start);//��
	dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	
	//������
	
	if(CurrentTime.Month <10)
	{
		dot_Write_bit(5, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	}
	else 
	{
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Month%10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	}
	dot_Write_bit(6, 1, &Static_6bit[12], &Location, &start);//��
	
	//������
	if(CurrentTime.Day < 10)
	{
		dot_Write_bit(5, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day], &Location, &start);//x
	}
	else
	{
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(4, color_new_Day, &Static_4bit[CurrentTime.Day%10], &Location, &start);//x
	}
	dot_Write_bit(6, 1, &Static_6bit[13], &Location, &start);//��
	
	
	//����
	
	if(CurrentTime.Week == 0)
		dot_Write_bit(6, 3, &Static_6bit[14], &Location, &start);//x
	else
		dot_Write_bit(6, 3, &Static_6bit[CurrentTime.Week], &Location, &start);//x
	
	
	
}

void Real_time_Static(void)
{
	unsigned	char Location;//ƫ����
	unsigned	char start;		//����λ��
	
	Location = 7;
	start = 21;
	
	//��ʾ�¶ȣ�2�����
	if(Temperature > 10)
	{
		dot_Write_bit(5, 2, &Static_5bit[Temperature/10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(5, 2, &Static_5bit[Temperature%10], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	}
	else
	{
		dot_Write_bit(6, 1, &Static_6bit[0], &Location, &start);//�ո�
		dot_Write_bit(5, 2, &Static_5bit[Temperature], &Location, &start);//x
		dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
	}
	
	dot_Write_bit(4, 2, &Static_5bit[10], &Location, &start);//��
	
	//��ʾʪ�ȣ�2�����
	if(Display_mode == 3)
	{
		Location = 7;
		start = 12;
		
		if(Humidity > 10)
		{
			dot_Write_bit(5, 2, &Static_5bit[Humidity/10], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
			dot_Write_bit(5, 2, &Static_5bit[Humidity%10], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		}
		else 
		{
			dot_Write_bit(6, 1, &Static_6bit[0], &Location, &start);//�ո�
			dot_Write_bit(5, 2, &Static_5bit[Humidity], &Location, &start);//x
			dot_Write_bit(1, 1, &Static_6bit[0], &Location, &start);//�ո�
		}
	
	dot_Write_bit(4, 2, &Static_5bit[11], &Location, &start);//x
	}
		
}

/*
****************************************************************
�� �� ����void Roll_Show(void)
�������ܣ�������ʾ
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2013/9/13��17.27
��    �壺temp�����ʹ�õĻ���
��    ע��
****************************************************************/
void Roll_Show(void)
{
	uint16 temp;
	if(Roll_Stop ==0)return;//ֹͣ������־λ
	if(Roll_speed-- >0)return;//�����ٶ�
	
	//������������������������������������������һ���ַ���������������������������������������������������������
	if(Rposition_n ==0)
	{
		Rposition +=1;
		if(Rposition>sizeof(queue)/3-1)//����Ĺ�������
			Rposition=0;
		Rposition_n = queue[Rposition][2];
	}
	
	//���������������������������������������ĵ����ڡ�����������������������������������������������������������������
	if(queue[Rposition][0] ==0)
	{
		if(queue[Rposition][1] != 0)
			D_Roll(queue[Rposition][1]);}
	
	//������������������������������������������ʾһ�š�����������������������������������������������������������������
	if(queue[Rposition][2] == 0)temp = 0;//������
	else if(queue[Rposition][2] == 16)//�˳�������ʾ����
	{
		temp = Nun_Roll[queue[Rposition][1]][16-Rposition_n];
		temp <<=8;
		temp |= Nun_Roll[queue[Rposition][1]][17-Rposition_n];
	}
	else if(queue[Rposition][2] == 32)//�˳�������ʾ����
	{
		temp = zhif_song[queue[Rposition][1]][32-Rposition_n];
		temp <<=8;
		temp |= zhif_song[queue[Rposition][1]][33-Rposition_n];
	}
	Rposition_n -=2;//����������ɺ�׼�������¸�
	Roll_char(queue[Rposition][0],temp);//�����ʾ����
	Roll_speed = Rspeed;//�ٶȿ���
}
/*
****************************************************************
�� �� ����void Roll_char(uint8 Coloer, uint16 zhifu16)
�������ܣ����������ַ����
��    �Σ�Coloer��zhifu16
��    �Σ���
���ƺ�����void Roll_Show(void)
�޸�ʱ�䣺2013/9/8��10.20
��    �壺Coloer����ɫ��zhifu16���ұ�׼���Ƴ��ַ�
ȫ�ֶ��壺show_R;show_G;show_O;	
��    ע��
****************************************************************/

void Roll_char(uint8 Coloer, uint16 zhifu16)
{
	uint8 i, j;	
	//��������������������������������������������λ���ơ���������������������������������������������������������

		for(i=0;i<16;i++){//һ��16λ
		show_R[i][8]<<=1;//�Ƴ���ʾ
		show_G[i][8]<<=1;
		for(j=0;j<7;j++)//һ��8λ
		{
			if((bit)(show_R[i][j+9]&0x80))
				show_R[i][j+8] |= 0x01;
			if((bit)(show_G[i][j+9]&0x80))
				show_G[i][j+8] |= 0x01;
			show_R[i][j+9] <<=1;
			show_G[i][j+9] <<=1;
		}}
	//����������������������������������������������������������������������������������������������������������������������
	//����������������������������������������д�����һ�š�����������������������������������������������������
	
	switch(Coloer)//��ɫ���
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
	//����������������������������������������������������������������������������������������������������������������������
}
/*
****************************************************************
�� �� ����void D_Roll(uint8 i)
�������ܣ��ı����������ݣ��൱�����ڸı䣩
��    �Σ�i
��    �Σ���
���ƺ�����void Roll_Show(void)
�޸�ʱ�䣺2013/9/8��10.20
��    �壺
ȫ�ֶ��壺Rposition����������֣�Rposition_n��������ŵ�λ�ã�
��    ע��
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
�� �� ����void Roll_wait(void)
�������ܣ�������ʾ�ȴ�
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2013/9/13
��    �壺��
ȫ�ֶ��壺Rposition;Roll_Stop;Stop_wait;	
��    ע��
****************************************************************/
void Roll_wait(void)
{	

	if(Stop_wait ==0)//�ж��Ƿ�ȴ���
	{
		if(Rposition ==19 && Rposition_n ==2){//���ĵȴ�
			Stop_wait = Swait;
			Roll_Stop = 0;}
		else if(Rposition ==27 && Rposition_n ==4){
			Stop_wait = Swait;
			Roll_Stop = 0;}
	return;}

	else if(--Stop_wait >1)return;//�ȴ���
	else if(Stop_wait ==1){Roll_Stop =1;Stop_wait =0;}//�ȴ��������ָ�����
}
/*
****************************************************************
�� �� ����void dot_show(void)
�������ܣ�ʱ��С������ʾ
��    �Σ���
��    �Σ���
�޸�ʱ�䣺2013/9/14
��    �壺i��8*16��ʾ j������С�� k��С��λ�û��� temp��С���������
ȫ�ֶ��壺dot_modle����ʾģʽ;dot����ʾ����;Dposition����ʾλ��
��    ע��
****************************************************************/

void dot_show(void)
{
	uint8 i, j, k, temp =0;
	
	//�ж�С���㶯��
	if(dot_delay  >0)dot_delay--;//��ⷭҳ��ʱ
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
	//����������������������������ʾģʽ����������������������������������
	if(dot_modle == 1){//��ʾģʽһ
		if(Dposition <8)
			temp = dot[Dposition][i];
		else
			temp = dot[0][i];
	}
	
	else if(dot_modle == 2){//��ʾģʽ��
		if(Dposition <6)
			temp = dot[Dposition+8][i];
		else if(Dposition >=6 && Dposition <11)
			temp = dot[18-Dposition][i];
		else temp = dot[8][i];
	}
	//����������������������������������������������������������������������������
	//����������������������������ʾ�������������������������������������
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
	//����������������������������������������������������������������������������
}