/* Game-Snake 贪吃蛇
作者：钱坤 QuainK
版本：v1.0.0-alpha
日期：2017.08.29
环境：此工程使用51系列的 MCU STC89C5x[RC]，使用 KST51 开发板。
简介：贪吃蛇2017。按上下左右方向键，控制8*8点阵里蛇的移动以及蛇吃食物。
问题：点阵闪烁严重，蛇身过长出现丢失或瞬移等。
*/

#include <reg52.h>
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit KEY_IN_1=P2^4;
sbit KEY_IN_2=P2^5;
sbit KEY_IN_3=P2^6;
sbit KEY_IN_4=P2^7;
sbit KEY_OUT_1=P2^3;
sbit KEY_OUT_2=P2^2;
sbit KEY_OUT_3=P2^1;
sbit KEY_OUT_4=P2^0;

unsigned char code KeyCodeMap[4][4]={
	{0x31, 0x32, 0x33, 0x26},
	{0x34, 0x35, 0x36, 0x25},
	{0x37, 0x38, 0x39, 0x28},
	{0x30, 0x1B, 0x0D, 0x27}
};
unsigned char KeySta[4][4]={
	{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}
};
unsigned char LedBuff[8]={
	248,255,255,255,255,255,255,255
};
unsigned char LedBuff01[8][8]={
	{0,0,0,1,1,1,1,1},//0
	{1,1,1,1,1,1,1,1},//1
	{1,1,1,1,1,1,1,1},//2
	{1,1,1,1,1,1,1,1},//3
	{1,1,1,1,1,1,1,1},//4
	{1,1,1,1,1,1,1,1},//5
	{1,1,1,1,1,1,1,1},//6
	{1,1,1,1,1,1,1,1},//7

};
signed char body[3][2]={
	{0,2},{0,1},{0,0}
};

unsigned char direction=2,backup=2;

void KeyAction(unsigned char keycode);
void KeyDriver();
void KeyScan();
void LedScan();
void Move();
unsigned char BIN_DEC(unsigned long x);
unsigned long Power(unsigned char x,unsigned char n);

void main(){
	EA=1;
	ENLED=0;
	ADDR3=0;
	TMOD=0x01;
	TH0=0xFC;
	TL0=0x67;
	ET0=1;
	TR0=1;
	while(1){
		KeyDriver();
	}
}

void KeyAction(unsigned char keycode){
	direction=0*(keycode==0x25)+1*(keycode==0x26)+2*(keycode==0x27)+3*(keycode==0x28);//0左1上2右3下
	if(((backup+direction)==2)||((backup+direction)==4))//不准掉头，因为0左+2右=2，或者1上+3下=4
		direction=backup;
	else
		backup=direction;
}

void KeyDriver(){
	unsigned char i,j;
	static unsigned char backup[4][4]={
		{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}
	};
	for(i=0;i<4;i++){
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
	static unsigned char keybuf[4][4]={
		{0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}, 
		{0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}
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
	keyout++;
	keyout=keyout&0x03;
	switch(keyout){
		case 0: KEY_OUT_4=1; KEY_OUT_1=0; break;
		case 1: KEY_OUT_1=1; KEY_OUT_2=0; break;
		case 2: KEY_OUT_2=1; KEY_OUT_3=0; break;
		case 3: KEY_OUT_3=1; KEY_OUT_4=0; break;
		default: break;
	}
}

void LedScan(){
	static unsigned char i=0;
	switch(i){
		case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedBuff[0]; break;
		case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedBuff[1]; break;
		case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedBuff[2]; break;
		case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedBuff[3]; break;
		case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedBuff[4]; break;
		case 5: ADDR2=1; ADDR1=0; ADDR0=1; i++; P0=LedBuff[5]; break;
		case 6: ADDR2=1; ADDR1=1; ADDR0=0; i++; P0=LedBuff[6]; break;
		case 7: ADDR2=1; ADDR1=1; ADDR0=1; i=0; P0=LedBuff[7]; break;
		default: break;
	}
}

void Move(){//蛇身的移动，转换成LED点阵的变化
	unsigned char i=0;
	unsigned long tmp=11111111;
	LedBuff01[body[2][0]][body[2][1]]=1;//蛇尾的灯熄灭
	body[2][0]=body[1][0];
	body[2][1]=body[1][1];
	body[1][0]=body[0][0];
	body[1][1]=body[0][1];
	switch(direction){
		case 0: body[0][1]--; break;
		case 1: body[0][0]--; break;
		case 2: body[0][1]++; break;
		case 3: body[0][0]++; break;
		default: break;
	}
		if(body[0][0]<0) body[0][0]=7;//蛇可以穿墙的特殊技能（先改变蛇头）
	if(body[0][0]>7) body[0][0]=0;
	if(body[0][1]<0) body[0][1]=7;
	if(body[0][1]>7) body[0][1]=0;

		LedBuff01[body[0][0]][body[0][1]]=0;
	for(i=0;i<=7;i++){
		tmp=10000000*LedBuff01[i][7]+1000000*LedBuff01[i][6]+100000*LedBuff01[i][5]+10000*LedBuff01[i][4]+1000*LedBuff01[i][3]+100*LedBuff01[i][2]+10*LedBuff01[i][1]+LedBuff01[i][0];
		LedBuff[i]=BIN_DEC(tmp);
	}
}

unsigned char BIN_DEC(unsigned long x){//无符长整型二进制转换无符成整型十进制
	unsigned char num[8];//num[8]每个位置存放一个二进制数
	unsigned long numd=0;//numd存放十进制结果
	signed char i=0;
	for(i=7;i>=0;i--){
		num[i]=(x%2);
		x=x/10;
		numd=(numd+Power(2,7-i)*num[i]);//用按权展开公式，调用幂函数Power()
	}
	return numd;//返回十进制结果给主调函数
}

unsigned long Power(unsigned char x,unsigned char n){//幂函数，形参x底数，形参n指数
	unsigned char i=0;
	unsigned long shu=1;
	for(i=0;i<n;i++){
		shu=shu*x;//实质就是同一个数的固定次数的累乘
	}
	return shu;//返回结果
}

void InterruptTimer0() interrupt 1{
	static unsigned int cnt=0;
	cnt++;
	TH0=0xFC;
	TL0=0x67;
	if(cnt>=500){
		cnt=0;
		Move();
	}
	LedScan();
	KeyScan();
	
}