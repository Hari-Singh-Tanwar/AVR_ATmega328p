#include <avr/io.h>
#include <util/delay.h>

#define true 1

int main(void)
{
	// DDRB |= (1<<PB5); // 0b 0010 0000
	DDRB |= 0x20;
	
	while(true)
	{
		PORTB |= 0x20;
		_delay_ms(500);
		PORTB &= (~0x20);
		_dealy_ms(500);
	}
}
