/*
 * 3ex1inC.cpp
 * Created: 13/11/2019 16:11:55
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

unsigned char nibbleSwap(unsigned char a) {
	// swaps Nibbles of a 8-bit variable
	return (a<<4) | (a>>4);
}

int scan_row(short row) {
	// scans one row for a pressed character
	DDRC = 0xF0;
	PORTC = 0b00001000 << row;	// left shift of 0b0000 1000 as many times as row
	asm ("nop");
	asm ("nop");
	return (PINC & 0x0F);
	}
	
short int scan_keypad(void) {
	// scans keypad for pressed characters, returns keypad register (key_reg) 
	// with the 0/1 state of all the characters  
	char r25 = 0;             // 8-bit reg
	char r24 = 0;             // 8-bit reg
	short int key_reg = 0;    // 16-bit reg for the values of the whole keypad
	r25 = scan_row(1);		  
	r25 = nibbleSwap(r25);    // MSBs <--> LSBs
	r25 = scan_row(2);
	r24 = scan_row(3);
	r24 = nibbleSwap(r24);    // MSBs <--> LSBs
	r24 = scan_row(4);
	key_reg = r25;
	key_reg <<= 8;
	key_reg += r24;
	return key_reg;
}

char keypad_to_ascii(short int key_reg) {
	//converts key_reg bits to ASCII characters, if no key is pressed returns '?' 
	switch (key_reg)
	{
		case 1:
			return '*';
		case 2:
			return '0';
		case 4:
			return '#';
		case 8:
			return 'D';
		case 16:
			return '7';
		case 32:
			return '8';
		case 64:
			return '9';
		case 128:
			return 'C';
		case 256:
			return '4';
		case 512:
			return '5';
		case 1024:
			return '6';
		case 2048:
			return 'B';
		case 4096:
			return '1';
		case 8192:
			return '2';
		case 16384:
			return '3';
		case 32768:
			return 'A';
		default:
			return '?';
	}
}
	

int main(void) {
	/* scans 2 digits from keypad, if the digits match the password '0','5' 
	   all PORTB LEDs light-up for 4 seconds, else all PORTB LEDs blink for
	   4 seconds, the program operates continuously 
	*/
	DDRC = 0xF0; // Initializing I/O
	PORTC = 0;
    DDRB = 0xFF;
	PORTB = 0;
	char dig1, dig2;
	short int scan, repeat_scan;
		
    while (1) // repeatedly scan for PASSWORD: "05", 2-digit scan and coordinate LEDs appropriately
    {
		scan = scan_keypad();			// scan digit1 
		_delay_ms(20);					// wait 20 ms
		repeat_scan = scan_keypad();	// debouncing check
		scan &= repeat_scan;
		dig1 = keypad_to_ascii(scan);   // convert to ASCII
		while ( dig1 == '?' ) {			// while no key is pressed repeat
			scan = scan_keypad();
			_delay_ms(20);				// wait 20 ms
			repeat_scan = scan_keypad();// debouncing check
			scan &= repeat_scan;
			dig1 = keypad_to_ascii(scan);
		}
		scan = scan_keypad();			// scan digit2
		_delay_ms(20);					// wait 20 ms
		repeat_scan = scan_keypad();	// debouncing check
		scan &= repeat_scan;
		dig2 = keypad_to_ascii(scan);   // convert to ASCII
		while ( dig2 == '?') {			// while no key is pressed repeat
			scan = scan_keypad();
			_delay_ms(20);				// wait 20 ms
			repeat_scan = scan_keypad();// debouncing check
			scan &= repeat_scan;
			dig2 = keypad_to_ascii(scan);
		}
		if ((dig1 == '0') && (dig2 == '5')) { // password correct, light all PORTB LEDs
			PORTB = 0xFF;
			_delay_ms(4000);
			PORTB = 0x00;
		} 
		else 
			for (int i = 1; i <= 8; i++) {   // password incorrect, blink all PORTB LEDs for 4 sec
				PORTB = 0xFF;
				_delay_ms(250);
				PORTB = 0x00;
				_delay_ms(250);
			}	
	}
	
}

