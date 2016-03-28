/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 2 Exercise 1
* Exercise Description: [1. (From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value
between 0 (empty) and 15 (full). A series of LEDs connected to PB5..PB0 should
light to graphically indicate the fuel level. If the fuel level is 1 or 2, PB5 lights. If
the level is 3 or 4, PB5 and PB4 light. 5-6 lights PB5..PB3. 7-9 lights PB5..PB2.
10-12 lights PB5..PB1. 13-15 lights PB5..PB0. Also, PB6 connects to a "Low
fuel" icon, which should light if the level is 4 or less. Use buttons on PA3..PA0
and mimic the fuel-level sensor with presses.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRA=0x00;PINA=0xFF;
	DDRB=0xFF;PINB=0x00;
	
	unsigned char ucValA=0x00;
	unsigned char ucSetB=0x00;
    while (1) 
    {
		ucValA=~PINA;
		ucSetB=0x00;
		if( ((ucValA&0x0F)==0x00) )
		{
			ucSetB=ucSetB|0x40;
		}
		if( ((ucValA&0x0F)==0x01) || ((ucValA&0x0F)==0x02) )
		{
			ucSetB=ucSetB|0x60;
		}
		if( ((ucValA&0x0F)==0x03) || ((ucValA&0x0F)==0x04) )
		{
			ucSetB=ucSetB|0x70;
		}
		if( ((ucValA&0x0F)==0x05) || ((ucValA&0x0F)==0x06) )
		{
			ucSetB=ucSetB|0x38;
		}
		if( ((ucValA&0x0F)==0x07) || ((ucValA&0x0F)==0x08) || ((ucValA&0x0F)==0x09) )
		{
			ucSetB=ucSetB|0x3C;
		}
		if( ((ucValA&0x0F)==0x0A) || ((ucValA&0x0F)==0x0B) || ((ucValA&0x0F)==0x0C) )
		{
			ucSetB=ucSetB|0x3E;
		}
		if( ((ucValA&0x0F)==0x0D) || ((ucValA&0x0F)==0x0E) || ((ucValA&0x0F)==0x0F) )
		{
			ucSetB=ucSetB|0x3F;
		}
		PORTB=ucSetB;	
		
    }
}

