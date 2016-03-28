/*
* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 5
* Exercise Description: [A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and
15 (full). A series of LEDs connected to PC5..PC0 should light to graphically
indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4,
PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights
PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which
should light if the level is 4 or less. (The example below shows the display for a
fuel level of 3). In this laboratory exercise, we will not actually use LEDS; just use
the simulator and set inputs to 0 and 1 as needed and observe the output.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRA=0x00;PORTA=0xFF;
	DDRC=0xFF;PORTC=0x00;
	
	unsigned char ucValA=0x00;
	unsigned char ucSetC=0x00;
    while (1) 
    {
		ucValA=PINA;
		ucSetC=0x00;
		if( ((ucValA&0xFF)==0x00) )
		{
			ucSetC=ucSetC|0x40;	
		}
		if( ((ucValA&0xFF)==0x01) || ((ucValA&0xFF)==0x02) )
		{
			ucSetC=ucSetC|0x60;	
		}
		if( ((ucValA&0xFF)==0x03) || ((ucValA&0xFF)==0x04) )
		{
			ucSetC=ucSetC|0x70;
		}
		if( ((ucValA&0xFF)==0x05) || ((ucValA&0xFF)==0x06) )
		{
			ucSetC=ucSetC|0x38;
		}
		if( ((ucValA&0xFF)==0x07) || ((ucValA&0xFF)==0x08) || ((ucValA&0xFF)==0x09) )
		{
			ucSetC=ucSetC|0x3C;
		}
		if( ((ucValA&0xFF)==0x0A) || ((ucValA&0xFF)==0x0B) || ((ucValA&0xFF)==0x0C) )
		{
			ucSetC=ucSetC|0x3E;
		}
		if( ((ucValA&0xFF)==0x0D) || ((ucValA&0xFF)==0x0E) || ((ucValA&0xFF)==0x0F) )
		{
			ucSetC=ucSetC|0x3F;
		} 
		PORTC=ucSetC;
		
    }
}

