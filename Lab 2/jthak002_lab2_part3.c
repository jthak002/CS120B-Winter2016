/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
 * Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
 * Lab Section: 021
 * Assignment: Lab 2 Exercise 3
 * Exercise Description: [ Create your own festive lights display with 6 LEDs connected to port PB5..PB0,
 lighting in some attractive sequence. Pressing the button on PA0 changes the
 lights to the next configuration in the sequence. Use a state machine (not
 synchronous) captured in C]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
enum States {Start, Init, Wait, A0_1, A0_0} present_state;

unsigned char valB=0x00;
unsigned char valA=0x00;
unsigned char ctr=0x00;

void Tick()
{
	valA=(~PINA)&0x01;
	switch(present_state){
		case Start:
		present_state=Init;
		break;
		
		case Init:
		present_state=Wait;
		break;
		
		case Wait:
		if(valA==0x01)
		{
			present_state=A0_1;
		}
		else
		present_state=Wait;
		break;
		
		case A0_1:
		if(valA==0x01)
		{
			present_state=A0_1;
		}
		else if(valA==0x00)
		{
			present_state=A0_0;
		}
		break;
		
		case A0_0:
		present_state=Wait;
		break;
		
		default:
		break;
	}//present_state TRANSITIONS
	
	
	switch(present_state)
	{
		case Init:
		valB=0x01;
		PORTB=valB;
		break;
		
		case A0_0:
		if(ctr==6)
		{
			ctr=0;
			valB=0x01;
		}
		else
		{
			valB=valB<<1;
			ctr++;	
		}
		PORTB=valB;
		break;
		
		default:
		break;
	}//present_state actions
}

int main(void)
{
	DDRA=0x00;PINA=0xFF;
	DDRB=0xFF;PINB=0x00;
	present_state=Start;
	while (1)
	{
		Tick();
	}
}

