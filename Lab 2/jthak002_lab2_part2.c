/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 2 Exercise 2
* Exercise Description: [(From an earlier lab) Buttons are connected to PA0 and PA1. The output for
PORTB is initially 0. Pressing PA0 increments PORTB (stopping at 9). Pressing
PA1 decrements PORTB (stopping at 0). If both buttons are depressed (even if
not initially simultaneously), PORTB resets to 0. If a reset occurs, both buttons
should be fully released before additional increments or decrements are allowed
to happen. Use LEDs (and resistors) on PORTB. Use a state machine (not
synchronous) captured in C.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
enum States {Start, Init, Wait, A0_1, A0_0, A1_1, A1_0, A01_0, A01_1} present_state;

unsigned char valB=0x00;
unsigned char valA=0x00;

 

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

void Tick()
{
	valA=(~PINA)&0x03;
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
		else if (valA==0x02)
		{
			present_state=A1_1;
		}
		else if (valA==0x03)
		{
			present_state=A01_1;
		}
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
		else if(valA==0x03)
		{
			present_state=A01_1;
		}
		break;
		
		case A0_0:
		present_state=Wait;
		break;
		
		case A1_1:
		if(valA==0x01)
		{
			present_state=A1_1;
		}
		else if(valA==0x00)
		{
			present_state=A1_0;
		}
		else if(valA==0x03)
		{
			present_state=A01_1;
		}
		break;
		
		case A1_0:
		present_state=Wait;
		break;
		
		case A01_1:
		if(valA==0x03)
		{
			present_state=A01_1;
		}
		else if (valA==0x00)
		{
			present_state=A01_0;
		}
		break;
		
		case A01_0:
		present_state=Init;
		break;
		
		default:
		break;
	}//present_state TRANSITIONS
	
	
	switch(present_state)
	{
		case Init:
		valB=0x00;
		PORTB=valB;
		break;
		
		case A0_0:
		if(valB<9)
		valB=valB+1;
		PORTB=valB;
		break;
		
		case A1_0:
		if(valB>0)
		valB=valB-1;
		PORTB=valB;
		break;
		
		case A01_0:
		valB=0x00;
		break;
		
		default:
		break;
	}//present_state actionss
}