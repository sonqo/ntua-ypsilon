#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

volatile char curr;

void usart_init(){
	UCSRA = 0x00;
	UCSRB |= (1<<RXEN)|(1<<TXEN); // activate transmitter/receiver
	UBRRH = 0x00; // BAUD rate: 9600
	UBRRL = 0x51;
	UCSRC |= (1<<URSEL)|(3<<UCSZ0); // 8bit character size, 1 stop-bit
}

void usart_transmit(char data){
	while (!( UCSRA & (1<<UDRE)));
	UDR = data;
}

char usart_receive(){
	while (!(UCSRA) & (1<<RXC));
	return UDR;
}

int hex_to_ascii(int number){ // converts a hex number to the ASCII code of the respective character
	switch (number){
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

int ascii_to_hex(int character){ // converts a hex number to the ASCII code of the respective character
	switch (character){
		case '0':
		return 0x00;
		case '1':
		return 0x01;
		case '2':
		return 0x02;
		case '3':
		return 0x03;
		case '4':
		return 0x04;
		case '5':
		return 0x05;
		case '6':
		return 0x06;
		case '7':
		return 0x07;
		case '8':
		return 0x08;
		case '9':
		return 0x09;
		default:
		return 0xFF;
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

void lcd_display_valid(void){
	
	lcd_init();
	
	lcd_data('R');
	lcd_data('e');
	lcd_data('a');
	lcd_data('d');
	lcd_data('i');
	lcd_data('n');
	lcd_data('g');
	lcd_data(' ');
	lcd_data(curr);

	lcd_command(0x02); // cursor home command
}

void lcd_display_invalid(void){
	
	lcd_data('I');
	lcd_data('n');
	lcd_data('v');
	lcd_data('a');
	lcd_data('l');
	lcd_data('i');
	lcd_data('d');
	lcd_data(' ');
	lcd_data('N');
	lcd_data('u');
	lcd_data('m');
	lcd_data('b');
	lcd_data('e');
	lcd_data('r');

	lcd_command(0x02); // cursor home command
}

char hamming_distance(char number){
	char res = 0x00;
	for (int i=0; i<8; i++){
		if ((number & 0x01) == 0x01){
			res <<= 1;
			res++;
		}
		number >>= 1;
	}
	return res;
}

int main(void){
	
	lcd_init(); // initializing LCD
	usart_init(); // initializing USART
	DDRB = 0XFF; // initializing PORTB for output
	
    while (1){
		curr = usart_receive();
		lcd_display_valid();
		curr = ascii_to_hex(curr);
		if ((curr >= 0) && (curr <=8)){
			PORTB = hamming_distance(curr);
		}
		else{
			lcd_display_invalid();
		}
	}
}