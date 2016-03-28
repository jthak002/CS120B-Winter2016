/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 1
* Exercise Description: [Garage open at night-- A garage door sensor connects to PA0 (1 means door open),
and a light sensor connects to PA1 (1 means light is sensed). Write a program that
illuminates an LED connected to PB0 (1 means illuminate) if the garage door is
open at night. In this laboratory exercise, we will not actually use sensors or LEDS;
just use the simulator and set inputs to 0 and 1 as needed and observe the output.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRA=0x00; PORTA=0xFF;//using PORTA as INPUT
	DDRB=0xFF;PORTB=0x00;//using PORTB as OUTPUT
    while (1) 
    {
		if( PINA == 0x01 ) //COMPARING PINA TO THE BITMASK OF 1
		{
			PORTB=0x01; //setting first pin to 1 if bitmask matches
		}
		else
			PORTB=0x00; //reverting to 0 if conditions if conditions dont change
    }
}

