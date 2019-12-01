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
		return 0x01;
	}
	else{
		return 0x00;
	}
}



int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

