/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 3 Part 2 Exercise 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
enum States{Start, LED_ON} state;

volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;

unsigned char valB = 0x00;
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	avr_timer_cntcurr = avr_timer_M;
	SREG = 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	avr_timer_cntcurr--;
	if (avr_timer_cntcurr == 0){
		TimerISR();
		avr_timer_cntcurr = avr_timer_M;
		
	}
}

void TimerSet(unsigned long M){
	avr_timer_M = M;
	avr_timer_cntcurr = avr_timer_M;
	
}



void Tick(){
	switch (state){
		
		case Start:
		state=LED_ON;
		break;
			
		case LED_ON:
		state=LED_ON;
		break;
		
		}
	
	switch (state){
		
		case LED_ON:
		if(valB==0x04 || valB==0x00)
			valB=0x01;
		else
			valB=valB<<1;
		PORTB=valB;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	
	DDRB = 0xFF;PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	state = Start;

	while(1)
	{
		Tick();
		
		while(!TimerFlag);
		TimerFlag = 0;
	}
}