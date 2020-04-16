#include <reg52.h>
extern unsigned char code LedChar[];
extern unsigned char LedBuff[];
unsigned char RunFlag=0;
unsigned char Time[]={0,0,0,0};
unsigned char Rec[][4]={
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
};
unsigned char RecFlag[]={0,0,0,0,  0,0,0,0};
signed char RecInx=-1;
void SecBuffer_Time(){
	unsigned char i=0;
	for(i=0;i<4;i++) LedBuff[i]=LedChar[Time[i]];
}
void SecBuffer_Rec(unsigned char index){
	unsigned char i=0;
	for(i=0;i<4;i++) LedBuff[i]=LedChar[Rec[index][i]];
}
void SecRec(unsigned char index){
	unsigned char i=0;
	if(RecFlag[index]==0){
		for(i=0;i<4;i++){
			Rec[index][i]=Time[i];
		}
		RecFlag[index]=1;
		SecBuffer_Rec(index);
	}
	RecInx=index;
	SecBuffer_Rec(index);
}
void SecAdd(){
	unsigned char i=0;
	if (RunFlag==1){
		Time[3]++;
		if(Time[3]>9){
			Time[3]=0;
			Time[2]++;
			if(Time[2]>9){
				Time[2]=0;
				Time[1]++;
				if(Time[1]>9){
				Time[1]=0;
				Time[0]++;
					if(Time[0]>=6){
						Time[0]=0;	
					}
				}
			}
		}
	}
	if(RecInx==-1) SecBuffer_Time();
}
void SecReset(){
	unsigned char i=0;
	RunFlag=0;
	RecInx=-1;
	for(i=0;i<4;i++){
		Time[i]=0;
		LedBuff[i]=LedChar[0];
	}
	for(i=0;i<8;i++) RecFlag[i]=0;
}
void SecStaShift(){
	unsigned char i=0;
	if(RecInx==-1){
		if(RunFlag==0) RunFlag=1;
		else RunFlag=0;
	}
	else
		RecInx=-1;
}