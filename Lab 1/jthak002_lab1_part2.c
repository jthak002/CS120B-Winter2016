/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 2
* Exercise Description: [Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is
parked in the space, of a four-space parking lot. Write a program that outputs in
binary on port C the number of available spaces (Hint: declare a variable "unsigned
char cntavail"; you can assign a number to a port as follows: PORTC = cntavail;).]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
	DDRA=0x00;
	PORTA=0xFF;
	DDRC=0xFF;
	PORTC=0x00;
	unsigned char cntavail=0x00; //variable to count the number of free spaces each iteration
    while (1) 
    {
		cntavail=0x00;

		if(PINA&0x01) //this checks for a free space and adds 1 to the count
			cntavail++;
		if(PINA&0x02) // this checks for a free space in 2 and adds 1 if free
			cntavail++;
		if(PINA&0x04)
			cntavail++;
		if(PINA&0x08)
			cntavail++;
		PORTC=4-cntavail;
	}
}

