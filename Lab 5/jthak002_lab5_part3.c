/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 5 Part 3
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

//**********TIMER_CODE********************
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
//**********TIMER_CODE************

unsigned short time_elapsed=0x00;
unsigned char valB=0x00;
enum BL_States{Start, LED_OFF, LED_ON} Bl_state;
void Tick_BL()
{
	switch(Bl_state)//state transitions
	{
		case Start:
		Bl_state=LED_ON;
		break;
		
		case LED_ON:
		if(time_elapsed==1000)
		{
			Bl_state=LED_OFF;
		}
		break;
		
		case LED_OFF:
		if(time_elapsed==1000)
		{
			Bl_state=LED_ON;
		}
		break;
		
		default:
		break;
	}
	switch(Bl_state)
	{
		case LED_ON:
		valB=valB|0x08;
		break;
		
		case LED_OFF:
		valB=valB&0xF7;
		break;
		
		default:
		break;
	}
}
enum THREE_LED{Start3, LED_1, LED_2, LED_3}TLED_state;
void Tick_3L()
{
	switch(TLED_state) //state transitions
	{
		case Start3:
		TLED_state=LED_1;
		break;
		
		case LED_1:
		if(time_elapsed%300==0)
		{
			TLED_state=LED_2;
		}
		else
		{
			TLED_state=LED_1;
		}
		break;
		
		case LED_2:
		if(time_elapsed%300==0)
		{
			TLED_state=LED_3;
		}
		else
		{
			TLED_state=LED_2;
		}
		break;
		
		case LED_3:
		if(time_elapsed%300==0)
		{
			TLED_state=LED_1;
		}
		else
		{
			TLED_state=LED_3;
		}
		break;
		
		default:
		TLED_state=Start3;
		break;
	}
	
	switch(TLED_state)
	{
		case LED_1:
		valB=valB&0xF8;
		valB=valB|0x04;
		break;
		
		case LED_2:
		valB=valB&0xF8;
		valB=valB|0x02;
		break;
		
		case LED_3:
		valB=valB&0xF8;
		valB=valB|0x01;
		break;
		
		default:
		break;
	}
	
}

unsigned char freq =0x00;


unsigned char valA=0x00;
enum BZ_States{Start1, buzz_on, buzz_off} BZ_State;
void Tick_Bz()
{
	valA=(~PINA)&0x01;
	switch (BZ_State)
	{
		case Start1:
		BZ_State=buzz_on;
		break;
		
		case buzz_on:
		if(time_elapsed%2==0)
			BZ_State=buzz_off;
		else if(valA==0x01)
			BZ_State=buzz_on;
		break;
		
		case buzz_off:
		if(time_elapsed%2==0 && valA==0x01)
			BZ_State=buzz_on;
		else
			BZ_State=buzz_off;
		break;
		
		default:
		break;
	}//state transition for buzzer
	
	switch (BZ_State)
	{
		case buzz_on:
		valB=valB|0x10;
		break;
		
		case buzz_off:
		valB=valB&0xEF;
		break;
		
		default:
		break;
	}
}


void Update_Port_B()
{
	PORTB=valB;
}



int main(void)
{
	DDRA=0x00; PORTA=0xFF;
	DDRB=0x0FF;PORTB=0x00;
	Bl_state=Start;
	TLED_state=Start3;
	BZ_State=Start1;
	TimerSet(1);
	TimerOn();
	
	while (1)
	{
		valA=0x00;
		Tick_BL();
		Tick_3L();
		Tick_Bz();
		Update_Port_B();
		while(!TimerFlag){}
		TimerFlag=0;
		time_elapsed+=1;
		if(time_elapsed>1000)
		time_elapsed=1;
	}
}