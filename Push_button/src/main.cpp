#include <stdio.h>
// #include <stdarg.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_RATE 9600UL // 9600 bits/sec
#define UBBR_VAL ((F_CPU / (BAUD_RATE * 16UL)) - 1)

#define LED (1 << PD7)
#define SW (1 << PD2)
#define SW2 (1 << PB0)
#define LED_BUILTIN (1 << PB5)

volatile uint8_t a = 0; // increased by the interrupt
volatile bool flag = false;

//************** UART **************/
void uart_init(void)
{
  UBRR0L = (uint8_t)UBBR_VAL;
  UCSR0B |= (1 << TXEN0);                  // Enabling  Tx
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Setting frame format to 8-bit, 1 stop bot & no parity
}


void uart_print(const char *format, ...)
{
  char buffer[64];
  va_list args;
  va_start(args, format);                          // Start handling arguments after `format`
  vsnprintf(buffer, sizeof(buffer), format, args); // Format the arguments into `buffer`
  va_end(args);                                    // End argument processing

  // Send the formatted string over UART
  for (char *data = buffer; *data; data++)
  {
    while (!(UCSR0A & (1 << UDRE0)))
      ; // wait until tha data reagister is enpty
    UDR0 = (uint8_t)(*data);
  }
}

/********************************************/

/// Without Interrupt ///

// int main(void){
//   DDRD &= ~SW; // SWITCH AT D2
//   DDRD |= LED; // LED AT D7

//   while(1){
//     if(PIND & SW){
//       _delay_ms(250);
//       PORTD |= LED;
//     }
//     else{
//       _delay_ms(250);
//       PORTD &= ~ LED;
//     }
//   }
// }

/// With External interrupt + PCINT///

int main(void)
{
  // char UART_buffer[50];

  DDRD &= ~SW;  // SWITCH AT D2
  DDRD |= LED;  // LED AT D7
  DDRB &= ~SW2; // SWITCH AT D8
  PORTB |= SW2; // PULLUP RESISTOR ENABLED
  DDRB &= ~LED_BUILTIN;

  // Enabling PCINT intrrupt
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0);

  EICRA |= (1 << ISC01) | (1 << ISC00); // External Interrupt Control Register A -> (For any falling edge)
  // EICRA = 2;
  EIMSK |= (1 << INT0); // External Interupt Mask Register (Enabling External intrupt 0)

  sei();
  uart_init();
  while (true)
  {
    if (flag)
    {
      uart_print("a = %d\n", a);
      flag = false;
    }
  }
}

ISR(INT0_vect)
{
  if (PIND & SW) // If buttons was pressed
  {
    PORTD ^= LED; // LED -> TOGGLE
  }
}

ISR(PCINT0_vect)
{
  if (!(PINB & SW2))
  {
    ++a;
    flag = true;
    PORTB ^= LED_BUILTIN; // LED -> ON
  }
}