#define F_CPU 8000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

char volatile sign, hundreds, tens, ones;
char* curr1, *curr2, *curr3, *curr4, *curr5;

char one_wire_reset(void){
	
	char scan;
	
	DDRA |= (1 << PA4); // PA4 configured for output
	PORTA &= ~(1 << PA4); // set PA4 to 0
	_delay_us(480);
	DDRA &= ~(1 << PA4); // PA4 configured for input
	PORTA &= ~(1 << PA4); // set PA4 to 0
	_delay_us(100);
	scan = PINA; // sample the line
	_delay_us(380);
	if ((scan & 0x10) == 0x10)
	return 0x00;
	else
	return 0x01;
}

void one_wire_transmit_bit(char bit){
	
	bit   &= 0b00000001; // clear all but the LSB
	
	DDRA |= (1 << PA4); // PA4 configured for output
	PORTA &= ~(1 << PA4); // set PA4 to 0
	_delay_us(2);
	if (bit == 1)
	PORTA |= (1 << PA4);
	else
	PORTA &= ~(1 << PA4);
	_delay_us(58);
	DDRA  &= ~(1 << PA4);
	PORTA &= ~(1 << PA4);
	_delay_us(1);
	return;
}

void one_wire_transmit_byte(char byte){
	
	for (int i=0; i<8; i++){
		if ((byte & 0x01) == 0x01)
		one_wire_transmit_bit(1);
		else
		one_wire_transmit_bit(0);
		byte >>= 1; // right shift one place
	}
}

char one_wire_receive_bit(void){
	
	short bit;
	
	DDRA |= (1 << PA4);
	PORTA &= ~(1 << PA4);
	_delay_us(2);
	DDRA  &= ~(1 << PA4);
	PORTA &= ~(1 << PA4);
	_delay_us(10);
	if ((PINA & 0x10 ) == 0x10)
	bit = 1;
	else
	bit = 0;
	_delay_us(49);
	return bit;
}

char one_wire_receive_byte(void){
	
	char bit;
	char byte = 0x00;
	
	for (int i=0; i<8; i++){
		bit = one_wire_receive_bit();
		byte >>= 1;
		if (bit == 1)
		byte |= 0x80;
	}
	return byte;
}

int ds1820_routine(void) {
	
	int MSB,LSB;
	int result = 0;
	char control = 0x00;
	
	control = one_wire_reset();
	if (control == 0x01){
		one_wire_transmit_byte(0xCC);
		one_wire_transmit_byte(0x44);
		while (!one_wire_receive_bit()) ;
		control = one_wire_reset();
		if (control == 0x01) {
			one_wire_transmit_byte(0xCC);
			one_wire_transmit_byte(0xBE);
			LSB = one_wire_receive_byte();
			MSB = one_wire_receive_byte();
			result = (MSB << 8) + LSB;
			result >>= 4; // rounding down temperature
			return result;
		}
		else
		return 0x8000;
	}
	else
	return 0x8000;
}

void usart_init(){
	
	UCSRA = 0;
	UCSRB = (1<<RXEN)|(1<<TXEN); // activate transmitter/receiver
	UBRRH = 0; // BAUD rate: 9600
	UBRRL = 51;
	UCSRC = (1<<URSEL)|(3<<UCSZ0); // 8bit character size, 1 stop-bit
}

char usart_receive(){
	
	while (!(UCSRA & (1<<RXC))); // waiting till RXC is equal to 1
	return UDR;
}

void usart_transmit(char data){
	
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void usart_transmit_string(char* message){
	
	while (*message != '\0'){
		usart_transmit(*message);
		message++;
	}
}

char* usart_receive_string(){

	static char acc[50];
	for (int j = 0; j < 50; j++) acc[j] = '\0';

	char character = usart_receive();
	int i =0;
	while (character != '\n'){
		acc[i++] = character;
		character = usart_receive();
	}

	return acc;
}

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

char keypad_to_ascii(short int key_reg){ // converts a status register to the ASCII code of the first key pressed
	
	switch (key_reg){
		case 1:
		return '*';
		case 2:
		return '0';
		case 4:
		return '#';
		case 8:
		return 'D';
		case 16:
		return '7';
		case 32:
		return '8';
		case 64:
		return '9';
		case 128:
		return 'C';
		case 256:
		return '4';
		case 512:
		return '5';
		case 1024:
		return '6';
		case 2048:
		return 'B';
		case 4096:
		return '1';
		case 8192:
		return '2';
		case 16384:
		return '3';
		case 32768:
		return 'A';
		default:
		return '?'; // error handling
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

void lcd_display(char* message){

	while (*message != '\0'){
		lcd_data(*message);
		message++;
	}
	lcd_command(0x02); // cursor-home
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

int main(void){

	usart_init();
	
	lcd_init();
	
	one_wire_reset();

	DDRC = 0xF0; // initializing PORTC for keypad scanning

	/* TeamName command */

	while(1){

		usart_transmit_string("teamname: \"A5\"\n");
		curr1 = usart_receive_string();
		lcd_data('1');
		lcd_data('.');
		lcd_display(curr1);
		_delay_ms(3000);
		if ( strcmp( curr1 , "\"Success\"" ) != 0 ) continue; // if not "Success" restart

		/* Connect command */

		usart_transmit_string("connect\n");
		curr2 = usart_receive_string();
		lcd_init();
		lcd_data('2');
		lcd_data('.');
		lcd_display(curr2);
		_delay_ms(3000);
		if ( strcmp( curr2 , "\"Success\"" ) != 0 ) continue; // if not "Success" restart
		break;
	}

	while (1) { 	// loop

		lcd_command(0x01);
		
		int flag;
		short temperature;
		
		static char final[5];

		static char* message_start = "payload: [{\"name\": \"Temperature\",\"value\": ";
		static char* message_end = "}]\n";

		char dig;
		short int scan, repeat_scan;

		/* DS1820 payload command */

		temperature = ds1820_routine();
		if (temperature == 0x8000)
		usart_transmit_string("No Device!");
		else {
			flag = 0;
			ones = 0;
			tens = 0;
			hundreds = 0;
			if ((temperature & 0xF000) == 0xE000){
				sign = '-';
				temperature &= 0x00FF;
				temperature ^= 0xFF; // 2's complement in case of negative number
				temperature++;
			}
			else{
				sign = '+';
				temperature &= 0x00FF;
			}

			if (temperature >= 0x64){
				hundreds++;
				temperature -= 0x64;
			}
			while (temperature >= 0x0A){
				tens++;
				temperature -= 0x0A;
			}
			ones = temperature;

			final[0] = hex_to_ascii(tens);
			final[1] = hex_to_ascii(ones);
			final[2] = '\0';

			usart_transmit_string(message_start);
			usart_transmit_string(final);
			usart_transmit_string(message_end);

			curr3 = usart_receive_string();
			lcd_init();
			lcd_data('3');
			lcd_data('.');
			lcd_display(curr3);
			_delay_ms(3000);
			if ( strcmp( curr3 , "\"Success\"" ) != 0 ) continue; // if not "Success" restart

			/* Keypad command */

			scan = scan_keypad();
			_delay_ms(20);
			repeat_scan = scan_keypad(); // debouncing handling
			scan &= repeat_scan;
			dig = keypad_to_ascii(scan);
			if (dig == '5'){
				usart_transmit_string("ready: \"true\"\n");
				curr4 = usart_receive_string();
				lcd_init();
				lcd_data('4');
				lcd_data('.');
				lcd_display(curr4);

			}
			else
				continue;

			/* ESP8266 */

			usart_transmit_string("transmit\n");
			curr5 = usart_receive_string();
			lcd_display(curr5);
			_delay_ms(3000);
		}
	}
}