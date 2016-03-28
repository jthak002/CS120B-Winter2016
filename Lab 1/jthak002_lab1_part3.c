/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 3
* Exercise Description: [Extend the program in Exercise 2 to still write the available spaces number, but
only to PC3…PC0, and to set PC7 to 1 if the lot is full.]
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
	unsigned char ucOccupied=0x00; //variable to count the number of free spaces each iteration
    while (1) 
    {
		//Program is same as part 2 with an added if statement at the end
		ucOccupied=0x00;


		if(PINA&0x01) //this checks for a free space and adds 1 to the count
			ucOccupied++;
		if(PINA&0x02) // this checks for a free space in 2 and adds 1 if free
			ucOccupied++;
		if(PINA&0x04)
			ucOccupied++;
		if(PINA&0x08)
			ucOccupied++;
		PORTC=4-ucOccupied;
		if(ucOccupied==4)
		{
			PORTC=PORTC|0x80;	//SETS PC7 TO 1, IF  zero spaces are free.
		}	
				
	}
}

