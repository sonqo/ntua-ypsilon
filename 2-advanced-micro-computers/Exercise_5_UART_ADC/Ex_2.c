#define F_CPU 8000000UL
 
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

volatile char* curr;
volatile char curr_hex;

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

char ascii_to_hex(int character){ // converts a hex number to the ASCII code of the respective character
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

void usart_transmit_string(char* message){
	
	while (*message != '\0'){
		usart_transmit(*message);
		message++;
	}
}

char hamming_distance(char number){
	
	char res = 0x00;

	for (int i=0; i<number; i++){
			res <<= 1;
			res++;
	}
	return res;
}

int main(void){
	
	char temp;
	char valid = "Reading ";
	char invalid = "Invalid Number";

	usart_init(); // initializing USART
	
	DDRB = 0XFF; // initializing PORTB for output

    while (1){
		curr = usart_receive_string();
		if (strlen(curr) == 1){
			curr_hex = ascii_to_hex(curr[0]);
			usart_string(valid);
			usart_transmit(curr[0]);
			usart_transmit('\n');
			if ((curr_hex >= 0) && (curr_hex <=8)){
				PORTB = hamming_distance(curr_hex);
			}
			else{
				usart_string(invalid);
				usart_transmit('\n');	
			}
		}
		else{
			usart_string(valid);
			usart_transmit_string(curr);
			usart_transmit('\n');
			usart_string(invalid);
			usart_transmit('\n');
		}
	}
}
