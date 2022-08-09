#include<reg52.h>
sbit D1= P2^0;
void Delay(unsigned int t)
{
	while(t--);
}
void main()
{
	while(1)
	{
		D1=1;
		Delay(10000);
		D1=0;
		Delay(10000);
	}
}