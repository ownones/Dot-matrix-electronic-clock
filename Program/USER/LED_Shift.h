
#include "font.h"
#include "MESSAGE.h"

#define uint8 unsigned char 
#define uint16 unsigned int

void Page_Show(void);
void Roll_Show(void);
void Roll_wait(void);
void Shift_Time(void);
void Static_display(void);
void dot_show(void);
void Real_time_Static(void);
void D_Roll(uint8);
void Roll_char(uint8, uint16);



//unsigned char Fspeed =1;
unsigned char Rspeed =2;
#define Swait  40;

unsigned char Fposition   = 0;//�������Ƶ�λ��
unsigned char Colour_Time = 2;//��ҳʱ����ɫ�� 1����ɫ 2����ɫ 3����ɫ
unsigned char Page_speed  = 3;//���巭ҳ�ٶ�
unsigned char Rposition   = 0;//���ڹ�����λ��
unsigned char Rposition_n = 0;//����n�ַ�λ����
unsigned char Colour_Roll = 1;//����ʱ����ɫ�� 1����ɫ 2����ɫ 3����ɫ
unsigned char Roll_speed  = 3;//��������ٶ�
unsigned char Stop_wait	  = 0;//���徲ֹʱ��
bit			  Roll_Stop   = 1;//0������ֹͣ��1��������ʼ
unsigned char dot_modle   = 2;//ʱ��С������ʾ��ʽ
unsigned char Dposition	  = 0;//С��������λ
unsigned char dot_colour  = 2;//С������ɫ
unsigned char Display_mode = 3;//��ʾ��ʽ�� 1��������ҳ��ʾ 2����̬��ҳ��ʾ 3����̬��ҳ12Сʱ��

extern unsigned char Temperature;//��ȡ���¶�
extern unsigned char Humidity;//��ȡ��ʪ��  

extern unsigned char TEMP_UPDATE;//ʱ��ˢ��λ 1-6

extern unsigned char nong_Day, nong_Month;//ת����ũ��ʱ��

extern unsigned char xdata show_R[16][16];
extern unsigned char xdata show_G[16][16];

//�������޸Ĳ��Զ���

unsigned char dot_delay = 0x00;