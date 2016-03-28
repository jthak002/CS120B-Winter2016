/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 4 Part 1
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"
enum States{Start, Init, Wait, A0_pressed, A0_released, A1_pressed, A1_released, A01_pressed, A01_released} state;
	
volatile unsigned char TimerFlag = 0;
unsigned long avr_timer_M = 1;
unsigned long avr_timer_cntcurr = 0;
void TimerISR(){TimerFlag=1;}
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


unsigned char temp=0x00;
unsigned char valA=0x00;

void Tick()
{
	valA=(~PINA)&0x03;
	switch(state){
		case Start:
		state=Init;
		break;
		
		case Init:
		state=Wait;
		break;
		
		case Wait:
		if(valA==0x01)
		{
			state=A0_pressed;
		}
		else if (valA==0x02)
		{
			state=A1_pressed;
		}
		else if (valA==0x03)
		{
			state=A01_pressed;
		}
		break;
		
		case A0_pressed:
		if(valA==0x01)
		{
			state=A0_pressed;
		}
		else if(valA==0x00)
		{
			state=A0_released;
		}
		else if(valA==0x03)
		{
			state=A01_pressed;
		}
		break;
		
		case A0_released:
		state=Wait;
		break;
		
		case A1_pressed:
		if(valA==0x01)
		{
			state=A1_pressed;
		}
		else if(valA==0x00)
		{
			state=A1_released;
		}
		else if(valA==0x03)
		{
			state=A01_pressed;
		}
		break;
		
		case A1_released:
		state=Wait;
		break;
		
		case A01_pressed:
		if(valA==0x03)
		{
			state=A01_pressed;
		}
		else if (valA==0x00)
		{
			state=A01_released;
		}
		break;
		
		case A01_released:
		state=Init;
		break;
		
		default:
		break;
	}//state TRANSITIONS
	
	
	switch(state)
	{
		case Init:
		temp=0x00;
		LCD_init(); LCD_WriteData('0'+temp);
		break;
		
		case A0_pressed:
		if(temp<9)
		temp=temp+1;
		LCD_init(); LCD_WriteData('0'+temp);
		while(!TimerFlag){}
		break;
		
		case A1_pressed:
		if(temp>0)
		temp=temp-1;
		LCD_init(); LCD_WriteData('0'+temp);
		while(!TimerFlag){}
		break;
		
		case A01_pressed:
		temp=0x00;
		LCD_init(); LCD_WriteData('0'+temp);
		while(!TimerFlag){}
		break;
		
		default:
		break;
	}//state actionss
}
int main(void)
{
	DDRA=0x00; PORTA=0xFF;
	DDRC=0xFF; PORTC=0xFF;
	DDRD=0xFF; PORTD=0x00;
	LCD_init();
	
    state=Start;
	TimerSet(1000);
	TimerOn();
	
	while (1) 
    {
		Tick();
		TimerFlag=0;
    }
}

