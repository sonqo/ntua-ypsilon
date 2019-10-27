#include <avr/io.h>
#include <avr/interrupt.h>

int volatile x, flag;

ISR(TIMER1_OVF_vect){	// TIMER1 ISR
	
	if (flag == 1){
		flag--;
		x = 0x00;
		PORTB = x;
	}
	else if (flag == 2){
		flag = 0;
		x = 0x01;
		PORTB = x;
		TCNT1 = 0x85EE;
	}
}

ISR(INT1_vect){
	
}

int main(void){
	
	DDRA = 0X00;		// initializing PORTD as input
	DDRB = 0XFF;		// initializing PORTB as output
	
	x = 0;				// initialization of LED variable
	flag = 0;			// initialization of flag
	
	TCCR1B = (1<<CS10) | (1<<CS12);			// 1/1024 prescaler
	TIMSK = (1 << TOIE1) ; 
	
	sei();
	
    while (1) {
		
		if ((PINA & 0X01) == 1){
			
			if (flag == 0){
				x = 0x01;
				flag++;
				PORTB = x;
				TCNT1 = 0x85EE;				// 4 seconds delay
			}
			else{
				flag++;
				x = 0xFF;
				PORTB = x;
				TCNT1 = 0xF0BD;				// 1/2 a second delay
			}
		}
    }
	return 0;
}

