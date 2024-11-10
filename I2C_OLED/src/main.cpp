#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define PCF8574_ADDR 0x27  // Address of the PCF8574
#define BACKLIGHT 0x08     // Backlight bit position on PCF8574

void TWI_init(void) {
    TWBR = ((F_CPU / 100000L) - 16) / 2;  // Set bit rate to 100 kHz
    TWSR = 0x00;  // Set prescaler to 1
}

void TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void PCF8574_write(uint8_t data) {
    TWI_start();
    TWI_write(PCF8574_ADDR << 1);
    TWI_write(data | BACKLIGHT);  // Send data with backlight bit
    TWI_stop();
}

void LCD_send_nibble(uint8_t nibble) {
    // Send high nibble to D4-D7 and pulse EN
    PCF8574_write(nibble | (1 << 4)); // Set EN high
    _delay_us(1);
    PCF8574_write(nibble);            // Set EN low
    _delay_us(40);
}

void LCD_send_byte(uint8_t data, uint8_t rs) {
    uint8_t highNibble = (data & 0xF0) | (rs ? (1 << 6) : 0);
    uint8_t lowNibble = ((data << 4) & 0xF0) | (rs ? (1 << 6) : 0);

    LCD_send_nibble(highNibble);
    LCD_send_nibble(lowNibble);
}

void LCD_command(uint8_t cmd) {
    LCD_send_byte(cmd, 0);
    _delay_ms(2);
}

void LCD_data(uint8_t data) {
    LCD_send_byte(data, 1);
    _delay_ms(2);
}

void LCD_init(void) {
    _delay_ms(50);  // Wait for LCD to power up
    LCD_command(0x30);  // Initialize
    _delay_ms(5);
    LCD_command(0x30);
    _delay_ms(1);
    LCD_command(0x20);  // Set to 4-bit mode

    // Function Set: 4-bit mode, 2 lines, 5x8 font
    LCD_command(0x28);

    // Display Control: Display ON, Cursor OFF, Blink OFF
    LCD_command(0x0C);

    // Entry Mode Set: Increment cursor, No shift
    LCD_command(0x06);

    // Clear display
    LCD_command(0x01);
    _delay_ms(2);
}

void LCD_print(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

int main(void) {
    TWI_init();
    LCD_init();

    // Display "Hello, World!"
    LCD_command(0x80);  // Move cursor to the first line
    LCD_print("Hello, World!");

    while (1) {
        // Main loop
    }
}
