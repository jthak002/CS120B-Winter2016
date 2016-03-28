/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 6 Part 2 part 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
void set_PWM(double frequency) {


	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter

		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;

		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;

		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {

	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
unsigned char flag=0x00;
unsigned char valA=0x00;
enum States {Start, Wait , A0_press, A0_release, A1_press, A1_release, A2_press, A2_release } state;
void Tick()
{
	valA=(~PINA)&0x07;
	switch(state)
	{
		case Start:
		state=Wait;
		break;

		case Wait:
		if(valA==0x01)
		state=A0_press;
		else if(valA==0x02)
		state=A1_press;
		else if(valA==0x04)
		state=A2_press;
		else state=Wait;
		break;

		case A0_press:
		if(valA==0x01)
		state=A0_press;
		else
		state=A0_release;
		break;

		case A0_release:
		state=Wait;
		break;

		case A1_press:
		if(valA==0x02)
		state=A1_press;
		else
		state=A1_release;
		break;

		case A1_release:
		state=Wait;
		break;

		case A2_press:
		if(valA==0x04)
		state=A2_press;
		else
		state=A2_release;
		break;

		case A2_release:
		state=Wait;
		break;

		default:break;

	}
	switch(state)
	{
		case Wait:
		set_PWM(0);
		break;

		case A0_press:
		set_PWM(261.63);
		break;

		case A0_release:
		set_PWM(0);
		break;

		case A1_press:
		set_PWM(293.66);
		break;

		case A1_release:
		set_PWM(0);
		break;

		case A2_press:
		set_PWM(329.63);
		break;

		case A2_release:
		set_PWM(0);
		break;

		default:break;
	}
}
int main(void)
{
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	state=Start;
	PWM_on();
	while (1)
	{
		Tick();
	}
}
