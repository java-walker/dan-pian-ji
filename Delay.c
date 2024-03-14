#include <REGX52.H>
#include <intrins.h>

void delay(unsigned char a)		//@11.03MHz
{
	
	while(a--){
		unsigned char i=2;
		while(--i);
	}
}