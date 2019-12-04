#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

char volatile sign, hundreds, tens, ones;

int scan_row(short n){ // scanning the nth row of keypad
	
	PORTC = 0b00001000 << n;
	asm ("nop");
	asm ("nop");
	return (PINC & 0x0F);
}

short int scan_keypad(void){ // scanning keypad for pressed characters, returns a 16-bit status register representing all keys
	
	int r24 = 0; // 8-bit reg
	int r25 = 0; // 8-bit reg
	short int key_reg = 0;
	
	r25 = scan_row(1);
	r25 = (r25 << 4) & 0xF0;
	r25 += scan_row(2);
	r24 = scan_row(3);
	r24 = (r24 << 4) & 0xF0;
	r24 += scan_row(4);
	key_reg = r25;
	key_reg <<= 8;
	key_reg += r24;
	return key_reg;
}

char keypad_to_hex(short int character){
	
	switch(character){
		case 1:
		return 0x0E;
		case 2:
		return 0x00;
		case 4:
		return 0x0F;
		case 8:
		return 0x0D;
		case 16:
		return 0x07;
		case 32:
		return 0x08;
		case 64:
		return 0x09;
		case 128:
		return 0x0C;
		case 256:
		return 0x04;
		case 512:
		return 0x05;
		case 1024:
		return 0x06;
		case 2048:
		return 0x0B;
		case 4096:
		return 0x01;
		case 8192:
		return 0x02;
		case 16384:
		return 0x03;
		case 32768:
		return 0x0A;
		default:
		return 0xAA; // error handling
	}
}

char hex_to_ascii(char number){
	
	switch(number){
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
		default:
		return 'A';
	}
}

void lcd_command(unsigned char command){
	
	PORTD = (PORTD & 0x0F) | (command & 0xF0); // sending upper nibble
	PORTD &= ~ (1<<PD2); // low PD2: sending command
	PORTD |= (1<<PD3); // activate enable pulse
	PORTD &= ~ (1<<PD3); // deactivate enable pulse
	PORTD = (PORTD & 0x0F) | (command << 4); // sending lower nibble
	PORTD |= (1<<PD3);
	PORTD &= ~ (1<<PD3);
	_delay_us(39);
}

void lcd_data(unsigned char data){
	
	PORTD = (PORTD & 0x0F) | (data & 0xF0); // sending upper nibble
	PORTD |= (1<<PD2); // high PD2: sending data
	PORTD|= (1<<PD3);
	PORTD &= ~ (1<<PD3);
	PORTD = (PORTD & 0x0F) | (data << 4); // sending lower nibble
	PORTD |= (1<<PD3);
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

void lcd_display(void){
	
	char hundreds_ascii, tens_ascii, ones_ascii;
	
	hundreds_ascii = hex_to_ascii(hundreds);
	tens_ascii = hex_to_ascii(tens);
	ones_ascii = hex_to_ascii(ones);
	
	lcd_data(sign);
	lcd_data(hundreds_ascii);
	lcd_data(tens_ascii);
	lcd_data(ones_ascii);

	lcd_command(0x02); // cursor home command
}

int main(void){
	
	DDRC = 0xF0; // initialize PORTC for keypad scanning
	DDRB = 0XFF;
	
	lcd_init();
	
	int flag;
	char dig1, dig2, sum;
	short int scan, repeat_scan;
	
	while (1){

		hundreds = 0;
		tens = 0;
		ones = 0;
		
		flag = 0;

		scan = scan_keypad();
		_delay_ms(50);
		repeat_scan = scan_keypad(); // debouncing handling
		scan &= repeat_scan;
		dig1 = keypad_to_hex(scan);
		while (dig1 == 0xAA){ // while no key is pressed, repeat
			scan = scan_keypad();
			_delay_ms(20);
			repeat_scan = scan_keypad();
			scan &= repeat_scan;
			dig1 = keypad_to_hex(scan);
		}
		flag++;
		_delay_ms(200);
		scan = scan_keypad();
		_delay_ms(20);
		repeat_scan = scan_keypad();
		scan &= repeat_scan;
		dig2 = keypad_to_hex(scan);
		while (dig2 == 0xAA){
			scan = scan_keypad();
			_delay_ms(20);
			repeat_scan = scan_keypad();
			scan &= repeat_scan;
			dig2 = keypad_to_hex(scan);
		}
		flag++;
		dig1 <<= 4;
		sum = dig1 + dig2;
		if ((sum & 0x80) == 0x80){
			sign = '-';
			sum ^= 0xFF;
			sum++;
		}
		else{
			sign = '+';
		}
		if (sum >= 0x64){
			hundreds++;
			sum -= 0x64;
		}
		while (sum >= 0x0A){
			tens++;
			sum -= 0x0A;
		}
		ones = sum;
		if (flag == 2){
			lcd_display(); // display only when 2 digits are read
		}
		_delay_ms(200);
	}
}