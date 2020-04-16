/* 万年历 PerpetualCalendar
作者：钱坤 QuainK
版本：v1.1.0-beta
日期：2017.12.02
环境：此工程使用51系列的 MCU STC89C5x[RC]，使用 MCS51 开发板。
简介：LCD实时显示日期时间星期，可以切换公历农历；
	按键规则：
	第1行：
		第1列：切换模式；
	
	第2行：
		第1列：闹钟开关；
		第3列：增大当前编辑的值；
	
	第3行：
		第1列：报时开关（整点半点）；
		第2列：左移当前编辑的位置；
		第3列：确定输入的值；
		第4列：右移当前编辑的位置；
	
	第4行：
		第1列：显示农历（约3秒后恢复显示阳历）；
		第3列：减小当前编辑的值；
更新：
1.修复：
	1.1 修正了闹钟在一些情况下不能正常工作；
	1.2 修正了编辑光标在按确认键后消失或者错位；
2.增加:
	2.1 增加闹钟和报时的开关，方便启用或禁用该功能。其中，闹钟开关和闹钟时间设定互不影响。
3.修改：
	3.1优化了闹钟开关状态和报时开关状态的显示样式，即“OFF”和“ON”，左边为闹钟，右边为报时；
	3.2 优化了蜂鸣器的发声，包括频率和时长；
*/

#include <reg52.h>

void ModeAlt();
extern void LcdInit();
extern void ConfigTimer0(unsigned int ms,unsigned long sysfrequ);
extern void ConfigTimer1(unsigned int ms,unsigned long sysfrequ);
extern void KeyDriver();
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void AlarmShowTime();
extern void AlarmSetInit();

unsigned char Mode=0;
extern signed char Time[];
extern signed char TimeTmp[];
extern unsigned char AlarmRunSta;
extern signed char EditIndex;
extern unsigned char EditAddr[];

void main(){
	EA=1;
	P1=0xFF;
	LcdInit();
	ConfigTimer0(10,12000000);
	ConfigTimer1(1,3000000);
	ET1=0;
	TR1=0;
	while(1){
		KeyDriver();
	}
}

void ModeAlt(){
	unsigned char i=0;
	switch(Mode){
	case 0:
		Mode=1;
		for(i=0;i<=6;i++)
			TimeTmp[i]=Time[i];
		LcdWriteCmd(EditAddr[EditIndex]|0x80);
		LcdWriteCmd(0x0F);
		break;
	case 1:
		Mode=2;
		for(i=0;i<16;i++){
			LcdWriteCmd(0x00+i|0x80);
			LcdWriteDat(0x20);
		}
		LcdShowStr(4,0,"Alarm");
		AlarmSetInit();
		AlarmShowTime();
		break;
	case 2:
		Mode=0;
		break;
	default: break;
	}
}