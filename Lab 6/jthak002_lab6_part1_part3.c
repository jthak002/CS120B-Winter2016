/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 6 Part 1 part 3
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
		if(x>Max)
			Max=x;
		if(x>(Max/2))
		{
			PORTB=0xFF;
			PORTD=0x03;
		}
		else
		{
			PORTB=0x00;
			PORTD=0x00;
		}
    }
}
