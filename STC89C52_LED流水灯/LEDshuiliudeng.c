#include<reg52.h>
sbit D1=P2^0;
sbit D2=P2^1;
sbit D3=P2^2;
sbit D4=P2^3;
sbit D5=P2^4;
sbit D6=P2^5;
sbit D7=P2^6;
sbit D8=P2^7;

void Delay(unsigned int t)
{
	while(t--);
}
void LED_shan()
{
	char i;
	for(i=0;i<3;i++)
	{
	P2=0x00;
	Delay(10000);
	P2=0xff;
	Delay(10000);	
	}
}
void LED_run()
{
	D1=0;Delay(10000);
	D2=0;Delay(10000);
	D3=0;Delay(10000);
	D4=0;Delay(10000);
	D5=0;Delay(10000);
	D6=0;Delay(10000);
	D7=0;Delay(10000);
	D8=0;Delay(10000);
	
	D1=1;Delay(10000);
	D2=1;Delay(10000);
	D3=1;Delay(10000);
	D4=1;Delay(10000);
	D5=1;Delay(10000);
	D6=1;Delay(10000);
	D7=1;Delay(10000);
	D8=1;Delay(10000);
	
	D1=0;Delay(10000);
	D2=0;Delay(10000);
	D3=0;Delay(10000);
	D4=0;Delay(10000);
	D5=0;Delay(10000);
	D6=0;Delay(10000);
	D7=0;Delay(10000);
	D8=0;Delay(10000);
	
	D8=1;Delay(10000);
	D7=1;Delay(10000);
	D6=1;Delay(10000);
	D5=1;Delay(10000);
	D4=1;Delay(10000);
	D3=1;Delay(10000);
	D2=1;Delay(10000);
	D1=1;Delay(10000);
}
void main()
{
	while(1)
	{
		LED_shan();
		LED_run();
	}
}