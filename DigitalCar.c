#include<reg52.h>
sbit LeftWheelA=P3^0;
sbit LeftWheelB=P3^1;								//定义控制左轮的两个端口
sbit RightWheelA=P3^2;
sbit RightWheelB=P3^3;								//定义控制右轮的两个端口
sbit PowerSwitch=P1^0;								//定义电源开关
sbit ButtonAdd=P1^1;								//定义按钮增，按下后车速增加
sbit ButtonMinus=P1^2;								//定义按钮减，按下后车速减慢
int DefaultSpeedLevel=3;							//定义默认车速级别		2,3,4,5
unsigned char SpeedCode[]={0xc0, 0xf9,0xa4,0xb0,0x99,0x92, 0x82,0xf8,0x80,0x90};		//定义车速级别：0, 1,2,3,4,5. 6,7,8,9
unsigned char SpeedCodeWithDot[]={0x40, 0x79,0x24,0x30,0x19,0x12, 0x02,0x78,0x00,0x10};	//定义工程模式时显示的车速级别（点亮小数点以区别于行进模式）：0, 1,2,3,4,5. 6,7,8,9
int SpeedLevel;										//定义车速级别
int MaxSpeed;										//定义最大车速级别

void delay(unsigned int z);							//声明延时函数
void StraightMove();								//声明直线行进函数
void StopMove();									//声明停止函数
void SetDefaultSpeedLevel();						//声明修改默认车速级别函数	工程模式

void main(){										//主函数
	SpeedLevel=DefaultSpeedLevel;					//重置车速级别为默认值
	MaxSpeed=SpeedLevel*2-1;						//计算最大车速级别		3,5,7,9
	while(1){										//死循环
		if(PowerSwitch==0){							//如果电源开关接通
			StraightMove();							//直线行进
			if(ButtonAdd==1&&ButtonMinus==0){		//如果按下按钮减 ，而且没有按下按钮增
				delay(50);							//延时
				if(ButtonAdd==1&&ButtonMinus==0){	//防抖动
					SpeedLevel--;
					if(SpeedLevel==0)
						SpeedLevel=MaxSpeed;		//如果车速降到0，则将车速置为最大车速
				}
			}
			if(ButtonAdd==0&&ButtonMinus==1){		//如果按下按钮增 ，而且没有按下按钮减
				delay(50);							//延时
				if(ButtonAdd==0&&ButtonMinus==1){	//防抖动
					SpeedLevel++;
					if(SpeedLevel==MaxSpeed+1)
						SpeedLevel=1;				//如果车速增到超过最大车速，则将车速置为1
				}
			}
		}
		else{										//如果电源开关断开
			StopMove();								//停止行进
			if(ButtonAdd==1&&ButtonMinus==0){		//如果按下按钮减
				delay(2000);						//延时
				if(ButtonAdd==1&&ButtonMinus==0){	//如果长按按钮减
					SetDefaultSpeedLevel();			//修改默认车速级别	工程模式
				}
			}
		}
	}
}


void delay(unsigned int z){							//延时函数
	unsigned int i,j;
	for(i=z;i>0;i--)
		for(j=110;j>0;j--);
}

void StraightMove(){								//直线行进函数
			P2=SpeedCode[SpeedLevel];				//数码管显示当前车速
			LeftWheelA=1;
			LeftWheelB=0;							//左轮旋转
			RightWheelA=1;
			RightWheelB=0;							//右轮旋转
			delay(SpeedLevel*100);					//调用延时函数，保持车速
			LeftWheelA=0;
			LeftWheelB=0;							//左轮停止加速
			RightWheelA=0;
			RightWheelB=0;							//右轮停止加速
			delay((MaxSpeed-SpeedLevel)*100);		//调用延时函数，保持停止
}

void StopMove(){									//停止函数
		P2=0xff;									//数码管关闭 什么都不显示
		LeftWheelA=0;
		LeftWheelB=0;								//左轮停转
		RightWheelA=0;
		RightWheelB=0;								//右轮停转
		SpeedLevel=DefaultSpeedLevel;				//重置车速级别为默认值
}

void SetDefaultSpeedLevel(){						//修改默认车速级别		2,3,4,5
	while(1){										//死循环
		P2=SpeedCodeWithDot[DefaultSpeedLevel];		//数码管显示默认车速（点亮小数点以区别于行进模式）
		if(ButtonAdd==0&&ButtonMinus==1){			//如果按下按钮增
			delay(500);								//延时
			if(ButtonAdd==0&&ButtonMinus==1){		//防抖动
				DefaultSpeedLevel++;
				if(DefaultSpeedLevel==6)
					DefaultSpeedLevel=2;			//如果默认车速增到6，则将默认车速置为2
			}
		}
		if(ButtonAdd==1&&ButtonMinus==0){			//如果按下按钮减
			delay(2000);							//延时
			if(ButtonAdd==1&&ButtonMinus==0){		//如果长按按钮减
				SpeedLevel=DefaultSpeedLevel;		//重置车速级别为新设置的默认值
				MaxSpeed=SpeedLevel*2-1;			//计算新的最大车速级别		3,5,7,9
				return;								//默认车速级别自定义完成 退出工程模式 返回main函数
			}
		}
		if(PowerSwitch==0){							//如果在工程模式直接接通电源开关 初始化默认车速级别
			DefaultSpeedLevel=3;					//初始化默认车速级别
			SpeedLevel=DefaultSpeedLevel;			//重置车速级别为默认值
			MaxSpeed=SpeedLevel*2-1;				//计算最大车速级别		3,5,7,9
			return;									//默认车速级别初始化完成 退出工程模式 返回main函数
		}
	}
}
