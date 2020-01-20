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

char hex_to_ascii(int character){ // converts a hex number to the ASCII code of the respective character
	switch (character){
		case 0:
		return '0';
		case 1:
		return '1';
		case 2:
		return '2';
		case 3:
		return '3';
		case 4:
		return '4';
		case 5:
		return '5';
		case 6:
		return '6';
		case 7:
		return '7';
		case 8:
		return '8';
		case 9:
		return '9';
		default:
		return 0xFF;
	}
}

int main(void)
{
    usart_init();
	ADC_init();
	
	while (1){
		curr = ADC_read();
		double Vin = (double) curr * (5.0) / 1024;
		int digit = (int) Vin;                // isolate first digit
		usart_transmit(hex_to_ascii(digit));  // transmit integer digit in UART
		usart_transmit('.');				  // transmit '.' symbol in UART
		Vin -= digit;						  // clear first Most Significant decimal Digit
		Vin *= 10;
		digit = (int) Vin;				      // isolate first decimal digit
		usart_transmit(hex_to_ascii(digit));  // transmit first decimal digit in UART
		Vin -= digit;                         // clear first Most Significant decimal Digit
		Vin *= 10;
		digit = (int) Vin;					  // isolate second decimal digit
		usart_transmit(hex_to_ascii(digit));  // transmit second decimal digit in UART
		usart_transmit('\n');
		_delay_ms(1000);
    }
}
