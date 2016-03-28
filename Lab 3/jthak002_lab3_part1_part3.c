/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 3 Part 1 Exercise 3
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>
enum States {Start, Init, Wait, A2_Press, Wait2, A1_Press, unlock, A7_Press, lock} state;

unsigned char valA=0x00;
unsigned char valB=0x00;

void Tick()
{
	valA=(~PINA)&0x87;
	switch(state)//State Transition
	{
		case Start:
		state=Init;
		break;

		case Init:
		state=Wait;
		break;

		case Wait:
		if(valA==0x80){
			state=A7_Press;
		}
		else if(valA==0x04){
			state=A2_Press;
		}
		else{
			state=Wait;
		}
		break;

		case A2_Press:
		if(valA==0x04)
		{
			state=A2_Press;
		}
		else if(valA==0x00)
		{
			state=Wait2;
		}
		else
		{
			state=Wait;
		}
		break;

		case Wait2:
		if(valA==0x00)
		{
			state=Wait2;
		}
		else if(valA==0x02)
		{
			state=A1_Press;
		}
		else
		{
			state=Wait;
		}
		break;

		case A1_Press:
		if(valA==0x02)
		{
			state=A1_Press;
		}
		else if(valA==0x00)
		{
			state=unlock;
		}
		else
		{
			state=Wait;
		}
		break;

		case unlock:
		state=Wait;
		break;

		case A7_Press:
		if((valA==0x80))
		{
			state=A7_Press;
		}
		else if(valA==0x00)
		{
			state=lock;
		}
		else
		{
			state=Wait;
		}
		break;

		case lock:
		state=Wait;
		break;

		default:break;

	}
	switch(state)//State Action
	{
		case Start:
		PORTC=0x00;
		break;

		case Init:
		PORTC=0x01;
		break;

		case Wait:
		PORTC=0x02;
		break;

		case A2_Press:
		PORTC=0x03;
		break;

		case Wait2:
		PORTC=0x04;
		break;

		case A1_Press:
		PORTC=0x05;
		break;

		case unlock:
		valB=0x01;
		PORTB=valB;
		PORTC=0x06;
		break;

		case A7_Press:
		PORTC=0x07;
		break;

		case lock:
		valB=0x00;
		PORTB=valB;
		PORTC=0x08;
		break;

		default:break;
	}
}


int main(void)
{
	DDRA=0x00;PORTA=0xFF;
	DDRB=0xFF;PORTB=0x00;
	DDRC=0xFF;PORTC=0x00;
	state=Start;
    /* Replace with your application code */
    while (1)
    {
		Tick();
    }
}
