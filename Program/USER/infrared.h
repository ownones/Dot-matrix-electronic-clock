#ifndef __INFRARED_H_
#define __INFRARED_H_

/*****************************
   		�������
******************************/

void transcode(void)
{
	uchar i, j, k=0;  
	uchar cord, disdat[7];		   //disdat[7]������Ž������ʮ��������
	int value;
	for(i=0;i<4;i++)                //����4���ֽ�
	{
	  for(j=1;j<=8;j++)            //����1���ֽ�8λ
	     {
	      cord=redat[k];
		  k++;
	      if(cord>7)               //����2240usΪ1
	         value=value |0x01;
	      if(j<8)
			 value=value <<1;
	     }
	recod[i]=value;
	value=0;
	}
	 
//**����������ת����ʮ��������ʾ**

	disdat[0]=recod[0]/16;        //��ʾ�û���
	disdat[1]=recod[0]%16;
	disdat[2]=recod[1]/16;
	disdat[3]=recod[1]%16;
	
	disdat[4]=recod[2]/16;        //��ʾ�û�����
	disdat[5]=recod[2]%16;
	
	disdat[6]=recod[3]/16;        //��ʾ���ݷ���
	disdat[7]=recod[3]%16;
	
	 if(recod[2] != ~recod[3])    //�ж������뷴����ȷ
	 	rerock = 0;
}

/*****************************
   	 �жϺ�������ж�
******************************/
void Numerical_judgment(void)
{
	if(rerock)
	{
		transcode();				//����ң�ؽ���
		if(!rerock)return;			//��ֵ�뷴�벻ͬ�������˳�			
	}
}

#endif