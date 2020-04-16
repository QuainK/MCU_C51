#include<reg52.h>

extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

unsigned char alarmhourtmp1=0x30,alarmhourtmp2=0x30,alarmmintmp1=0x30,alarmmintmp2=0x30,alarmsectmp1=0x30,alarmsectmp2=0x30;
unsigned char alarmhour1=0x30,alarmhour2=0x30,alarmmin1=0x30,alarmmin2=0x30,alarmsec1=0x30,alarmsec2=0x30;
unsigned char code addr[6]={0x44,0x45,0x47,0x48,0x4A,0x4B};
unsigned char AlarmRunSta=0;
signed char AlarmSetIndex=0;
extern unsigned char RingSta;
extern signed char Time[7];

void AlarmSetInit(){
	alarmhourtmp1=alarmhour1;
	alarmhourtmp2=alarmhour2;
	alarmmintmp1=alarmmin1;
	alarmmintmp2=alarmmin2;
	alarmsectmp1=alarmsec1;
	alarmsectmp2=alarmsec2;
}

void AlarmShowTime()
{
	LcdWriteCmd(0x44|0x80);
	LcdWriteDat(alarmhourtmp1);
	LcdWriteCmd(0x45|0x80);
	LcdWriteDat(alarmhourtmp2);
	LcdWriteCmd(0x47|0x80);
	LcdWriteDat(alarmmintmp1);
	LcdWriteCmd(0x48|0x80);
	LcdWriteDat(alarmmintmp2);
	LcdWriteCmd(0x4A|0x80);
	LcdWriteDat(alarmsectmp1);
	LcdWriteCmd(0x4B|0x80);
	LcdWriteDat(alarmsectmp2);
	if(AlarmRunSta==1) LcdShowStr(0,1,"ON ");
	else LcdShowStr(0,1,"OFF");
	LcdWriteCmd(addr[AlarmSetIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void AlarmSetSeat(unsigned char dir)
{
	if(dir==0){
		AlarmSetIndex++;
		if(AlarmSetIndex>5) AlarmSetIndex=0;
	}
	else if(dir==1){
		AlarmSetIndex--;
		if(AlarmSetIndex<0) AlarmSetIndex=5;
	}
	LcdWriteCmd(addr[AlarmSetIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void AlarmSetValue(unsigned char dir){
	if(dir==0){
		if(addr[AlarmSetIndex]==0x44)
		{
			alarmhourtmp1=alarmhourtmp1+0x01;
			if(alarmhourtmp1>=0x33) alarmhourtmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x45)
		{
			if(alarmhourtmp1<=0x31)
			{
				alarmhourtmp2=alarmhourtmp2+0x01;
				if(alarmhourtmp2>=0x3A) alarmhourtmp2=0x30;
			}
			else if(alarmhourtmp1==0x32)
			{
				alarmhourtmp2=alarmhourtmp2+0x01;
				if(alarmhourtmp2>=0x34) alarmhourtmp2=0x30;
			}
		}
		else if(addr[AlarmSetIndex]==0x47)
		{
			alarmmintmp1=alarmmintmp1+0x01;
			if(alarmmintmp1>=0x36) alarmmintmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x48)
		{
			alarmmintmp2=alarmmintmp2+0x01;
			if(alarmmintmp2>=0x3A) alarmmintmp2=0x30;
		}
		else if(addr[AlarmSetIndex]==0x4A)
		{
			alarmsectmp1=alarmsectmp1+0x01;
			if(alarmsectmp1>=0x36) alarmsectmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x4B)
		{
			alarmsectmp2=alarmsectmp2+0x01;
			if(alarmsectmp2>=0x3A) alarmsectmp2=0x30;
		}
	}
	
	else if(dir==1){
		if(addr[AlarmSetIndex]==0x44)
		{
			alarmhourtmp1=alarmhourtmp1-0x01;
			if(alarmhourtmp1<=0x2F) alarmhourtmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x45)
		{
			alarmhourtmp2=alarmhourtmp2-0x01;
			if(alarmhourtmp2<=0x2F) alarmhourtmp2=0x30;
		}
		else if(addr[AlarmSetIndex]==0x47)
		{
			alarmmintmp1=alarmmintmp1-0x01;
			if(alarmmintmp1<=0x2F) alarmmintmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x48)
		{
			alarmmintmp2=alarmmintmp2-0x01;
			if(alarmmintmp2<=0x2F) alarmmintmp2=0x30;
		}
		else if(addr[AlarmSetIndex]==0x4A)
		{
			alarmsectmp1=alarmsectmp1-0x01;
			if(alarmsectmp1<=0x2F) alarmsectmp1=0x30;
		}
		else if(addr[AlarmSetIndex]==0x4B)
		{
			alarmsectmp2=alarmsectmp2-0x01;
			if(alarmsectmp2<=0x2F) alarmsectmp2=0x30;
		}
	}
	AlarmShowTime();
	if((alarmhour1!=alarmhourtmp1)||(alarmhour2!=alarmhourtmp2)||(alarmmin1!=alarmmintmp1)||(alarmmin2!=alarmmintmp2)||(alarmsec1!=alarmsectmp1)||(alarmsec2!=alarmsectmp2)){
		LcdShowStr(10,0,"   ");
	}
	else if((alarmhour1==alarmhourtmp1)&&(alarmhour2==alarmhourtmp2)&&(alarmmin1==alarmmintmp1)&&(alarmmin2==alarmmintmp2)&&(alarmsec1==alarmsectmp1)&&(alarmsec2==alarmsectmp2)){
		LcdShowStr(10,0,"Set");
	}
	LcdWriteCmd(addr[AlarmSetIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void AlarmSetOK(){
	alarmhour1=alarmhourtmp1;
	alarmhour2=alarmhourtmp2;
	alarmmin1=alarmmintmp1;
	alarmmin2=alarmmintmp2;
	alarmsec1=alarmsectmp1;
	alarmsec2=alarmsectmp2;
	LcdShowStr(10,0,"Set");
	LcdWriteCmd(addr[AlarmSetIndex]|0x80);
	LcdWriteCmd(0x0F);
}

void AlarmRun(){
	unsigned char hour=0,min=0,sec=0;
	hour=(alarmhour1-0x30)*10+(alarmhour2-0x30);
	min=(alarmmin1-0x30)*10+(alarmmin2-0x30);
	sec=(alarmsec1-0x30)*10+(alarmsec2-0x30);
	if(Time[4]==hour){
		if(Time[5]==min){
			if(Time[6]==sec){
				RingSta=1;
			}
		}
	}
}