#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

volatile short int curr;

void ADC_init(){
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

short int ADC_read(){
	ADCSRA |= (1<<ADSC); // initiate conversion
	while ((ADCSRA & (1<<ADSC)));
	return (ADC);
}

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

int main(void)
{
    usart_init();
	ADC_init();
	DDRB = 0xFF;
	
	while (1){
		curr = ADC_read();
		/* INSERT CODE HERE */
    }
}

