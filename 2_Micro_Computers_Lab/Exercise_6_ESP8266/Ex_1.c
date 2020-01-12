#define F_CPU 8000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

volatile char curr;

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
	
	lcd_command(0x01); // clear display
	
	while (*message != '\0'){
		lcd_data(*message);
		message++;
	}
	lcd_command(0x02); // cursor-home
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

void usart_string(char* message){
	while (*message != '\0'){
		usart_transmit(*message);
		message++;
	}
}

int main(void){
	
	lcd_init();
	usart_init();
	
	char connect = "connect";
	char teamname = "Team 05\n";
	
	char success_1 = "1. Success";
	char fail_1 = "1. Fail";
	
	char success_2 = "2. Success";
	char fail_2 = "2. Fail";
	
    while (1){
		usart_string(teamname);
		curr = usart_receive();
		if (strcmp(curr, "Success") == 0)
			lcd_display(success_1);
		else
			lcd_display(fail_1);
		_delay_ms(2000);
		usart_string(connect);
		curr = usart_receive();
		if (strcmp(curr, "Success") == 0)
			lcd_display(success_1);
		else
			lcd_display(fail_1);
    }
}