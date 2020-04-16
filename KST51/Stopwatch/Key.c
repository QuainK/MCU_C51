#include <reg52.h>
sbit KEY_OUT_1=P2^0;
sbit KEY_OUT_2=P2^1;
sbit KEY_OUT_3=P2^2;
sbit KEY_IN_1=P2^4;
sbit KEY_IN_2=P2^5;
sbit KEY_IN_3=P2^6;
sbit KEY_IN_4=P2^7;

extern void SecRec(unsigned char index);
extern void SecStaShift();
extern void SecReset();

void KeyAction(unsigned char keycode);

unsigned char code KeyCodeMap[][4]={//按键对应的ASCII,总共10个按键
	{0x31, 0x32, 0x33, 0x34},//数字键1-4
	{0x35, 0x36, 0x37, 0x38},//数字键5-8
	{0x00, 0x00, 0x11, 0x12},//设备控制键1，设备控制键2
};
unsigned char KeySta[][4]={
	{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}
};


void KeyDriver(){
	unsigned char i,j;
	static unsigned char backup[][4]={
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1}
	};
	for(i=0;i<3;i++){
		for(j=0;j<4;j++){
			if(backup[i][j]!=KeySta[i][j]){
				if(backup[i][j]!=0){
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j]=KeySta[i][j];
			}
		}
	}
}

void KeyScan(){
	unsigned char i;
	static unsigned char keyout=0;
	static unsigned char keybuf[][4]={
		{0xFF, 0xFF, 0xFF, 0xFF}, 
		{0xFF, 0xFF, 0xFF, 0xFF}, 
		{0xFF, 0xFF, 0xFF, 0xFF}
	};
	keybuf[keyout][0]=(keybuf[keyout][0]<<1)|KEY_IN_1;
	keybuf[keyout][1]=(keybuf[keyout][1]<<1)|KEY_IN_2;
	keybuf[keyout][2]=(keybuf[keyout][2]<<1)|KEY_IN_3;
	keybuf[keyout][3]=(keybuf[keyout][3]<<1)|KEY_IN_4;
	for(i=0;i<4;i++){
		if((keybuf[keyout][i]&0x0F)==0x00){
			KeySta[keyout][i]=0;
		}
		else if((keybuf[keyout][i]&0x0F)==0x0F){
			KeySta[keyout][i]=1;
		}
	}
	KeySta[2][0]=1;
	KeySta[2][1]=1;
	keyout++;
	if(keyout>=3) keyout=0;
	switch(keyout){
		case 0: KEY_OUT_3=1; KEY_OUT_1=0; break;
		case 1: KEY_OUT_1=1; KEY_OUT_2=0; break;
		case 2: KEY_OUT_2=1; KEY_OUT_3=0; break;
		default: break;
	}
}
void KeyAction(unsigned char keycode){
	switch(keycode){
		case (0x11): SecReset(); break; 
		case (0x12): SecStaShift(); break;
		case (0x31): SecRec(0); break;
		case (0x32): SecRec(1); break;
		case (0x33): SecRec(2); break;
		case (0x34): SecRec(3); break;
		case (0x35): SecRec(4); break;
		case (0x36): SecRec(5); break;
		case (0x37): SecRec(6); break;
		case (0x38): SecRec(7); break;

		default: break;
	}
}