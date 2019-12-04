#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

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
	if ((scan & 0x10) == 0x10){
		return 0x00;
	}
	else{
		return 0x01;
	}
}

void one_wire_transmit_bit(char bit){
	
	bit   &= 0b00000001; // clear all but the LSB
	
	DDRA |= (1 << PA4); // PA4 configured for output
	PORTA &= ~(1 << PA4); // set PA4 to 0
	_delay_us(2);
	if (bit == 1){
		PORTA |= (1 << PA4);
	}
	else{
		PORTA &= ~(1 << PA4);
	}
	_delay_us(58);
	DDRA  &= ~(1 << PA4);
	PORTA &= ~(1 << PA4);
	_delay_us(1);
	return;
}

void one_wire_transmit_byte(char byte){
	for (int i=0; i<8; i++){
		if ((byte & 0x01) == 0x01){
			one_wire_transmit_bit(1);
		}
		else{
			one_wire_transmit_bit(0);
		}
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
	if ((PINA & 0x10 ) == 0x10){
		bit = 1;
	}
	else{
		bit =0;
	}
	_delay_us(49);
	return bit;
}

char one_wire_receive_byte(void){
	
	char bit;
	char byte = 0x00;
	
	for (int i=0; i<8; i++){
		bit = one_wire_receive_bit();
		byte >>= 1;
		if (bit == 1){
			byte |= 0x80;
		}
	}
	return byte;
}

short ds1820_routine(void) {
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
			result >>= 1; // rounding down temperature
			return result;
		}
		else{
			return 0x8000; // no device detected code
		}
	}
	else{
		return 0x8000;
	}
}

int main(void)
{
	DDRB = 0xFF; // initialize PORTB for output
	short temperature;
	
	while (1){
		temperature = ds1820_routine();
		if (temperature == 0x8000){ // in case no device is detected
			PORTB = temperature;
		}
		else{
			temperature &= 0x00FF; // ignore sign
			PORTB = temperature;
		}
	}
}