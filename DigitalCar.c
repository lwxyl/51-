/*
DigitalCar v1.0 2017-11-12
五档数显小车，包含电源开关(伪)、增加车速的按钮、减弱车速的按钮、显示车速的数显管、当然还有引擎(电机)。 :)
*/

#include<reg51.h>

sbit LeftWheelA=P3^0;
sbit LeftWheelB=P3^1;			//定义控制左轮的两个端口
sbit RightWheelA=P3^2;
sbit RightWheelB=P3^3;			//定义控制右轮的两个端口
sbit PowerSwitch=P1^0;			//定义电源开关
sbit ButtonAdd=P1^1;			//定义按钮增，按下后车速增加
sbit ButtonMinus=P1^2;			//定义按钮减，按下后车速减慢
int SpeedLevel=3;				//定义默认车速级别		2,3,4,5
unsigned char SpeedCode[]={0xc0, 0xf9,0xa4,0xb0,0x99,0x92, 0x82,0xf8,0x80,0x90};	//定义车速级别：0, 1,2,3,4,5. 6,7,8,9
int MaxSpeed;

void delay(unsigned int z);		//声明延时函数
void StraightMove();			//声明直线行进函数
void StopMove();				//声明停止函数

void main(){					//主函数
	MaxSpeed=SpeedLevel*2-1;	//定义最大车速级别		3,5,7,9
	while(1){					//死循环
		if(PowerSwitch==0){		//如果电源开关接通
			StraightMove();		//直线行进
			if(ButtonAdd==1&&ButtonMinus==0){		//如果按下 按钮减 ，而且没有按下按钮增
				delay(3);							//延时
				if(ButtonAdd==1&&ButtonMinus==0){	//防抖动
					SpeedLevel--;
					if(SpeedLevel==0)
						SpeedLevel=MaxSpeed;		//如果车速降到0，则将车速置为最大车速
				}
			}
			if(ButtonAdd==0&&ButtonMinus==1){		//如果按下 按钮增 ，而且没有按下按钮减
				delay(3);							//延时
				if(ButtonAdd==0&&ButtonMinus==1){	//防抖动
					SpeedLevel++;
					if(SpeedLevel==MaxSpeed+1)
						SpeedLevel=1;				//如果车速增到超过最大车速，则将车速置为1
				}
			}
		}
		else{					//如果电源开关断开 没有接通电源
			StopMove();
		}
	}
}


void delay(unsigned int z){	//延时函数
	unsigned int i,j;
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}

void StraightMove(){							//直线行进函数
			P2=SpeedCode[SpeedLevel];			//数码管显示当前车速
			LeftWheelA=1;
			LeftWheelB=0;						//左轮旋转
			RightWheelA=1;
			RightWheelB=0;						//右轮旋转
			delay(SpeedLevel*100);				//调用延时函数，保持车速
			LeftWheelA=0;
			LeftWheelB=0;						//左轮停止加速
			RightWheelA=0;
			RightWheelB=0;						//右轮停止加速
			delay((MaxSpeed-SpeedLevel)*100);	//调用延时函数，保持停止
}

void StopMove(){		//停止函数
		P2=0xff;		//数码管关闭 什么都不显示
		LeftWheelA=0;
		LeftWheelB=0;	//左轮停转
		RightWheelA=0;
		RightWheelB=0;	//右轮停转
		SpeedLevel=3;	//恢复默认车速级别为	3级
}