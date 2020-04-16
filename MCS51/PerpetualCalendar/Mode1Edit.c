#include <reg52.h>

void TimeTmpCorrect();
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdTimeRefresh(signed char *t);
extern void WeekRefresh(signed char *t);
void LcdShow();
extern void TimeCorrect(signed char *t);
signed char TimeTmp[7]={20,17,11,30,23,59,55};
unsigned char EditAddr[7]={
	0x02,0x04,0x07,0x0A,
	0x45,0x48,0x4B
};
signed char EditIndex=4;
extern signed char Time[7];
extern unsigned char Mode;

void EditSeat(unsigned char dir){
	if(dir==0){
		EditIndex++;
		if(EditIndex>6) EditIndex=0;
	}
	else{
		EditIndex--;
		if(EditIndex<0) EditIndex=6;
	}
	LcdWriteCmd(EditAddr[EditIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void EditValue(unsigned char dir){
	if(dir==0){
		TimeTmp[EditIndex]++;
	}
	else if(dir==1){
		TimeTmp[EditIndex]--;
	}
	
	TimeCorrect(&TimeTmp);
	LcdTimeRefresh(&TimeTmp);
	WeekRefresh(&TimeTmp);
	LcdShow();
	LcdWriteCmd(EditAddr[EditIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void TimeTmpOK(){
	unsigned char i=0;
	for(i=0;i<=6;i++)
		Time[i]=TimeTmp[i];
	LcdWriteCmd(0x0C);
	Mode=0;
}