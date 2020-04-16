#include <reg52.h>

extern void LedScan();
extern void KeyScan();
extern void SecAdd();

extern unsigned char RunFlag;

unsigned char T0RH;
unsigned char T0RL;

/*配置并启动T0，ms-T0定时时间*/
void Timer0Config(unsigned int ms){
	unsigned long tmp;//临时变量
	tmp=11059200/12;//定时器计数频率
	tmp=(tmp*ms)/1000;//计算所需的计数值
	tmp=65536-tmp;//计算定时器重载值
	tmp=tmp+18;//补偿中断响应延时造成的误差
	T0RH=(unsigned char)(tmp>>8);//定时器重载值拆分为高低字节
	T0RL=(unsigned char)tmp;
	TMOD&=0xF0;//清零T0的控制位，即TMOD=bXXXX0000
	TMOD|=0x01;//配置T0为模式1,即TMOD=XXXX0001
	TH0=T0RH;//加载T0重载值
	TL0=T0RL;
	ET0=1;//使能T0中断
	TR0=1;//开启T0
}
//T0中断服务函数，完成数码管、按键扫描与秒表计数
void Timer0Interrupt() interrupt 1{
	static unsigned char cnt=0;
	cnt++;
	TH0=T0RH;//重新加载重载值
	TL0=T0RL;
	LedScan();//数码管扫描显示
	KeyScan();//按键扫描
	if(cnt>=10){
		cnt=0;
		if(RunFlag==1) SecAdd();
	}
}