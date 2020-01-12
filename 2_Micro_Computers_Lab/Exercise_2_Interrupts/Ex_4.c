#include <avr/io.h>
#include <avr/interrupt.h>

int volatile flag;

void initInterrupt(){
	
	GICR = (1<<INT1);
	MCUCR = (1<<ISC11) | (1<<ISC10);
}

void initTIMER1(){
	
	TCCR1B = (1<<CS10) | (1<<CS12);
	TIMSK = (1 << TOIE1) ;
}

ISR(TIMER1_OVF_vect){ // TIMER1 ISR
	
	if (flag == 1){
		flag = 0;
		PORTB = 0x00;
	}
	else if (flag == 2){
		flag--;
		PORTB = 0x01;
		TCNT1 = 0x85EE;
	}
}

ISR(INT1_vect){
	
	if (flag == 0){
		flag++;
		PORTB = 0x01;
		TCNT1 = 0x85EE; // 4 seconds delay
	}
	else if (flag == 1){
		flag++;
		PORTB = 0xFF;
		TCNT1 = 0xF0BD; // 1/2 a second delay
	}
}

int main(void){
	
	DDRA = 0x00; // initializing PORTA as input
	DDRB = 0xFF; // initializing PORTB as output
	
	flag = 0; // initialization of flag
	
	initInterrupt();
	initTIMER1();	
	sei(); // enabling global interrupts
	
    while (1) {
		
		if ((PINA & 0x80) == 0x80){
			
			while ((PINA & 0x80) == 0x80);
			
			if (flag == 0){
				flag++;
				PORTB = 0x01;
				TCNT1 = 0x85EE; // 4 seconds delay
			}
			else if (flag == 1){
				flag++;
				PORTB = 0xFF;
				TCNT1 = 0xF0BD; // 1/2 a second delay
			}
		}
    }
	return 0;
}