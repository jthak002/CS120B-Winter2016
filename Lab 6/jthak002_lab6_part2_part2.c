/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 6 Part 2 part 2
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


#include <avr/io.h>

/*
 * Timer implementation for avr atmega1284
 */

#include <avr/io.h>
#include <avr/interrupt.h>


volatile unsigned char TimerFlag=0x00;
// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
void TimerOn() {
    // AVR timer/counter controller register TCCR1
    // bit3 = 0: CTC mode (clear timer on compare)
    // bit2bit1bit0=011: pre-scaler /64
    // 00001011: 0x0B
    // SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
    // Thus, TCNT1 register will count at 125,000 ticks/s
    TCCR1B = 0x0B;

    // AVR output compare register OCR1A.
    // Timer interrupt will be generated when TCNT1==OCR1A
    // We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
    // So when TCNT1 register equals 125,
    // 1 ms has passed. Thus, we compare to 125.
    OCR1A = 125;

    // AVR timer interrupt mask register
    // bit1: OCIE1A -- enables compare match interrupt
    TIMSK1 = 0x02;

    //Initialize avr counter
    TCNT1=0;

    // TimerISR will be called every _avr_timer_cntcurr milliseconds
    _avr_timer_cntcurr = _avr_timer_M;

    //Enable global interrupts: 0x80: 1000000
    SREG |= 0x80;
}

void TimerOff() {
    // bit3bit1bit0=000: timer off
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
    // CPU automatically calls when TCNT1 == OCR1
    // (every 1 ms per TimerOn settings)
    // Count down to 0 rather than up to TOP (results in a more efficient comparison)
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        // Call the ISR that the user uses
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}




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
unsigned char i=0x00;
double last[]={261.3, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
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
		else if(valA==0x00)
			state=A0_release;
		break;

		case A0_release:
		state=Wait;
		break;

		case A1_press:
		if(valA==0x02)
		state=A1_press;
		else if(valA==0x00)
		state=A1_release;
		break;

		case A1_release:
		state=Wait;
		break;

		case A2_press:
		if(valA==0x04)
		state=A2_press;
		else if(valA==0x00)
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
		//set_PWM(0);
		break;

		case A0_release:
		if(!flag)
		{
			set_PWM(last[i]);
			flag=0x01;
		}
		else
		{
			set_PWM(0);
			flag=0x00;
		}
		break;

		case A1_release:
		if(i<7 && flag)
			i++;
		if(flag)
		set_PWM(last[i]);
		break;

		case A2_release:
		if(i>0 && flag)
			i--;
		if(flag)
		set_PWM(last[i]);
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
