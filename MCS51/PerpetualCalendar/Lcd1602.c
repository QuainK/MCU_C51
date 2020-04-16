#include <reg52.h>

#define LCD1602_DB P0
sbit LCD1602_RS=P1^0;
sbit LCD1602_RW=P1^1;
sbit LCD1602_E=P1^2;

void LcdWaitReady(){
	unsigned char sta;
	LCD1602_DB=0xFF;
	LCD1602_RS=0;
	LCD1602_RW=1;
	do{
		LCD1602_E=1;
		sta=LCD1602_DB;
		LCD1602_E=0;
	}while(sta&0x80);
}

void LcdWriteCmd(unsigned char cmd){
	LcdWaitReady();
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_DB=cmd;
	LCD1602_E=1;
	LCD1602_E=0;
}

void LcdWriteDat(unsigned char dat){
	LcdWaitReady();
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DB=dat;
	LCD1602_E=1;
	LCD1602_E=0;
}

void LcdSetCursor(unsigned char x,unsigned char y){
	unsigned char addr;
	if(y==0)
		addr=0x00+x;
	else
		addr=0x40+x;
	LcdWriteCmd(addr|0x80);
}

void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str){
	LcdSetCursor(x,y);
	while(*str!='\0'){
		LcdWriteDat(*str++);
	}
}

void LcdInit(){
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0C);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}