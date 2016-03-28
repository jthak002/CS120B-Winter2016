/*
* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 4
* Exercise Description: [Count the number of 1s on ports A and B and output that number on port C.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
   DDRA=0x00;PORTA=0xFF;
   DDRB=0x00;PORTB=0xFF;
   DDRC=0xFF;PORTC=0x00;
   
        unsigned char ucNumA=0x00;//number of 1's in A
	    unsigned char ucValA=0x00;//number of 1's in B
		unsigned char ucNumB=0x00;
		unsigned char ucValB=0x00;
		unsigned char i=0x00;
    while (1) 
    {
		ucNumA=0;
		ucNumB=0;
		ucValA=PINA;
		ucValB=PINB;
		for(i=0;i<=7;i++)//Navigating through 8 bits
		{
			if((ucValA & 0x80)==0x80)//checking if last bit is 1.
			{
				ucNumA++;
			}
			
			ucValA=ucValA << 1;
		}
		for(i=0;i<=7;i++)
		{
			if((ucValB & 0x80)==0x80)
			{
				ucNumB++;	 
			}
			ucValB=ucValB << 1;	
		}
		PORTC=ucNumA+ucNumB;	//number of bits in A and B
    }
}

