#include <reg52.h>

unsigned char code LedChar[]={//低电平有效
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,//0 1 2 3 4 5 6 7
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E//8 9 A B C D E F,0xBF是负号
};
unsigned char LedBuff[]={//数码管从左往右
	0xFF, 0xFF, 0xFF, 0xFF
};

//数码管动态扫描刷新函数，需在定时中断中调用
void LedScan(){
	static unsigned char i=0;//动态扫描索引
	P0=0xFF;//关闭所有段选，显示消隐
	P1=(P1&0xF8)|i;//位选索引值赋值到P1口低三位
	P0=LedBuff[i];//缓冲区中索引位置的数据送到P0口
	if(i<3)//索引递增循环，遍历整个缓冲区
		i++;
	else
		i=0;
}