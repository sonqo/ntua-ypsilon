#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int scan_row(short n){ // scanning the nth row of keypad
	
	PORTC = 0b00001000 << n;
	asm ("nop");
	asm ("nop");
	return (PINC & 0x0F);
}
	
short int scan_keypad(void){ // scanning keypad for pressed characters, returns a 16-bit status register representing all keys
	
	int r24 = 0; // 8-bit reg
	int r25 = 0; // 8-bit reg
	short int key_reg = 0;
	
	r25 = scan_row(1);
	r25 = (r25 << 4) & 0xF0;	  
	r25 += scan_row(2);
	r24 = scan_row(3);
	r24 = (r24 << 4) & 0xF0;
	r24 += scan_row(4);
	key_reg = r25;
	key_reg <<= 8;
	key_reg += r24;
	return key_reg;
}

char keypad_to_ascii(short int key_reg){ // converts a status register to the ASCII code of the first key pressed
	
	switch (key_reg){
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
			return '?'; // error handling
	}
}
	
int main(void){
	
	DDRC = 0xF0; // initializing PORTC for keypad scanning
    DDRB = 0xFF; // initializing PORTB for output
	PORTB = 0;
	
	char dig1, dig2;
	short int scan, repeat_scan;
		
    while (1){ // repeatedly scan for PASSWORD: "05"
   
		scan = scan_keypad();			 
		_delay_ms(20);					
		repeat_scan = scan_keypad(); // debouncing handling
		scan &= repeat_scan;
		dig1 = keypad_to_ascii(scan);  
		while (dig1 == '?'){ // while no key is pressed, repeat
			scan = scan_keypad();
			_delay_ms(20);				
			repeat_scan = scan_keypad();
			scan &= repeat_scan;
			dig1 = keypad_to_ascii(scan);
		}
		_delay_ms(20);					
		scan = scan_keypad();			
		_delay_ms(20);					
		repeat_scan = scan_keypad();	
		scan &= repeat_scan;
		dig2 = keypad_to_ascii(scan);   
		while ( dig2 == '?') {			
			scan = scan_keypad();
			_delay_ms(20);				
			repeat_scan = scan_keypad();
			scan &= repeat_scan;
			dig2 = keypad_to_ascii(scan);
		}
		if ((dig1 == '0') && (dig2 == '5')){ // if PASSWORD is correct, light all PORTB LEDs for 4 seconds
			PORTB = 0xFF;
			_delay_ms(4000);
			PORTB = 0x00;
		} 
		else{ 
			for (int i = 1; i <= 8; i++){ // else, blink all PORTB LEDs for 4 seconds, every 0.25 seconds
				PORTB = 0xFF;
				_delay_ms(250);
				PORTB = 0x00;
				_delay_ms(250);
			}	
		}
	}
}