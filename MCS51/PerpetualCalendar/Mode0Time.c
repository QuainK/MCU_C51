#include <reg52.h>

void TimeAdd();
void TimeCorrect(signed char *t);
void WeekRefresh(signed char *t);
extern void LcdShow();
extern void LcdWriteCmd(unsigned char cmd);
extern void LcdWriteDat(unsigned char dat);
signed char Time[7]={20,17,11,30,23,59,55};
unsigned char Week=4;
/*unsigned char code nl[2][24]={
	{12,1, 2, 3, 4, 5, 6, 60, 7, 8, 9, 10,  11,12,1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	{4, 5, 4, 5, 6, 7, 8, 10, 11,12,13,14,  15,16,14,16,16,18,18,20,22,22,24,24}
};*/
unsigned char nongli[2]={10,14};
extern unsigned char LcdBuff[2][16];
unsigned char ShowTimeFlag=0;

void TimeAdd(){
	Time[6]++;
	TimeCorrect(&Time);
	WeekRefresh(&Time);
}

void TimeCorrect(signed char *t){
	unsigned int YearTmp;
	YearTmp=*t*100+*(t+1);
	if(*(t+6)>59){
		*(t+6)=0;
		(*(t+5))++;
	}
	else if(*(t+6)<0){
		*(t+6)=59;
		(*(t+5))--;
	}
	
	if(*(t+5)>59){
		*(t+5)=0;
		(*(t+4))++;
	}
	else if(*(t+5)<0){
		*(t+5)=59;
		(*(t+4))--;
	}
	
	if(*(t+4)>23){
		*(t+4)=0;
		(*(t+3))++;
	}
	else if(*(t+4)<0){
		*(t+4)=23;
		(*(t+3))--;
	}
	
	if(*(t+3)>=30&&*(t+2)!=2){
		switch(*(t+2)){
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if(*(t+3)>31){
					*(t+3)=1;
					(*(t+2))++;
				}
				break;
			case 4:
			case 6:
			case 9:
			case 11:
				if(*(t+3)>30){
					*(t+3)=1;
					(*(t+2))++;
				}
				break;
		}
	}
	else if((*(t+3)>=28)&&(*(t+2)==2)){
		if(YearTmp%100==0&&YearTmp%400==0){
			if(*(t+3)>29){
				*(t+3)=1;
				*(t+2)=3;
			}
		}
		else if(YearTmp%100!=0&&YearTmp%4==0){
			if(*(t+3)>29){
				*(t+3)=1;
				*(t+2)=3;
			}
		}
		else{
			if(*(t+3)>28){
				*(t+3)=1;
				*(t+2)=3;
			}
		}
	}
	else if(*(t+3)<1){
		switch(*(t+2)){
			case 2:
			case 4:
			case 6:
			case 8:
			case 9:
			case 11:
			case 1:
				*(t+3)=31;
				(*(t+2))--;
				break;
			case 5:
			case 7:
			case 10:
			case 12:
				*(t+3)=30;
				(*(t+2))--;
				break;
			case 3:
				if(YearTmp%100==0&&YearTmp%400==0){
					*(t+3)=29;
					*(t+2)=2;
				}
				else if(YearTmp%100!=0&&YearTmp%4==0){
					*(t+3)=29;
					*(t+2)=2;
				}
				else{
					*(t+3)=28;
					*(t+2)=2;
				}
				break;
		}
	}

	if(*(t+2)>12){
		*(t+2)=1;
		(*(t+1))++;
	}
	else if(*(t+2)<1){
		*(t+2)=12;
		(*(t+1))--;
	}
	
	if(*(t+1)>99) *(t+1)=0;
	else if(*(t+1)<0) *(t+1)=99;
	
	if(*t>99) *t=0;
	else if(*t<0) *t=99;
}

void WeekRefresh(signed char *t){//由年月日求星期的蔡勒公式
	unsigned char c,y,m,d;
	signed char Result;
	if(*(t+2)==1||*(t+2)==2){
		if(*(t+1)==0) c=*t-1;
		else c=*t;
		y=*(t+1)-1;
		m=*(t+2)+12;
		d=*(t+3);
	}
	else{
		c=*t;
		y=*(t+1);
		m=*(t+2);
		d=*(t+3);
	}
	Result=c/4-2*c+y/4+y+13*(m+1)/5+d-1;
	Result=Result%7;
	if(Result<0) Result+=7;
	Week=Result;
}

void NongLiRefresh(){
unsigned char i=0;
	if(Time[0]==20&&Time[1]==17){
		switch(Time[2]){
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10: break;
			case 11:
				if(Time[3]<=17){
					nongli[0]=9;
					nongli[1]=Time[3]-1+13;
				}
				else{
					nongli[0]=10;
					nongli[1]=Time[3]-17;
				}
				break;
			case 12:
				if(Time[3]<=17){
					nongli[0]=10;
					nongli[1]=Time[3]-1+14;
				}
				else{
					nongli[0]=11;
					nongli[1]=Time[3]-17;
				}
				break;
		}
	}
	else if(Time[0]==20&&Time[1]==18){
		switch(Time[2]){
			case 1:
				if(Time[3]<=16){
					nongli[0]=11;
					nongli[1]=Time[3]-1+15;
				}
				else{
					nongli[0]=12;
					nongli[1]=Time[3]-16;
				}
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12: break;
		}
	}
}