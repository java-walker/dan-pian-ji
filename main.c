#include <REGX52.H>
#include <stdlib.h>//rand
#include <intrins.h>//位移
#include "Delay.h"
#include "MatrixLED.h"
 
 
sbit up=P3^1;
sbit down=P3^0;
sbit lt=P3^2;
sbit rt=P3^3;
 
unsigned char coordx[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe,};
unsigned char coordy[]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,};
 
unsigned char	 foodx,foody;//食物坐标
unsigned char   snake_x[20]={0};
unsigned char   snake_y[20]={0};
unsigned char   length;
unsigned char   direction;
unsigned char   died;
 
void snake_init();
void Keyboard();
void move();
void creatfood();
void snakegrow(void);
void xssnake();
void endd();
 
void snake_init()//初始化蛇 左上角开始，向下移动
{
    snake_x[0]=0;//1
    snake_y[0]=0;//1
    length=2;//身体长度，不加头
    direction=2;//向下
	died=0;
}
void move()//蛇的移动函数
{
    unsigned char len=length-1;
    for (len;len>0;len--)
    {
        snake_x[len]=snake_x[len-1];
        snake_y[len]=snake_y[len-1];
    }
    switch(direction)
    {
        case 1:
            snake_y[0]--;
            break;
        case 2:
            snake_y[0]++;
            break;
        case 3:
            snake_x[0]--;
            break;
        case 4:
            snake_x[0]++;
            break;
        default:
            break;
    }
    
}
 
void Keyboard()//按键检测
{
    if((up==0)&&(direction!=2))
    {
        Delay(10);//消抖
        if(up==0)
        {
            direction=1;
        }
        while(up==1);
    }
    if((down==0)&&(direction!=1))
    {
        Delay(10);
        if(down==0)
        {
            direction=2;
        }
        while(down==1);
    }
    if((lt==0)&&(direction!=4))
    {
        Delay(10);
        if(lt==0)
        {
            direction=3;
        }
        while(lt==1);
    }
    if((rt==0)&&(direction!=3))
    {
        Delay(10);
        if(rt==0)
        {
            direction=4;
        }
        while(rt==1);
    }
}
 
void creatfood()//creat食物
{
    unsigned char i;
    foodx=rand()%8;
    foody=rand()%8;
    for(i=0; i<length; i++)
    {
        if((foodx==snake_x[i]) && (foody==snake_y[i]))
		{
            creatfood();
        }
    }
    
}
 
void snakegrow()
{
    if(foody==snake_y[0]&&foodx==snake_x[0])
    {
        creatfood();
        snake_x[length]=snake_x[length-1];
        snake_y[length]=snake_y[length-1];
        length++;
    }
}
 
void xssnake()//显示图案
{
	if(died==0){
    unsigned char i;
    for(i=0;i<length;i++)
    {
        MatrixLED_ShowColumn(coordy[snake_y[i]],coordx[snake_x[i]]);
        MatrixLED_ShowColumn(coordy[foody],coordx[foodx]);
    }}
}
 
void judgedeath()//判断死亡
{
    unsigned char i;
    if((snake_x[0]>7)||(snake_y[0]>7))
    {
		MatrixLED_Clear();
        died=1;  }
    for(i=4; i<length;i++)
    {
        if((snake_x[0]==snake_x[i])&&(snake_y[0]==snake_y[i]))
        {
            P0=0xFF;
            died=1;//jieweidonghua
        }
		
    }
}
void endd()
{
	unsigned char k;
    for(k=1;k<7;k++)
    {
        MatrixLED_ShowColumn(coordy[k],coordx[k]);
        MatrixLED_ShowColumn(coordy[k],coordx[7-k]);
    }
	if(up==0||down==0||lt==0||rt==0)
		    snake_init();
}
void Timer0Init(void)//定时器
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0=0XF0;		//设置定时初值
	TH0=0XD8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}

void Timer0() interrupt 1
{
	static unsigned int i;
	TL0=0XF0;		//设置定时初值10ms
	TH0=0XD8;		//设置定时初值
	i++;
	if(i==50)
	{
		i=0;
		move();
	}
}
 
void main()
{
    Timer0Init();
    snake_init();
    while (1)
    {   
		if(died==0){
    xssnake();
    snakegrow();
    Keyboard();
    judgedeath();}
	if(died==1){
	endd();
	}
	

    }
    
}