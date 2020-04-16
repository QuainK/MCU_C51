/* Game-Snake 贪吃蛇
作者：钱坤 QuainK
版本：v1.0.0
日期：2018.08.11
环境：此工程使用51系列的 MCU STC89C5x[RC]，使用 KST51 开发板。
简介：贪吃蛇2018。按上下左右方向键，控制8*8点阵里蛇的移动以及蛇吃食物。
更新：
1.修复：解决v1.0.0-alpha中出现的点阵闪烁严重，蛇身过长出现丢失或瞬移等问题。
2.新增：1.新增数码管显示蛇身长度和得分；2.可以选择3种难度，不同难度蛇移动速度不同，得分不同。
3.修改：按键扫描、蛇身移动、蛇身变长和食物随机出现的业务代码放在主函数 main() 的 while 循环中，减轻中断函数执行的负担。
*/

#include <reg52.h>
#include <intrins.h>//char位移所需头文件
#include <stdlib.h>//随机数所需头文件
#define MAX 7 //随机数最大值
#define MIN 0 //随机数最小值

sbit ADDR0 = P1 ^ 0;//LED点阵和数码管
sbit ADDR1 = P1 ^ 1;
sbit ADDR2 = P1 ^ 2;
sbit ADDR3 = P1 ^ 3;
sbit ENLED = P1 ^ 4;
sbit KEYOUT1 = P2 ^ 3;//按键KeyOut
sbit KEYOUT2 = P2 ^ 2;
sbit KEYOUT3 = P2 ^ 1;
sbit KEYOUT4 = P2 ^ 0;

void ScanLed();//扫描LED显示(包括点阵和数码管)
void ScanKey();//扫描按键
void Delay(unsigned int ms);//延迟函数
void MoveBody();//移动蛇身。改变蛇身的坐标（包括并特殊处理蛇头）
void DataToBuffer();//数据缓冲。将蛇身的坐标算入LED缓冲中，准备显示
unsigned char GetHex(signed char index);
/*1位低电平的8位掩码。参数是低电平在char中位置，返回值是对应的十六进制。
例如参数index=3，返回值是0xf7
*/
void RandFood();//随机生成食物坐标
void JudgeEatFood();//判断是否吃到食物。以蛇头和食物坐标重合为准
void LengthenBody();//伸长蛇身。在吃到食物后立刻调用

unsigned char code LedChar[] = { //数码管数字显示字符表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90};
unsigned char buffer_matrix[8] = {//LED点阵缓冲
    0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char buffer_digitron[6] = {//数码管缓冲
    0xc0, 0xc0, 0xff, 0xff, 0xb0, 0xc0};
signed char body[11][2] = {//蛇身坐标（包括蛇头），二维数组，每一行是一节蛇身；每节中前一个是行号，后一个是列号，坐标对应点阵。
    0, 2,//蛇头
    0, 1,//蛇身
    0, 0};//蛇尾
unsigned char direction = 0; //蛇运动方向，0右1下2左3上
bit flag_movebody = 0;//蛇身移动标志位
unsigned char length = 3;//蛇长，初始3
unsigned char food_row = 5;//食物坐标行号
unsigned char food_col = 5;//食物坐标列号
unsigned int level_time = 500;//难度等级，有500ms、250ms、125ms三种
unsigned char score = 0;//游戏得分

void main()//主函数
{
    unsigned char i = 0;
    EA = 1;
    ENLED = 0;
    TMOD = 0x01;
    TH0 = 0xfc;
    TL0 = 0x67;
    ET0 = 1;
    TR0 = 1;
    while (1)
    {
        ScanKey();
        if ((score <= 30) && (flag_movebody == 1))
        {
            flag_movebody = 0;
            MoveBody();
            DataToBuffer();
            JudgeEatFood();
        }

        if (score > 30)
        {
            for (i = 0; i < 8; ++i)
                buffer_matrix[i] = 0x00;
            for (i = 0; i < 6; ++i)
                buffer_digitron[i] = 0x00;
        }
    }
}

void InterruptTimer0() interrupt 1
{
    static unsigned int cnt = 0;
    TH0 = 0xfc;
    TL0 = 0x67;
    ScanLed();
    ++cnt;
    if (cnt >= level_time)
    {
        cnt = 0;
        flag_movebody = 1;
    }
}

void ScanLed()
{
    static unsigned char index = 0;
    P0 = 0xff;
    switch (index)
    {
        case 0: ADDR3 = 0; ADDR2 = 0; ADDR1 = 0; ADDR0 = 0; break; 
        case 1: ADDR2 = 0; ADDR1 = 0; ADDR0 = 1; break; 
        case 2: ADDR2 = 0; ADDR1 = 1; ADDR0 = 0; break; 
        case 3: ADDR2 = 0; ADDR1 = 1; ADDR0 = 1; break; 
        case 4: ADDR2 = 1; ADDR1 = 0; ADDR0 = 0; break; 
        case 5: ADDR2 = 1; ADDR1 = 0; ADDR0 = 1; break; 
        case 6: ADDR2 = 1; ADDR1 = 1; ADDR0 = 0; break; 
        case 7: ADDR2 = 1; ADDR1 = 1; ADDR0 = 1; break; 
        case 8: ADDR3 = 1; ADDR2 = 0; ADDR1 = 0; ADDR0 = 0; break; 
        case 9: ADDR2 = 0; ADDR1 = 0; ADDR0 = 1; break; 
        case 10: ADDR2 = 0; ADDR1 = 1; ADDR0 = 0; break; 
        case 11: ADDR2 = 0; ADDR1 = 1; ADDR0 = 1; break; 
        case 12: ADDR2 = 1; ADDR1 = 0; ADDR0 = 0; break; 
        case 13: ADDR2 = 1; ADDR1 = 0; ADDR0 = 1; break; 
        default: break;
    }
    if (index < 8)
        P0 = buffer_matrix[index];
    else
        P0 = buffer_digitron[index-8];
    ++index;
    if (index > 13)
        index = 0;
}

void ScanKey()
{
    unsigned char row, temp;
    for (row = 0; row < 4; ++row)
    {
        P2 = _cror_(0xf7, row); //char round right
        temp = P2;
        temp = temp & 0xf0;
        if (temp != 0xf0)
        {
            Delay(5);
            temp = P2;
            temp = temp & 0xf0;
            if (temp != 0xf0)
            {
                temp = P2;
                switch (temp)
                {
                    case 0xe7: level_time = 500; break; 
                    case 0xd7: level_time = 250; break; 
                    case 0xb7: level_time = 125; break; 
                    case 0x77: if (direction != 1) direction = 3; break; //up 
                    case 0x7b: if (direction != 0) direction = 2; break; //left 
                    case 0x7d: if (direction != 3) direction = 1; break; //down 
                    case 0x7e: if (direction != 2) direction = 0; break; //right 
                    default: break;
                }
            }
        }
    }
}


void Delay(unsigned int ms)
{
    unsigned int x, y;
    for (x = ms; x > 0; --x)
        for (y = 110; y > 0; --y);
}


void MoveBody()
{
    unsigned char i;
    for (i = length - 1; i >= 1; --i){
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
    switch (direction)
    {
        case 0: ++body[0][1]; break; 
        case 1: ++body[0][0]; break; 
        case 2: --body[0][1]; break; 
        case 3: --body[0][0]; break; 
        default: break;
    }
    if (body[0][0] < 0) body[0][0] = 7;
    else if (body[0][0] > 7) body[0][0] = 0;
    if (body[0][1] < 0) body[0][1] = 7;
    else if (body[0][1] > 7) body[0][1] = 0;
}


void DataToBuffer()
{
    unsigned char i = 0;
    static bit time_light = 0;

    for (i = 0; i < 8; ++i)
        buffer_matrix[i] = 0xff;
    for (i = 0; i < length; ++i)
        buffer_matrix[body[i][0]] = buffer_matrix[body[i][0]] & GetHex(body[i][1]);

    time_light=~time_light;
    if (time_light ==0)
        buffer_matrix[food_row] = buffer_matrix[food_row] & GetHex(food_col);
    else
        buffer_matrix[food_row] = buffer_matrix[food_row] | (~GetHex(food_col));

    buffer_digitron[0] = LedChar[score % 10];
    buffer_digitron[1] = LedChar[score / 10 % 10];
    buffer_digitron[2] = LedChar[score / 100 % 10];

    buffer_digitron[4] = LedChar[length % 10];
    buffer_digitron[5] = LedChar[length / 10 % 10];
}

unsigned char GetHex(signed char index)
{
    switch (index)
    {
    case 0: return 0xfe; 
    case 1: return 0xfd; 
    case 2: return 0xfb; 
    case 3: return 0xf7; 
    case 4: return 0xef; 
    case 5: return 0xdf; 
    case 6: return 0xbf; 
    case 7: return 0x7f; 
    default: return 0x00;
    }
}

void RandFood()
{
    unsigned char i = 0;
    food_row = rand() % (MAX + 1 - MIN) + MIN;
    food_col = rand() % (MAX + 1 - MIN) + MIN; //获取一个随机数(0-7)
    for (i = 0; i < length; ++i)
    {
        if ((food_row != body[i * 2]) && (food_col != body[i * 2 + 1]))
            continue;
        else
            RandFood();
    }
}

void JudgeEatFood()
{
    if ((body[0][0] == food_row) && (body[0][1] == food_col))
    {
        LengthenBody();
        if (level_time == 500)
            ++score;
        else if (level_time == 250)
            score += 2;
        else if (level_time == 125)
            score += 4;

        RandFood();
    }
}

void LengthenBody()
{
    ++length;
    body[length - 1][0] = body[length - 2][0];
    body[length - 1][1] = body[length - 2][1];
}