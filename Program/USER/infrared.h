#ifndef __INFRARED_H_
#define __INFRARED_H_

/*****************************
   		红外解码
******************************/

void transcode(void)
{
	uchar i, j, k=0;  
	uchar cord, disdat[7];		   //disdat[7]用来存放解出来的十六进制数
	int value;
	for(i=0;i<4;i++)                //处理4个字节
	{
	  for(j=1;j<=8;j++)            //处理1个字节8位
	     {
	      cord=redat[k];
		  k++;
	      if(cord>7)               //大于2240us为1
	         value=value |0x01;
	      if(j<8)
			 value=value <<1;
	     }
	recod[i]=value;
	value=0;
	}
	 
//**将二进制码转换成十六进制显示**

	disdat[0]=recod[0]/16;        //显示用户码
	disdat[1]=recod[0]%16;
	disdat[2]=recod[1]/16;
	disdat[3]=recod[1]%16;
	
	disdat[4]=recod[2]/16;        //显示用户数据
	disdat[5]=recod[2]%16;
	
	disdat[6]=recod[3]/16;        //显示数据反码
	disdat[7]=recod[3]%16;
	
	 if(recod[2] != ~recod[3])    //判断数据与反码正确
	 	rerock = 0;
}

/*****************************
   	 中断红外解码判断
******************************/
void Numerical_judgment(void)
{
	if(rerock)
	{
		transcode();				//进行遥控解码
		if(!rerock)return;			//码值与反码不同，程序退出			
	}
}

#endif