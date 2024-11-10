#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

// #define F_CPU 16000000L
#define BAUD_RATE 9600UL // 9600 bits/sec
#define UBBR_VAL ((F_CPU / (BAUD_RATE * 16UL)) - 1)

//*********LED variables******//
#define RED (1 << PD6)   // OC0A
#define GREEN (1 << PD5) // OC0B
#define BLUE (1 << PD3)  // OC2B

#define PWM_RED OCR0A
#define PWM_GREEN OCR0B
#define PWM_BLUE OCR2B
//************** UART **************/
void uart_init(void)
{
  UBRR0L = (uint8_t)UBBR_VAL;
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   // Enabling Rx and Tx
  UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Setting frame format to 8-bit, 1 stop bot & no parity
}

uint8_t uart_read(void) // Recives UART data
{
  while (!(UCSR0A & (1 << RXC0)))
    ; // wait until the data is recived
  return UDR0;
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

/*********************ADC*******************/

void adc_init(void) // Initialising of the ADC
{
  ADMUX |= (1 << REFS0); // Setting the ADC to the referince voltage

  ADCSRA |= (1 << ADEN);                                // enabling the ADC
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // Prescaling the ADC 16MHz/128
}

uint16_t adc_read(uint8_t channel) // reading the adc value
{
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Read the adc on the pin
  ADCSRA |= (1 << ADSC);                     // Start the converstion
  while (ADCSRA & (1 << ADSC))
    ;
  return ADC;
}
/******************************************/

/******************PWM*********************/
void pwm_init(void) // Iniitalising of PWM
{
  /// Timer0 ///
  // Setting The Timer0 in Fast PWM mode && Clear OCR0A on compare match
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
  // Prescaling timer by 64
  TCCR0B |= (1 << CS00) | (1 << CS01); // 16MHz / (256 * 64) = 976Hz

  /// Timer2 ///
  // Setting The Timer2 in Fast PWM mode || Clear OCR2A & OCR2B on compare match
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  // Prescaling timer by 64
  TCCR2B |= (1 << CS20) | (1 << CS21); // 16MHz / (256 * 64) = 976Hz
}

/******************************************/
int main(void)
{
  // Setting PortD3, PortD5 & PortD6 to Output
  DDRD |= GREEN | BLUE | RED;

  uart_init(); // Intialising UART
  adc_init();  // Initialising ADC
  pwm_init();  // Initialising PWM

  while (true)
  {
    /// Controlling Red led ///
    PWM_RED = adc_read(3) / 4;

    /// Controlling Green led ///
    PWM_GREEN = adc_read(1) / 4;

    /// Controlling Blue led ///
    PWM_BLUE = adc_read(2) / 4;

    uart_print("Red: %d, Green: %d, Blue: %d\n", PWM_RED, PWM_GREEN, PWM_BLUE);

    _delay_ms(50);
  }
}
