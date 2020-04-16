#include<reg52.h>

void KeyAction(unsigned char x,unsigned char y);
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void ModeAlt();
extern void AlarmSetSeat(unsigned char dir);
extern void AlarmSetValue(unsigned char dir);
extern void AlarmShowTime();
extern void AlarmSetOK();
extern void TimeTmpOK();
extern void EditSeat(unsigned char dir);
extern void EditValue(unsigned char dir);
extern void NongLiRefresh();

extern unsigned char Mode;
extern unsigned char AlarmRunSta;
extern unsigned char GiveTimeRunSta;
extern unsigned char RingSta;
extern unsigned char ShowTimeFlag;

void KeyDriver(){
	unsigned char KeyLine=0,KeyCol=0;
	unsigned int a=1000;
	P2=0x0F;
	if(P2!=0x0F){
		while(a--);
		if(P2!=0x0F){
			P2=0x0F;//扫描列
			switch(P2){
				case (0x07): KeyCol=1; break;
				case (0x0B): KeyCol=2; break;
				case (0x0D): KeyCol=3; break;
				case (0x0E): KeyCol=4; break;
				default: break;
			}
			P2=0xF0;//扫描行
			switch(P2){
				case (0x70): KeyLine=1; break;
				case (0xB0): KeyLine=2; break;
				case (0xD0): KeyLine=3; break;
				case (0xE0): KeyLine=4; break;
				default: break;
			}
			while(P2!=0xF0){//判断是否松开，松开按键后再执行
				P2=0xF0;
				a=1000;
				while(a--);
			}
			KeyAction(KeyLine,KeyCol);
		}
	}
}

void KeyAction(unsigned char x,unsigned char y){
	switch(x){
		case 1:
			switch(y){
				case 1: ModeAlt(); break;
				case 2:
				case 3:
				case 4:
				default: break;
			}
			break;
		case 2:
			switch(y){
				case 1:
					if(AlarmRunSta==1){
						AlarmRunSta=0;
						if(RingSta==1) RingSta=0;
						LcdShowStr(0,1,"OFF");
					}
					else if(AlarmRunSta==0){
						AlarmRunSta=1;
						LcdShowStr(0,1,"ON ");
					}
					LcdWriteCmd(0x0C);
					break;
				case 2: break;
				case 3:
					if(Mode==1){
						EditValue(0);
					}
					else if(Mode==2){
						AlarmSetValue(0);
					}
					break;
				case 4:
				default: break;
			}
			break;
		case 3:
			switch(y){
				case 1:
					if(GiveTimeRunSta==1){
						GiveTimeRunSta=0;
						if(RingSta==1) RingSta=0;
						LcdShowStr(13,1,"OFF");
					}
					else if(GiveTimeRunSta==0){
						GiveTimeRunSta=1;
						LcdShowStr(13,1," ON");
					}
					LcdWriteCmd(0x0C);
					break;
				case 2:
					if(Mode==1){
						EditSeat(1);
					}
					else if(Mode==2){
						AlarmSetSeat(1);
					}
					break;
				case 3:
					if(Mode==1){
						TimeTmpOK();
					}
					else if(Mode==2){
						AlarmSetOK();
						AlarmShowTime();
					}
					break;
				case 4:
					if(Mode==1){
						EditSeat(0);
					}
					else if(Mode==2){
						AlarmSetSeat(0);
					}
					break;
				default: break;
			}
			break;
		case 4:
			switch(y){
				case 1:
					if(Mode==0){
						ShowTimeFlag=1;
					}
					break;
				case 2: break;
				case 3:
					if(Mode==1){
						EditValue(1);
					}
					else if(Mode==2){
						AlarmSetValue(1);
					}
					break;
				case 4:
				default: break;
			}
			break;
		default: break;
	}
}