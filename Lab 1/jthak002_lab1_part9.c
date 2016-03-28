/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 9
* Exercise Description: [A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511)
and connects to input PD7..PD0PB0 on the micro controller. If the weight is equal
to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. If the
weight is above 5 but below 70, the airbag should be disabled and an "Airbag
disabled" icon should light by setting PB2 to 1. (Neither B1 nor B2 should be set if
the weight is 5 or less, as there is no passenger).]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    DDRD=0x00;PORTD=0xFF;	//setting PORTD to act completely as the input
	DDRB=0xFE;PORTB=0x01;	//setting the first bit of PORTB to act as input and the rest as output
	unsigned short usWeight=0x00;	//to create the magnitude of the number using the bits in PORTD and PORTB
    while (1) 
    {
		usWeight=PIND;		//Setting PIND to the entire magnitude of PIND
		usWeight=usWeight<<1;	//MULTIPLYING THE MAGN. of PIND by 2 since this still has one more bit to add
		if((PINB&0x01)==0x01)	//checking thge last bit i.e PB0 if it is one adding 1 to the final some.
		{
			usWeight=usWeight+1;
		}
		
		if(usWeight>=70) //This checks the if the weight is greater than 70 
		{
			PORTB=PORTB&0x01;	//if TRUE, then it resets the lights back to zero and 
			PORTB=PORTB|0x02;	//sets PB1 to 1 i.e. AIRBAGS enabled
		}
		else if(usWeight<70 && usWeight>5) //CHECKS if weight is between 5 pounds and 70,
		{
			PORTB=PORTB&0x01;	//if TRUE, it rests all lights back to zero and 
			PORTB=PORTB|0x04;	//SETS PB2 to 1 i.e Airbag disabled
		}
		else
		{
			PORTB=PORTB&0x01;	//THIS resets all lights to zero i.e. no one in seat
		}
    }
}

