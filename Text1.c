/*
51单片机 五档数显风扇，包含电源开关(伪)、增加风力的按钮、减弱风力的按钮、显示风力的数显管、当然还有风扇(电机)。 :)
*/

#include<reg51.h>

sbit FanA=P3^0;
sbit FanB=P3^1;				//定义控制风扇的两个端口
sbit PowerSwitch=P1^0;			//定义电源开关
sbit ButtonAdd=P1^1;			//定义按钮增，按下后风力加强
sbit ButtonMinus=P1^2;			//定义按钮减，按下后风力减弱

unsigned char TableCode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92};		//定义风力级别：0,1,2,3,4,5.

void delay(unsigned int z){		//延时函数
	unsigned int i,j;
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}

void main(){				//主函数
	int WindLevel=3;		//定义默认风力级别为	3级
	while(1){			//死循环
		
		if(PowerSwitch==0){	//如果电源开关接通
			
			P2=TableCode[WindLevel];			//数码管显示当前风力	3级风
			FanA=1;
			FanB=0;						//风扇旋转
			delay(WindLevel*100);				//调用延时函数，保持旋转	300
			FanA=0;
			FanB=0;						//风扇停止加速
			delay(500-WindLevel*100);			//调用延时函数，保持停止	200
									//占空比为		300/(300+200)=3/5
			
			if(ButtonAdd==1&&ButtonMinus==0){	//如果按下 按钮减 ，而且没有按下按钮增
				delay(5);				//延时
				if(ButtonAdd==1&&ButtonMinus==0){	//防抖动
					WindLevel--;
					if(WindLevel==0)
						WindLevel=5;		//如果风力降到0，则将风力置为5
					P2=TableCode[WindLevel];	//数码管显示减弱后的风力	2级风
					FanA=1;
					FanB=0;				//风扇旋转
					delay(WindLevel*100);		//调用延时函数，保持旋转	200
					FanA=0;
					FanB=0;				//风扇停止加速
					delay(500-WindLevel*100);	//调用延时函数，保持停止	300
				}					//占空比为		200/(200+300)=2/5
			}
			
			if(ButtonAdd==0&&ButtonMinus==1){		//如果按下 按钮增 ，而且没有按下按钮减
				delay(5);				//延时
				if(ButtonAdd==0&&ButtonMinus==1){	//防抖动
					WindLevel++;
					if(WindLevel==6)
						WindLevel=1;		//如果风力增到6，则将风力置为1
					P2=TableCode[WindLevel];	//数码管显示加强后的风力	4级风
					FanA=1;
					FanB=0;				//风扇旋转
					delay(WindLevel*100);		//调用延时函数，保持旋转	400
					FanA=0;
					FanB=0;				//风扇停止加速
					delay(500-WindLevel*100);	//调用延时函数，保持停止	100
				}					//占空比为		400/(400+100)=4/5
			}
			
		}
		
		else				//如果电源开关断开 没有接通电源
		{
			P2=0xff;		//数码管关闭 什么都不显示
			FanA=0;
			FanB=0;			//风扇不转
			WindLevel=3;		//恢复默认风力级别为	3级
		}
		
	}
}
