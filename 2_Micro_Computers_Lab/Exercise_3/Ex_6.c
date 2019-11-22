#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

volatile short int min_1, min_2, sec_1, sec_2;

void lcd_command(unsigned char command){
	
	PORTD = (PORTD & 0x0F) | (command & 0xF0); // sending upper nibble
	PORTD &= ~ (1<<PD2); // low PD2: sending command		
	PORTD |= (1<<PD3); // activate enable pulse 		
	//_delay_us(1);
	PORTD &= ~ (1<<PD3); // deactivate enable pulse
	//_delay_us(200);
	PORTD = (PORTD & 0x0F) | (command << 4); // sending lower nibble
	PORTD |= (1<<PD3);
	//_delay_us(1);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
}

void lcd_data(unsigned char data){
	
	PORTD = (PORTD & 0x0F) | (data & 0xF0); // sending upper nibble
	PORTD |= (1<<PD2); // high PD2: sending data		
	PORTD|= (1<<PD3);
	//_delay_us(1);
	PORTD &= ~ (1<<PD3);
	//_delay_us(200);
	PORTD = (PORTD & 0x0F) | (data << 4); // sending lower nibble
	PORTD |= (1<<PD3);
	//_delay_us(1);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
}

void lcd_init(void){
	
	DDRD = 0xFF; // initialize PORTD for output
	
	_delay_ms(40); // initial power on delay 
	PORTD = 0x30; // command switching to 8-bit mode - first
	PORTD |= (1<<PD3);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
	PORTD = 0x30; // command switching to 8-bit mode - second
	PORTD |= (1<<PD3);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
	PORTD = 0x20; // switch to 4-bit mode
	PORTD |= (1<<PD3);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
	lcd_command(0x28); // 2-line display, 5x8 character pattern
	lcd_command(0x0C); // hide cursor
	lcd_command(0x01); // clear screen
	_delay_us(1530);
	lcd_command(0x06); // automatic display increment, disable shifting
}

void zerofy(void){
	
	min_1 = 0x00;
	min_2 = 0x00;
	sec_1 = 0x00;
	sec_2 = 0x00;
}

void lcd_display(void){
	
	lcd_data('min_1');
	lcd_data('min_2');
	lcd_data(' ');
	lcd_data('M');
	lcd_data('I');
	lcd_data('N');
	lcd_data(' ');
	lcd_data(':');
	lcd_data('sec_1');
	lcd_data('sec_2');
	lcd_data(' ');
	lcd_data('S');
	lcd_data('E');
	lcd_data('C');

	lcd_command(0x02); // cursor home command
}

char hex_to_ascii(char key_reg){ // converts a hex number to the ASCII code of the respective character
	
	switch (key_reg){
		case 0x00:
			return '0';
		case 0x01:
			return '1';
		case 0x02:
			return '2';
		case 0x03:
			return '3';
		case 0x04:
			return '4';
		case 0x05:
			return '5';
		case 0x06:
			return '6';
		case 0x07:
			return '7';
		case 0x08:
			return '8';
		case 0x09:
			return '9';
	}
}	

int main(void){
	
	DDRB = 0x00; // initialization of PORTB for input
	lcd_init(); // initialization of LCD
	zerofy(); // initialize variables
		
    while (1){
		 
		while (PINB & 0x01 == 0x01){
			if (PINB & 0x80 == 0x80){
				zerofy();
			}
			sec_2++;
			if (sec_2 == 0x0A){
				sec_2 = 0x00;
				sec_1++;
				if (sec_1 == 0x07){
					sec_1 = 0x00;
					sec_2 = 0x00;
					min_2++;
					if (min_2 == 0x0A){
						sec_1 = 0x00;
						sec_2 = 0x00;
						min_2 = 0x00;
						min_1++;
						if (min_1 == 0x07){
							zerofy();
						}
					}
				}
			}
		}
		lcd_display();
	}
}