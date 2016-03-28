/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 6 Part 1 part4
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}



int main(void)
{
	DDRB=0xFF; PORTB=0x00;
	DDRD=0xFF; PORTD=0x00;
	unsigned short Max=0x00;
	ADC_init();
	while (1)
    {

		unsigned short x = ADC; // Value of ADC register now stored in variable x.
		if(Max<x)
			Max=x;

		if(x==Max)
		PORTB=0x80;
		else if(x>(7*Max/8))
		PORTB=0x40;
		else if(x>(6*Max/8))
		PORTB=0x20;
		else if(x>(5*Max/8))
		PORTB=0x10;
		else if(x>(4*Max/8))
		PORTB=0x08;
		else if(x>(3*Max/8))
		PORTB=0x04;
		else if(x>(2*Max/8))
		PORTB=0x02;
		else if(x>(Max/8))
		PORTB=0x01;
		else if(x>=0 && x<(Max/8))
		PORTB=0x00;
    }
}
