#include <reg52.h>
sbit BUZZ=P3^7;

void ConfigTimer0(unsigned int ms,unsigned long sysfrequ);
void ConfigTimer1(unsigned int ms,unsigned long sysfrequ);
extern void TimeAdd();
extern void LcdTimeRefresh(signed char *t);
extern void WeekRefresh(signed char *t);
extern void LcdShow();
extern void LcdWriteCmd(unsigned char cmd);
extern void AlarmRun();
extern void NongLiRefresh();

unsigned char T0RH=0x00;
unsigned char T0RL=0x00;
unsigned char T1RH=0x00;
unsigned char T1RL=0x00;
unsigned char GiveTimeRunSta=1;
unsigned char RingSta=0;
extern unsigned char Mode;
extern signed char Time[];
extern signed char TimeTmp[];
extern unsigned char AlarmRunSta;
extern unsigned char ShowTimeFlag;
extern unsigned char nongli[2];
extern unsigned char LcdBuff[2][16];

void ConfigTimer0(unsigned int ms,unsigned long sysfrequ){
	unsigned long tmp;
	tmp=65536-ms*sysfrequ/12000;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void ConfigTimer1(unsigned int ms,unsigned long sysfrequ){
	unsigned long tmp;
	tmp=65536-ms*sysfrequ/12000;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD&=0x0F;
	TMOD|=0x10;
	TH1=T1RH;
	TL1=T1RL;
	ET1=0;
	TR1=0;
}

void InterruptTimer0() interrupt 1{
	static unsigned char count=0,cnt=0,buzzsec=0;
	count++;
	TH0=T0RH;
	TL0=T0RL;
	if(count>=100){
		count=0;
		TimeAdd();
		
		if(Mode==0){//显示阳历或者农历
			LcdTimeRefresh(&Time);
			if(ShowTimeFlag==1){
				cnt++;
				NongLiRefresh();
				LcdBuff[0][5]=0x20;
				LcdBuff[0][6]=0x30+nongli[0]/10;
				LcdBuff[0][7]=0x30+nongli[0]%10;
				LcdBuff[0][8]=0x20;
				LcdBuff[0][9]=0x30+nongli[1]/10;
				LcdBuff[0][10]=0x30+nongli[1]%10;
				if(cnt>=3){
					cnt=0;
					LcdBuff[0][5]=0x2D;
					LcdBuff[0][8]=0x2D;
					ShowTimeFlag=0;
				}
			}
			LcdShow();
			LcdWriteCmd(0x0C);
		}
		
		if(AlarmRunSta==1){
			AlarmRun();
		}
		
		if(GiveTimeRunSta==1){
			if((Time[5]==0)||(Time[5]==30)){
				if(Time[6]<=8) RingSta=1;
				else if(AlarmRunSta==0) RingSta=0;
			}
		}
		
		if(RingSta==1){//响大约8秒
			buzzsec++;
			ET1=1;
			TR1=1;
			if(buzzsec>=10){
				buzzsec=0;
				RingSta=0;
				ET1=0;
				TR1=0;
			}
		}
		else{
			ET1=0;
			TR1=0;
		}
	}
}

void InterruptTimer1() interrupt 3{
	static unsigned int j=0;
	TH1=T1RH;
	TL1=T1RL;
	j++;
	if(j<=150){
		if(Mode==0) LcdWriteCmd(0x0C);
		BUZZ=~BUZZ;
	}
	else if(j<=300){
		if(Mode==0) LcdWriteCmd(0x0C);
		BUZZ=~BUZZ;
	}
	else if(j<=450){
		if(Mode==0) LcdWriteCmd(0x08);
	}
	else if(j<=600){
		if(j>=600) j=0;
		if(Mode==0) LcdWriteCmd(0x08);
	}
}