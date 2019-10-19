#include <avr/io.h>

int main (void) {
	
	DDRA = 0x00;
	DDRB = 0xFF;
	
	PORTB  = 0x00;
	
	while (1) {
		
		//F0
		if (((PINA & 0x07) == 7 ) || ((PINA & 0x0C) == 8))
			PORTB = (PORTB & 0x0E);
		else
			PORTB = (PORTB | 0x01);
			
		//F1
		if ( ((PINA & 0x01)  == 1 ) || ((PINA & 0x02) == 2) )
			if (((PINA & 0x04) == 4) || ((PINA & 0x08) == 8))
				PORTB = (PORTB | 0x02);
			else
				PORTB = (PORTB & 0x0D);
		else
			PORTB = (PORTB & 0x0D);
	}
	
	return 0;
}

