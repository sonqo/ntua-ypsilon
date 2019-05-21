#include <avr/io.h>

char x, y;

int main(void)
{
    DDRA = 0xFF; //initialization of PORTA for output
	DDRC = 0x00; //initialization of PORTC for input

	x = 1; //led0 initially on

    while (1){
		
		if ((PINC & 0x01) == 1){ //shift left
			while ((PINC & 0x01) == 1);
				if (x == 128)
					x = 1;
				else
					x = x<<1;
		}
		if ((PINC & 0x02) == 2){ //shift right
			while ((PINC & 0x02) == 2);
				if (x == 1)
					x = 128;
				else
					x = x>>1;
		}
		if ((PINC & 0x04) == 4){ //MSB led on
			while ((PINCy & 0x04) == 4);
				x = 128;
		}
		if ((PINC & 0x08) == 8){ //LSB led on
			while ((PINC & 0x08) == 8);
				x = 1;
		}
		
		PORTA = x;
    }

	return 0;
}

