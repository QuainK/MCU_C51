#include <reg52.h>

void LcdShow();
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

unsigned char LcdBuff[2][16]={
	{0x20,0x30,0x30,0x30, 0x30,0x2D,0x30,0x30, 0x2D,0x30,0x30,0x20 ,0x20,0x20,0x20,0x20},//_0000-00-00_____
	{0x20,0x20,0x20,0x20, 0x30,0x30,0x3A,0x30, 0x30,0x3A,0x30,0x30, 0x20,0x20,0x20,0x20}//____00:00::00____
};
extern unsigned char Mode;
extern unsigned char Time[7];
extern unsigned char TimeTmp[7];
extern unsigned char code weekcode[];
extern unsigned char Week;
extern unsigned char AlarmRunSta;
extern unsigned char GiveTimeRunSta;

void LcdTimeRefresh(signed char *t){
	LcdBuff[0][1]=0x30+*t/10;
	LcdBuff[0][2]=0x30+*t%10;
	LcdBuff[0][3]=0x30+*(t+1)/10;
	LcdBuff[0][4]=0x30+*(t+1)%10;
	
	LcdBuff[0][6]=0x30+*(t+2)/10;
	LcdBuff[0][7]=0x30+*(t+2)%10;
	
	LcdBuff[0][9]=0x30+*(t+3)/10;
	LcdBuff[0][10]=0x30+*(t+3)%10;
	
	LcdBuff[1][4]=0x30+*(t+4)/10;
	LcdBuff[1][5]=0x30+*(t+4)%10;
	
	LcdBuff[1][7]=0x30+*(t+5)/10;
	LcdBuff[1][8]=0x30+*(t+5)%10;
	
	LcdBuff[1][10]=0x30+*(t+6)/10; 
	LcdBuff[1][11]=0x30+*(t+6)%10;
	
}

void LcdShow(){
	unsigned char i=0;
	for(i=0;i<16;i++){
		LcdWriteCmd(0x00+i|0x80);
		LcdWriteDat(LcdBuff[0][i]);
	}
	for(i=0;i<16;i++){
		LcdWriteCmd(0x40+i|0x80);
		LcdWriteDat(LcdBuff[1][i]);
	}
	
	if(AlarmRunSta==0) LcdShowStr(0,1,"OFF");
	else if(AlarmRunSta==1) LcdShowStr(0,1,"ON ");
	
	if(GiveTimeRunSta==0) LcdShowStr(13,1,"OFF");
	else if(GiveTimeRunSta==1) LcdShowStr(13,1," ON");

	if(Mode==0){
		if(Week==0)	LcdShowStr(12,0,"Sun");
		else if(Week==1) LcdShowStr(12,0,"Mon");
		else if(Week==2) LcdShowStr(12,0,"Tue");
		else if(Week==3) LcdShowStr(12,0,"Wed");
		else if(Week==4) LcdShowStr(12,0,"Thu");
		else if(Week==5) LcdShowStr(12,0,"Fri");
		else if(Week==6) LcdShowStr(12,0,"Sat");
	}
}