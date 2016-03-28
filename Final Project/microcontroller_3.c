/*
 * microcontroller_3.c
 *
 * Created: 3/1/2016 11:43:19 AM
 * Author : Jeet
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

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
//**********END_OF_TIMER_CODE************
unsigned char startvar=0x00;


//*********DISPLAY_CODE_AND_STATE_MACHINE****
volatile unsigned char a [16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
enum SM_States1 {Start, Display} SM_State1;
unsigned char i = 0;
void SM_display()
{
	switch (SM_State1)
	{
		case Start:
		SM_State1=Display;
		PORTA=0x00;
		PORTC=0X00;
		PORTD=0x00;
		break;

		case Display:
		SM_State1=Display;
		break;

		default:
		SM_State1=Display;
		break;
	}
	switch (SM_State1)
	{
		case Display:
		if(a[0] && i==0)
		{
			PORTA=0x03;
			PORTD=0xFC;
			if(PINB&0x02)
			PORTC=0xFC;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[1] && i==1)
		{
			PORTA=0x0C;
			if(PINB&0x02)
			PORTD=0xFC;
			else
			PORTD=0xFF;
			PORTC=0xFC;
			i++;
			break;
		}
		if(a[2] && i==2)
		{
			PORTA=0x30;
			PORTD=0xFC;
			if(PINB&0x02)
			PORTC=0xFC;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[3] && i==3)
		{
			PORTA=0xC0;
			if(PINB&0x02)
			PORTD=0xFC;
			else
			PORTD=0xFF;
			PORTC=0xFC;
			i++;
			break;
		}
		if(a[4] && i==4)
		{
			PORTA=0x03;
			PORTD=0xF3;
			if(PINB&0x02)
			PORTC=0xF3;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[5] && i==5)
		{
			PORTA=0x0C;
			if(PINB&0x02)
			PORTD=0xF3;
			else
			PORTD=0xFF;
			PORTC=0xF3;
			i++;
			break;
		}
		if(a[6] && i==6)
		{
			PORTA=0x30;
			PORTD=0xF3;
			if(PINB&0x02)
			PORTC=0xF3;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[7] && i==7)
		{
			PORTA=0xC0;
			if(PINB&0x02)
			PORTD=0xF3;
			else
			PORTD=0xFF;
			PORTC=0xF3;
			i++;
			break;
		}
		if(a[8] && i==8)
		{
			PORTA=0x03;
			PORTD=0xCF;
			if(PINB&0x02)
			PORTC=0xCF;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[9] && i==9)
		{
			PORTA=0x0C;
			if(PINB&0x02)
			PORTD=0xCF;
			else
			PORTD=0xFF;
			PORTC=0xCF;
			i++;
			break;
		}
		if(a[10] && i==10)
		{
			PORTA=0x30;
			PORTD=0xCF;
			if(PINB&0x02)
			PORTC=0xCF;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[11] && i==11)
		{
			PORTA=0xC0;
			if(PINB&0x02)
			PORTD=0xCF;
			else
			PORTD=0xFF;
			PORTC=0xCF;
			i++;
			break;
		}
		if(a[12] && i==12)
		{
			PORTA=0x03;
			PORTD=0x3F;
			if(PINB&0x02)
			PORTC=0x3F;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[13] && i==13)
		{
			PORTA=0x0C;
			if(PINB&0x02)
			PORTD=0x3F;
			else
			PORTD=0xFF;
			PORTC=0x3F;
			i++;
			break;
		}
		if(a[14] && i==14)
		{
			PORTA=0x30;
			PORTD=0x3F;
			if(PINB&0x02)
			PORTC=0x3F;
			else
			PORTC=0xFF;
			i++;
			break;
		}
		if(a[15] && i==15)
		{
			PORTA=0xC0;
			if(PINB&0x02)
			PORTD=0x3F;
			else
			PORTD=0xFF;
			PORTC=0x3F;
			i=0;
			break;
		}
		else
		{
			PORTA=0x00;
			if(i<15)
				i++;
			else
				i=0;
		}
		break;
		default:break;
	}

}
//***************END_OF_DISPLAY_CODE**************
enum SM_States2 {Start1, Shift_1} SM_State2;
void SM_shift()
{
	switch(SM_State2)
	{
		case Start1:
		SM_State2=Shift_1;
		break;

		case Shift_1:
		SM_State2=Shift_1;
		break;

		default:
		break;
	}
	switch(SM_State2)
	{
		case Shift_1:
		a[15]=a[11];
		a[14]=a[10];
		a[13]=a[9];
		a[12]=a[8];
		a[11]=a[7];
		a[10]=a[6];
		a[9]=a[5];
		a[8]=a[4];
		a[7]=a[3];
		a[6]=a[2];
		a[5]=a[1];
		a[4]=a[0];
		a[3]=0x00;
		a[2]=0x00;
		a[1]=0x00;
		a[0]=0x00;
		break;

		default:break;
	}

}

//**************BUTTON_PRESS*************
enum SM_States3{Start2, button_identify} SM_State3;
void SM_button_identify()
{
	switch(SM_State3)
	{
		case Start2:
		SM_State3=button_identify;
		break;

		case button_identify:
		SM_State3=button_identify;
		break;
	}
	switch(SM_State3)
	{
		case Start2:
		break;
		case button_identify:
		if(a[12])
			PORTB=(PORTB&0x0F)|0x10;
		else if(a[13])
			PORTB=(PORTB&0x0F)|0x20;
		else if(a[14])
			PORTB=(PORTB&0x0F)|0x40;
		else if(a[15])
			PORTB=(PORTB&0x0F)|0x80;
		else
			PORTB=(PORTB&0x0F);
		break;
	}
}

enum SM_gens {Start5, Wait, generate} SM_gen;
void SM_Generate_Random()
{
	switch (SM_gen)
	{
		case Start5:
		SM_gen=Wait;
		break;

		case Wait:
		if( (PINB&0x01) ==0x01)
			SM_gen=generate;
		else
			SM_gen=Wait;
		break;

		case generate:
		if( (PINB&0x01) ==0x01)
			SM_gen=generate;
		else
			SM_gen=Wait;
		break;

		default: break;
	}
	switch (SM_gen)
	{
		case generate:
		a[rand()%4]=0x01;
		break;
		default: break;
	}

}
int main(void)
{
    /* Replace with your application code */
	DDRA=0xFF;PORTA=0x00;
	DDRB=0xF0; PORTB=0x0F;
	DDRC=0xFF;PORTC=0x00;
	DDRD=0xFF;PORTD=0x00;
	//unsigned char i=0xFC;
	unsigned long elapsedtime_1=0x00;
	TimerOn();
	TimerSet(1);
	SM_State1=Start;
	SM_State2=Start1;
	SM_State3=Start2;
    while (1)
    {
		if(elapsedtime_1!=400)
			SM_display();
		if(elapsedtime_1==400)
		{
				SM_shift();
				SM_Generate_Random();
				SM_button_identify();
				elapsedtime_1=0;
		}
		while(!TimerFlag){}
		TimerFlag=0;
		elapsedtime_1++;
    }
}
