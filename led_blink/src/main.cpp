// #define F_CPU 16000000L  // Defining the clock frequency (16 MHz for Arduino Uno/Nano)
#include <avr/io.h>     // AVR IO library
#include <util/delay.h> // For delay functions

// Define pin mappings for the LEDs (assuming the same pin numbers as in Arduino)
#define ledRed (1 << PD6)    // Pin 6 on Arduino corresponds to PD4
#define ledYellow (1 << PD5) // Pin 5 on Arduino corresponds to PD3
#define ledGreen (1 << PD3)  // Pin 3 on Arduino corresponds to PD2

#define dealy 500


int main(void)
{
  // Set pins PD4, PD3, PD2 as output (LEDs are connected to these pins)
  DDRD |= ledRed | ledYellow | ledGreen; // Set pins as output (DDR = Data Direction Register)
  // DDRD |= 28;

  while (1)
  {
    PORTD |= ledRed;       // Set PD4 high (turn on LED Merah)
    PORTD &= ~(ledYellow); // Set PD3 low (turn off LED Kuning)
    PORTD &= ~(ledGreen);  // Set PD2 low (turn off LED Hijau)
    _delay_ms(dealy);      // Delay 5 seconds

    PORTD &= ~(ledRed);   // Set PD4 low (turn off LED Merah)
    PORTD |= ledYellow;   // Set PD3 high (turn on LED Kuning)
    PORTD &= ~(ledGreen); // Set PD2 low (turn off LED Hijau)
    _delay_ms(dealy);     // Delay 2 seconds

    PORTD &= ~(ledRed);    // Set PD4 low (turn off LED Merah)
    PORTD &= ~(ledYellow); // Set PD3 low (turn off LED Kuning)
    PORTD |= ledGreen;     // Set PD2 high (turn on LED Hijau)
    _delay_ms(dealy);      // Delay 5 seconds

    PORTD &= ~(ledRed);   // Set PD4 low (turn off LED Merah)
    PORTD |= ledYellow;   // Set PD3 high (turn on LED Kuning)
    PORTD &= ~(ledGreen); // Set PD2 low (turn off LED Hijau)
    _delay_ms(dealy);     // Delay 2 seconds
  }
  return 0;
}


///************* Timer interrupts *************///
// #include <avr/io.h>
// #include <avr/interrupt.h>

// // #define F_CPU 1000000L
// // // Define pin mappings for the LEDs (assuming the same pin numbers as in Arduino)
// #define ledRed (1 << PD6)    // Pin 6 on Arduino corresponds to PD4 - OC0A
// #define ledYellow (1 << PD5) // Pin 5 on Arduino corresponds to PD3 - OC0B
// #define ledGreen (1 << PD3)  // Pin 3 on Arduino corresponds to PD2 - OC2B

// int main(void)
// {
//   DDRD |= ledRed | ledYellow | ledGreen; // Set pins as output (DDR = Data Direction Register)

//   TCCR1B |= (1 << CS12);
//   TIMSK1 |= (1 << TOIE1);
//   sei();

//   while (true)
//     ; // infinite loop
// }

// ISR(TIMER1_OVF_vect)
// {
//   PORTD ^= ledRed;
//   PORTD ^= ledYellow;
//   PORTD ^= ledGreen;
// }