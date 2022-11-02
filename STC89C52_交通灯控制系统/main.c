#include <reg52.h>	         //调用单片机头文件
#define uchar unsigned char   //宏定义"uchar"代替"unsigned char"
#define uint  unsigned int	  //宏定义"uint"用来定义无符号整型数

//数码管段选定义 0     1    2    3 	  4	   5	6	 7	  8	   9	
uchar code smg_du[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
//					   A	B	 C	  D	   E   F	不显示
					 0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};	 //??	

uchar dis_smg[8] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8};	
uchar smg_i = 2;    //显示数码管的个位数

//数码管位选定义
sbit smg_we1 = P2^0;	    //数码管位选定义
sbit smg_we2 = P2^1;
sbit smg_we3 = P3^6;
sbit smg_we4 = P3^7;

char dx_s = 0;	//东西  南北 倒计时变量
sbit dx_red    = P2^4;	  //东西红灯
sbit dx_green  = P2^3;		//东西绿灯
sbit dx_yellow = P2^2;		//东西黄灯

sbit nb_red    = P2^7;		//南北红灯
sbit nb_green  = P2^6;		//南北绿灯
sbit nb_yellow = P2^5;		//南北黄灯	

uchar flag_jtd_mode;  //交通灯的模式 根据时间
bit flag_1s = 0;
bit flag_500ms;
bit flag_dx_nb;		 //东西南北模式
uchar flag_5m_value;
uchar i;
uchar flag_alarm;	//模式
uchar dx_time = 30,nb_time = 20;   //东西、南北模式
uchar flag_jdgz ;     //交通管制

/*数码管位选函数*/
void smg_we_switch(uchar i)
{
	switch(i)
	{
		case 0: smg_we1 = 0;  smg_we2 = 1; smg_we3 = 1;  smg_we4 = 0; break;
		case 1: smg_we1 = 1;  smg_we2 = 0; smg_we3 = 0;  smg_we4 = 1; break;
		case 2: smg_we1 = 1;  smg_we2 = 1; smg_we3 = 0;  smg_we4 = 1; break;
		case 3: smg_we1 = 1;  smg_we2 = 1; smg_we3 = 1;  smg_we4 = 0; break;
	}	
}

/********************************************************************
* 名称 : delay_1ms()
* 功能 : 延时1ms函数
* 输入 : q
* 输出 : 无
***********************************************************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<110;j++);
}

/********************************************************************
* 名称 : display()
* 功能 : 数码管显示
* 输入 : 无
* 输出 : 无
***********************************************************************/
void display()
{
	uchar i;
	for(i=0;i<smg_i;i++)
	{		
		P0 = 0xff;	        //消隐
		smg_we_switch(i);  	//位选
		P0 = dis_smg[i];	  //段选
		delay_1ms(3);
	}
}

/*定时器0,、定时器1初始化*/
void time0_init()	  
{
	EA  = 1;	 	  //开关中断
	TMOD = 0X11;	//定时器0定时器1工作方式1
	ET0 = 1;		  //开定时器0中断
	TR0 = 1;		  //允许定时器0定时
//	ET1 = 1;		//开定时器1中断
//	TR1 = 1;		//允许定时器1定时
}

/*交通灯处理函数*/
void jiaotongdeng_dis()
{
	if(flag_1s == 1)
	{
		flag_1s = 0;
		if(dx_s == 0)
		{
			if(flag_dx_nb == 1)
				dx_s = nb_time;	  //南北时间
			else 
				dx_s = dx_time;	  //东西时间
			flag_dx_nb = ~flag_dx_nb;
		}
		dx_s --;
	}
	dis_smg[0] = smg_du[dx_s % 10] ;
	dis_smg[1] = smg_du[dx_s / 10] ;
	
	dis_smg[2] = smg_du[dx_s % 10] ;
	dis_smg[3] = smg_du[dx_s / 10] ;

/*南北时间*/
		if(flag_dx_nb == 0)  
		{
			if(dx_s > 5)
			{
				dx_red    = 1;  //灭
				dx_green  =	0;	//亮
				dx_yellow = 1;	//灭
				nb_red    = 0;  //亮
				nb_green  =	1;	//灭
				nb_yellow = 1;	//灭
				flag_5m_value = 0;	
			}else if(dx_s <= 5)		 //当小于5秒时 黄灯闪烁
			{
				dx_red    = 1;    //灭
				dx_green  =	1;	  //灭
				nb_red    = 0;    //亮
				nb_green  =	1;	  //灭
				nb_yellow = 1;	  //灭
				if(flag_500ms == 0)
				{
					dx_yellow = 0;//亮
				}
				else 
				{
					dx_yellow = 1;//灭
				}
			}
		}
/*东西时间*/
		if(flag_dx_nb == 1)  
		{
			if(dx_s > 5)
			{
				dx_red    = 0;     //亮
				dx_green  =	1;	   //灭
				dx_yellow = 1;	   //灭
				nb_red    = 1;     //灭
				nb_green  =	0;	   //亮
				nb_yellow = 1;	   //灭
				flag_5m_value = 0;	
			}else if(dx_s <= 5)		 //当小于5秒时  黄灯闪烁
			{
				dx_red    = 0;     //灭
				dx_green  =	1;	   //灭
				dx_yellow = 1;	   //灭
				nb_red    = 1;     //灭
				nb_green  =	1;	   //灭
				if(flag_500ms == 0)	   //黄灯闪烁
				{
					nb_yellow = 0;	//亮
				}
				else 
				{
					nb_yellow = 1;	//灭
				}
			}
	}			
}

/*独立按键程序*/
uchar key_can;	 //按键值

void key()	 //独立按键程序
{
	static uchar key_new;
	key_can = 20;                   //按键值还原
	P1 |= 0x1f;
	if((P1 & 0x1f) != 0x1f)		//按键按下
	{
		delay_1ms(1);	     	//按键消抖
		if(((P1 & 0x1f) != 0x1f) && (key_new == 1))
		{						//确认是按键按下
			key_new = 0;
			switch(P1 & 0x1f)
			{
				case 0x1e:  key_can = 1;  break;	 //得到按键值
				case 0x1d:  key_can = 2;  break;	 //得到按键值
				case 0x1b:  key_can = 3;  break;	 //得到按键值
				case 0x17:  key_can = 4;  break;	 //得到按键值
			}
		}			
	}
	else 
		key_new = 1;	
}

uchar menu_1,flag_s;

/*设置函数*/
void key_with()
{
	if(key_can == 4)   //交通管制按键
	{
		flag_jdgz ++;
		if(flag_jdgz > 5)
			flag_jdgz = 0;	
		if(flag_jdgz == 1)	 //  全部亮红灯 
		{
			dx_red    = 0;  //亮
			dx_green  =	1;	//灭
			dx_yellow = 1;	//灭
			nb_red    = 0;  //亮
			nb_green  =	1;	//灭
			nb_yellow = 1;	//灭		
		}
		if(flag_jdgz == 2)	 //  东西红灯  南北绿灯
		{
			dx_red    = 0;  //亮
			dx_green  =	1;	//灭
			dx_yellow = 1;	//灭
			nb_red    = 1;  //灭
			nb_green  =	0;	//亮
			nb_yellow = 1;	//灭		
		}
		if(flag_jdgz == 3)	 //  南北红灯  东西绿灯
		{
			dx_red    = 1;  //灭
			dx_green  =	0;	//亮
			dx_yellow = 1;	//灭
			nb_red    = 0;  //亮
			nb_green  =	1;	//灭
			nb_yellow = 1;	//灭		
		}
		if(flag_jdgz == 4)	 //  南北绿灯  东西绿灯
		{
			dx_red    = 1;  //灭
			dx_green  =	0;	//亮
			dx_yellow = 1;	//灭
			nb_red    = 1;  //灭
			nb_green  =	0;	//亮
			nb_yellow = 1;	//灭		
		}
		if(flag_jdgz == 5)	 //  南北黄灯  东西黄灯
		{
			dx_red    = 1;  //灭
			dx_green  =	1;	//灭
			dx_yellow = 0;	//亮
			nb_red    = 1;  //灭
			nb_green  =	1;	//灭
			nb_yellow = 0;	//亮		
		}
	}
	if(key_can == 1)	  //设置键
	{
		menu_1 ++;
		if(menu_1 >= 3)
		{
			menu_1  = 0;
		}
	}

	if(menu_1 == 1)	   //设置东西的时间
	{
		if(key_can == 2)
		{
			dx_time ++ ;		//加1
			if(dx_time > 99)
				dx_time = 99;
		}
		if(key_can == 3)
		{
			dx_time -- ;		//减1
			if(dx_time <= 10)
				dx_time = 10;
		}
//		dis_smg[0] = smg_du[10] ;	//显示为A
//		dis_smg[1] = smg_du[10] ;	//显示为A
		dis_smg[2] = smg_du[dx_time % 10] ;		//显示东西设置的时间
		dis_smg[3] = smg_du[dx_time / 10] ;	
	}	
	if(menu_1 == 2)	   //设置南北时间
	{
		if(key_can == 2)
		{
			nb_time ++ ;		//加1
			if(nb_time > 99)
				nb_time = 99;
		}
		if(key_can == 3)
		{
			nb_time -- ;		//减1
			if(nb_time <= 10)
				nb_time = 10;
		}
//		dis_smg[0] = smg_du[11] ;	//显示为B
//		dis_smg[1] = smg_du[11] ;	//显示为B
		dis_smg[2] = smg_du[nb_time % 10] ;		//显示东西设置的时候
		dis_smg[3] = smg_du[nb_time / 10] ;	
	}
}

/********************************************************************
* 名称 : main()
* 功能 : 实现灯的闪烁
* 输入 : 无
* 输出 : 无
***********************************************************************/
void main()
{
	time0_init();
	dx_s = nb_time;	  //东西时间
	while(1)
	{
		key();
		if(key_can < 20)
			key_with();
//		menu_dis();
		if((menu_1 == 0) && (flag_jdgz == 0))
			jiaotongdeng_dis();
		display();	
	}
}  

/*定时器0中断服务程序*/
void time0_int() interrupt 1  
{						   
	static uchar value;			 //定时10ms中断一次
	TH0 = 0x3c;
	TL0 = 0xb0;     //50ms
	value ++;
	flag_5m_value++;
	if(flag_5m_value % 10 == 0)
		flag_500ms = ~flag_500ms;
	if(value >= 20)
	{
		value = 0;
		flag_1s = 1;
	}
}