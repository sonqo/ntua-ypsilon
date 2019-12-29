#include <avr/io.h>

void usart_init(){
	UCSRA = 0x00;
	UCSRB |= (1<<RXEN)|(1<<TXEN); // activate transmitter/receiver
	UBRRH = 0x00; // BAUD rate: 9600
	UBRRL = 0x51;
	UCSRC |= (1<<URSEL)|(3<<UCSZ0); // 8bit character size, 1 stop-bit
}

void usart_transmit(char data){
	while ((UCSRA & 0x10) != 0x10){}
	UDR = data;
}

char usart_receive(){
	while ((UCSRA & 0x80) != 0x80){}
	return UDR;
}

void msg_invalid(){
	usart_transmit('I');
	usart_transmit('n');
	usart_transmit('v');
	usart_transmit('a');
	usart_transmit('l');
	usart_transmit('i');
	usart_transmit('d');
	usart_transmit(' ');
	usart_transmit('N');
	usart_transmit('u');
	usart_transmit('m');
	usart_transmit('b');
	usart_transmit('e');
	usart_transmit('r');
}

void msg_valid(char data){
	usart_transmit('R');
	usart_transmit('e');
	usart_transmit('a');
	usart_transmit('d');
	usart_transmit(' ');
	usart_transmit(data);
}

int main(void){
	char curr;
	
	usart_init(); // initializing USART
	DDRB = 0XFF; // initializing PORTB for output
	
    while (1){
		curr = usart_receive();
		if ((curr >=0) && (curr <=8)){
			msg_valid(curr);
			PORTB = curr;
		}
		else{
			msg_invalid();
		}
	}
}