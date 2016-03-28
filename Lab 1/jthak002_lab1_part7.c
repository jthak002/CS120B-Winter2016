/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 7
* Exercise Description: [An amusement park kid ride cart has three seats, with 8-bit weight sensors
connected to ports A, B, and C (measuring from 0-255 kilograms). Set PD0 to 1 if
the cart's total passenger weight exceeds the maximum of 140 kg. Also, the cart
must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80
kg. Can you also devise a way to inform the ride operator of the approximate
weight using the remaining bits on D? (Interesting note: Disneyland recently redid
their "It's a Small World" ride because the average passenger weight has increased
over the years, causing more boats to get stuck on the bottom).]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>


int main(void)
{
    DDRA=0x00;PORTA=0xFF;
	DDRB=0x00;PORTB=0xFF;
	DDRC=0x00;PORTC=0xFF;
	DDRD=0xFF;PORTD=0x00;
	unsigned char ucValA=0x00;
	unsigned char ucValB=0x00;
	unsigned char ucValC=0x00;
	unsigned char uctotWeight=0x00;
	while (1) 
    {
		ucValA=PINA;
		ucValB=PINB;
		ucValC=PINC;
		uctotWeight=ucValA+ucValB+ucValC;
		;
		if(((ucValA+ucValB+ucValC)>=140) || (ucValA-ucValC>80) || (ucValC-ucValA)>80)
		{
			PORTD=(PORTD|0x01);
		}
		else //if the cart is balanced AND the total weight is less than 140 kg, the display will show the weight
		{
			uctotWeight=uctotWeight>>1; //of the car divided by 2
			uctotWeight=uctotWeight<<1; //the bits are shifted to the right so the weight can be displayed on the remaining 7 bits
			PORTD=PORTD|uctotWeight;
		}
		PORTD=0x00; //reset bits on PORTD
	}
}

