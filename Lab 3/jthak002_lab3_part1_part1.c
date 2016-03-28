/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 3 Part 1 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, Init, Wait, A0_1, A0_0_B1_1, A0_0_B0_1} state;
unsigned char valB=0x01;
unsigned char valA=0x00;

void Tick()
{
	valA=(~PINA)&0x01;
	switch (state)//state transition
	{

		case Start:
		state=Init;
		break;

		case Init:
		state=Wait;
		break;

		case Wait:
		if(valA==0x01)
		state=A0_1;
		else if(valA==0x00)
		state=Wait;
		break;

		case A0_1:
		if(valA==0x01)
		{
			state=A0_1;
		}
		else if(valA==0x00)
		{
			if(valB==0x01)
			state=A0_0_B1_1;
			else if(valB==0x02)
			state=A0_0_B0_1;
		}
		break;

		case A0_0_B0_1:
		state=Wait;
		break;

		case A0_0_B1_1:
		state=Wait;
		break;

	}
	switch (state)//State Actions
	{
		case Start:
		break;

		case Init:
		valB=0x01;
		PORTB=0x01;
		break;

		case A0_0_B0_1:
		valB=0x01;
		PORTB=valB;
		break;

		case A0_0_B1_1:
		valB=0x02;
		PORTB=valB;
		break;
	}
}
int main(void)
{
  DDRA=0x00;PORTA=0xFF;
	DDRB=0xFF;PORTB=0x00;
	state=Start;
    while (1)
    {
		Tick();
    }
}
