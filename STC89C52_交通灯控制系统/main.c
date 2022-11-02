#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
#define uchar unsigned char   //�궨��"uchar"����"unsigned char"
#define uint  unsigned int	  //�궨��"uint"���������޷���������

//����ܶ�ѡ���� 0     1    2    3 	  4	   5	6	 7	  8	   9	
uchar code smg_du[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
//					   A	B	 C	  D	   E   F	����ʾ
					 0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};	 //??	

uchar dis_smg[8] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8};	
uchar smg_i = 2;    //��ʾ����ܵĸ�λ��

//�����λѡ����
sbit smg_we1 = P2^0;	    //�����λѡ����
sbit smg_we2 = P2^1;
sbit smg_we3 = P3^6;
sbit smg_we4 = P3^7;

char dx_s = 0;	//����  �ϱ� ����ʱ����
sbit dx_red    = P2^4;	  //�������
sbit dx_green  = P2^3;		//�����̵�
sbit dx_yellow = P2^2;		//�����Ƶ�

sbit nb_red    = P2^7;		//�ϱ����
sbit nb_green  = P2^6;		//�ϱ��̵�
sbit nb_yellow = P2^5;		//�ϱ��Ƶ�	

uchar flag_jtd_mode;  //��ͨ�Ƶ�ģʽ ����ʱ��
bit flag_1s = 0;
bit flag_500ms;
bit flag_dx_nb;		 //�����ϱ�ģʽ
uchar flag_5m_value;
uchar i;
uchar flag_alarm;	//ģʽ
uchar dx_time = 30,nb_time = 20;   //�������ϱ�ģʽ
uchar flag_jdgz ;     //��ͨ����

/*�����λѡ����*/
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
* ���� : delay_1ms()
* ���� : ��ʱ1ms����
* ���� : q
* ��� : ��
***********************************************************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<110;j++);
}

/********************************************************************
* ���� : display()
* ���� : �������ʾ
* ���� : ��
* ��� : ��
***********************************************************************/
void display()
{
	uchar i;
	for(i=0;i<smg_i;i++)
	{		
		P0 = 0xff;	        //����
		smg_we_switch(i);  	//λѡ
		P0 = dis_smg[i];	  //��ѡ
		delay_1ms(3);
	}
}

/*��ʱ��0,����ʱ��1��ʼ��*/
void time0_init()	  
{
	EA  = 1;	 	  //�����ж�
	TMOD = 0X11;	//��ʱ��0��ʱ��1������ʽ1
	ET0 = 1;		  //����ʱ��0�ж�
	TR0 = 1;		  //����ʱ��0��ʱ
//	ET1 = 1;		//����ʱ��1�ж�
//	TR1 = 1;		//����ʱ��1��ʱ
}

/*��ͨ�ƴ�����*/
void jiaotongdeng_dis()
{
	if(flag_1s == 1)
	{
		flag_1s = 0;
		if(dx_s == 0)
		{
			if(flag_dx_nb == 1)
				dx_s = nb_time;	  //�ϱ�ʱ��
			else 
				dx_s = dx_time;	  //����ʱ��
			flag_dx_nb = ~flag_dx_nb;
		}
		dx_s --;
	}
	dis_smg[0] = smg_du[dx_s % 10] ;
	dis_smg[1] = smg_du[dx_s / 10] ;
	
	dis_smg[2] = smg_du[dx_s % 10] ;
	dis_smg[3] = smg_du[dx_s / 10] ;

/*�ϱ�ʱ��*/
		if(flag_dx_nb == 0)  
		{
			if(dx_s > 5)
			{
				dx_red    = 1;  //��
				dx_green  =	0;	//��
				dx_yellow = 1;	//��
				nb_red    = 0;  //��
				nb_green  =	1;	//��
				nb_yellow = 1;	//��
				flag_5m_value = 0;	
			}else if(dx_s <= 5)		 //��С��5��ʱ �Ƶ���˸
			{
				dx_red    = 1;    //��
				dx_green  =	1;	  //��
				nb_red    = 0;    //��
				nb_green  =	1;	  //��
				nb_yellow = 1;	  //��
				if(flag_500ms == 0)
				{
					dx_yellow = 0;//��
				}
				else 
				{
					dx_yellow = 1;//��
				}
			}
		}
/*����ʱ��*/
		if(flag_dx_nb == 1)  
		{
			if(dx_s > 5)
			{
				dx_red    = 0;     //��
				dx_green  =	1;	   //��
				dx_yellow = 1;	   //��
				nb_red    = 1;     //��
				nb_green  =	0;	   //��
				nb_yellow = 1;	   //��
				flag_5m_value = 0;	
			}else if(dx_s <= 5)		 //��С��5��ʱ  �Ƶ���˸
			{
				dx_red    = 0;     //��
				dx_green  =	1;	   //��
				dx_yellow = 1;	   //��
				nb_red    = 1;     //��
				nb_green  =	1;	   //��
				if(flag_500ms == 0)	   //�Ƶ���˸
				{
					nb_yellow = 0;	//��
				}
				else 
				{
					nb_yellow = 1;	//��
				}
			}
	}			
}

/*������������*/
uchar key_can;	 //����ֵ

void key()	 //������������
{
	static uchar key_new;
	key_can = 20;                   //����ֵ��ԭ
	P1 |= 0x1f;
	if((P1 & 0x1f) != 0x1f)		//��������
	{
		delay_1ms(1);	     	//��������
		if(((P1 & 0x1f) != 0x1f) && (key_new == 1))
		{						//ȷ���ǰ�������
			key_new = 0;
			switch(P1 & 0x1f)
			{
				case 0x1e:  key_can = 1;  break;	 //�õ�����ֵ
				case 0x1d:  key_can = 2;  break;	 //�õ�����ֵ
				case 0x1b:  key_can = 3;  break;	 //�õ�����ֵ
				case 0x17:  key_can = 4;  break;	 //�õ�����ֵ
			}
		}			
	}
	else 
		key_new = 1;	
}

uchar menu_1,flag_s;

/*���ú���*/
void key_with()
{
	if(key_can == 4)   //��ͨ���ư���
	{
		flag_jdgz ++;
		if(flag_jdgz > 5)
			flag_jdgz = 0;	
		if(flag_jdgz == 1)	 //  ȫ������� 
		{
			dx_red    = 0;  //��
			dx_green  =	1;	//��
			dx_yellow = 1;	//��
			nb_red    = 0;  //��
			nb_green  =	1;	//��
			nb_yellow = 1;	//��		
		}
		if(flag_jdgz == 2)	 //  �������  �ϱ��̵�
		{
			dx_red    = 0;  //��
			dx_green  =	1;	//��
			dx_yellow = 1;	//��
			nb_red    = 1;  //��
			nb_green  =	0;	//��
			nb_yellow = 1;	//��		
		}
		if(flag_jdgz == 3)	 //  �ϱ����  �����̵�
		{
			dx_red    = 1;  //��
			dx_green  =	0;	//��
			dx_yellow = 1;	//��
			nb_red    = 0;  //��
			nb_green  =	1;	//��
			nb_yellow = 1;	//��		
		}
		if(flag_jdgz == 4)	 //  �ϱ��̵�  �����̵�
		{
			dx_red    = 1;  //��
			dx_green  =	0;	//��
			dx_yellow = 1;	//��
			nb_red    = 1;  //��
			nb_green  =	0;	//��
			nb_yellow = 1;	//��		
		}
		if(flag_jdgz == 5)	 //  �ϱ��Ƶ�  �����Ƶ�
		{
			dx_red    = 1;  //��
			dx_green  =	1;	//��
			dx_yellow = 0;	//��
			nb_red    = 1;  //��
			nb_green  =	1;	//��
			nb_yellow = 0;	//��		
		}
	}
	if(key_can == 1)	  //���ü�
	{
		menu_1 ++;
		if(menu_1 >= 3)
		{
			menu_1  = 0;
		}
	}

	if(menu_1 == 1)	   //���ö�����ʱ��
	{
		if(key_can == 2)
		{
			dx_time ++ ;		//��1
			if(dx_time > 99)
				dx_time = 99;
		}
		if(key_can == 3)
		{
			dx_time -- ;		//��1
			if(dx_time <= 10)
				dx_time = 10;
		}
//		dis_smg[0] = smg_du[10] ;	//��ʾΪA
//		dis_smg[1] = smg_du[10] ;	//��ʾΪA
		dis_smg[2] = smg_du[dx_time % 10] ;		//��ʾ�������õ�ʱ��
		dis_smg[3] = smg_du[dx_time / 10] ;	
	}	
	if(menu_1 == 2)	   //�����ϱ�ʱ��
	{
		if(key_can == 2)
		{
			nb_time ++ ;		//��1
			if(nb_time > 99)
				nb_time = 99;
		}
		if(key_can == 3)
		{
			nb_time -- ;		//��1
			if(nb_time <= 10)
				nb_time = 10;
		}
//		dis_smg[0] = smg_du[11] ;	//��ʾΪB
//		dis_smg[1] = smg_du[11] ;	//��ʾΪB
		dis_smg[2] = smg_du[nb_time % 10] ;		//��ʾ�������õ�ʱ��
		dis_smg[3] = smg_du[nb_time / 10] ;	
	}
}

/********************************************************************
* ���� : main()
* ���� : ʵ�ֵƵ���˸
* ���� : ��
* ��� : ��
***********************************************************************/
void main()
{
	time0_init();
	dx_s = nb_time;	  //����ʱ��
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

/*��ʱ��0�жϷ������*/
void time0_int() interrupt 1  
{						   
	static uchar value;			 //��ʱ10ms�ж�һ��
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