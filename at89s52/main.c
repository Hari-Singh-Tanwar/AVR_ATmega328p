#include <reg51.h>

sbit LED = P1^0; // Define the led pin

void delay(unsigned int count){
	unsigned int i, j;
	for(i = 0, i < count; i++)
		for(j = 0; j < 1275; j++)
}

void main(){
	while(1){
	LED = 0;
	delay(500);
	LED = 1;
	delay(500);
	}
}
