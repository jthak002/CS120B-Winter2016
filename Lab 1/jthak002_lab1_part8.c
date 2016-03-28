/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 8
* Exercise Description: [Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. That
is to say, take the upper nibble of PINA and map it to the lower nibble of PORTB,
likewise take the lower nibble of PINA and map it to the upper nibble of PORTC
(PA7 -> PB3, PA6 -> PB2, � PA1 -> PC5, PA0 -> PC4).]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRA=0x00;PORTA=0xFF;
	DDRB=0xFF;PORTB=0x00;
	DDRC=0xFF;PORTC=0x00;

	unsigned char temp=0x00;
    while (1)
    {
		temp=PINA;
		temp=temp>>4;
		PORTB=temp;
		temp=PINA;
		temp=temp<<4;
		PORTC=temp;
    }
}
