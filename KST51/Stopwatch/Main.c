/* Stopwatch 秒表
作者：钱坤 QuainK
版本：v1.0.0
日期：2018.07.09
环境：此工程使用51系列的 MCU STC89C5x[RC]，使用 MCS51 开发板。
简介：数字电子技术基础课程设计。数码管显示两位分钟、两位秒钟、两位毫秒共六位。包括“开始/暂停”按钮、“重置”按钮、8个记录按钮等。
*/

#include <reg52.h>
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
extern void Timer0Config(unsigned int ms);
extern void SecReset();
extern void KeyDriver();
void main(){
	EA=1;//使能单片机中断
	ENLED=0;
	ADDR3=1;//使能74LS138译码器
	Timer0Config(1);//配置定时器0为1ms
	SecReset();//时间清零重置
	while(1){
		KeyDriver();//按键驱动
	}
}