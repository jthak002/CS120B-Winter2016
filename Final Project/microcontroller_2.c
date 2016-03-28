/*
 * microcontroller_1.c
 *
 * Created: 3/7/2016 1:14:14 PM
 * Author : Jeet
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "C:\includes\io.c"
#include "C:\includes\keypad.h"

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
unsigned char multiplier =0x01;
unsigned char startvar=0x00;
unsigned long score=0x00;
unsigned char key_input=0x00;
unsigned char te=0x00;
unsigned char i=0;
signed long timerleft=0x00;
signed long timerleftarray[5]={30000,22000,25000,22000,18000};
enum SM_states1 {Start, Startup, Wait1, select_song, Wait12, Play_Screen, Final_Screen, Wait13} SM_state1;
char* welcome_string = "Welcome to Guitar Hero!";
char* k ="Welcome to Guit";
char* sel_string1="Select a Song";
char* sel_string2="1 2 3 4 5";
char* instruction = "Press * to play";
char* play = "Score:";

void shift_1bit()
{
	for(unsigned char j=0;j<16;j++)
	{
		if(i+j<=22)
		{
			k[j]=welcome_string[i+j]; continue;
		}
		k[j]=' ';
	}
}
void SM_Tick1()
{

	switch(SM_state1)
	{
		case Start:
		i=0;
		startvar=0;
		SM_state1=Startup;
		break;

		case Startup:
		if(key_input=='*')
			SM_state1=Wait1;
		else
			SM_state1=Startup;
		break;

		case Wait1:
		if(key_input=='*')
			SM_state1=Wait1;
		else
		{
			SM_state1=select_song;
			LCD_ClearScreen();
		}
		break;

		case select_song:
		if( key_input=='1' || key_input=='2' || key_input=='3' || key_input=='4' || key_input=='5')
		{
			SM_state1=Wait12;
			PORTD=PORTD&0x8F;
			PORTD=PORTD|(0x70&(key_input<<4));
			timerleft=timerleftarray[key_input -1 - 48];
			startvar=0x01;
		}
		else if(key_input=='\0')
			SM_state1=select_song;
		break;

		case Wait12:
		if(key_input!='\0')
		{
			SM_state1=Wait12;
		}
		else
		{
			SM_state1=Play_Screen;
			LCD_ClearScreen();
		}
		break;

		case Play_Screen:
		if(timerleft>0)
		{
			SM_state1=Play_Screen;
			timerleft-=100;
		}
		else
		{
			SM_state1=Final_Screen;
			LCD_ClearScreen();
			startvar=0x00;
		}
		break;

		case Final_Screen:
		if(key_input=='*')
			SM_state1=Wait13;
		else
			SM_state1=Final_Screen;
		break;

		case Wait13:
		SM_state1=Wait1;
		break;

		default:
		break;
	}

	switch(SM_state1)
	{
		case Start:break;
		case Startup:
		for(unsigned char j=1; j<=16; j++)
		{
				LCD_Cursor(j);
				LCD_WriteData(k[j-1]);
				continue;
		}
		if(te>=5)
		{
			shift_1bit();
			te=0;
			if(i>=22)
				i=0;
			else
				i++;
		}
		else
			te++;
		for(unsigned char j=0; j<16;j++)
		{
			LCD_Cursor(17+j);
			if(j<15)
				LCD_WriteData(instruction[j]);
		}
		break;

		case select_song:
		for(unsigned  char j=0;j<=12;j++)
		{
			LCD_Cursor(j+1);
			LCD_WriteData(sel_string1[j]);
		}
		for(unsigned  char j=0;j<=8;j++)
		{
			LCD_Cursor(17+j);
			LCD_WriteData(sel_string2[j]);
		}
		LCD_Cursor(32);
		break;


		case Play_Screen:
		for(unsigned char j=0;j<6;j++)
		{
			LCD_Cursor(j+1);
			LCD_WriteData(play[j]);
		}
		LCD_Cursor(7);
		LCD_WriteData('0' + ((score%10000)/1000) );
		LCD_Cursor(8);
		LCD_WriteData('0' + ((score%1000)/100) );
		LCD_Cursor(9);
		LCD_WriteData('0' + ((score%100)/10 ) );
		LCD_Cursor(10);
		LCD_WriteData('0' + (score%10));
		LCD_Cursor(11);
		break;

		case Final_Screen:
		for(unsigned char j=0;j<6;j++)
		{
			LCD_Cursor(j+1);
			LCD_WriteData(play[j]);
		}
		LCD_Cursor(7);
		LCD_WriteData('0' + ((score%10000)/1000) );
		LCD_Cursor(8);
		LCD_WriteData('0' + ((score%1000)/100) );
		LCD_Cursor(9);
		LCD_WriteData('0' + ((score%100)/10 ) );
		LCD_Cursor(10);
		LCD_WriteData('0' + (score%10));
		LCD_Cursor(17);
		for(unsigned char j=0; j<16;j++)
		{
			LCD_Cursor(17+j);
			if(j<15)
			LCD_WriteData(instruction[j]);
		}
		break;

		case Wait13:
		startvar=0;
		i=0;
		score=0;
		multiplier=0x01;
		PORTD=PORTD&0x8F;
		timerleft=0;
		te=0;

		break;

	}
}
enum SM_states2 {Start2, take_input, check_lift} SM_state2;
void SM_Tick2()
{
	switch(SM_state2)
	{
		case Start2:
		SM_state2=take_input;
		break;

		case take_input:
		SM_state2=take_input;
		break;


		default:break;
	}
	switch(SM_state2)
	{
		case take_input:
		if(GetKeypadKey()=='\0')
			key_input='\0';
		if(key_input!=0xFE)
		key_input=GetKeypadKey();
		break;

		default: break;
	}
}
unsigned char button =0x00;

enum SM_states3 {Start3, Wait2, Gameplay } SM_state3;
void SM_Tick3()
{
	switch(SM_state3)
	{
		case Start3:
		SM_state3=Wait2;
		break;

		case Wait2:
		if(startvar==0x01)
		{
			SM_state3=Gameplay;
			PORTA=PORTA | 0x04;
		}
		else
		SM_state3=Wait2;
		break;

		case Gameplay:
		if(startvar)
		SM_state3=Gameplay;
		else
		{
			SM_state3=Wait2;
			PORTA=PORTA&0xFB;
		}

		break;
	}
	switch(SM_state3)
	{
		case Gameplay:
			;
			unsigned char button_2 = 0x00;
			button_2=PINA;
			button_2=button_2>>4;
			if(key_input == 'D')
				button = 0x01;
			else if(key_input == 'C')
				button = 0x02;
			else if(key_input == 'B')
				button = 0x04;
			else if(key_input == 'A')
				button = 0x08;
			else
				button = 0xFF;
			if(button==button_2 && (PIND&0x80))
			{

				key_input=0xFE;
				score+=(1*multiplier);
				if(multiplier<8)
				multiplier++;
			}
			else if(key_input!='\0')
			{
				key_input =0xFE;
				multiplier=0x01;
			}
			break;
		default: break;
	}
}

enum SM_states4 {Start4, report_multiplier} SM_state4;
void SM_Tick4()
{
	switch(SM_state4)
	{
		case Start4:
		SM_state4=report_multiplier;
		break;

		case report_multiplier:
		SM_state4=report_multiplier;
		break;
	}
	switch(SM_state4)
	{
		case report_multiplier:
		if(multiplier==0x08)
			PORTA = PORTA | 0x08;
		else
			PORTA = PORTA & 0xF7;
		break;
	}
}
enum SM_states5 {Start5, Send_SPI} SM_state5;
void SM_Tick5()
{
	switch(SM_state5)
	{
		case Start5:
		SM_state5=Send_SPI;
		break;

		case Send_SPI:
		SM_state5=Send_SPI;
		unsigned char valD=PORTD&0xF0;
		PORTD=valD|(multiplier&0x0F);
		break;

		default:break;
	}

}
int main(void)
{
	DDRA=0x0F; PORTA=0xF0;
	DDRB=0xFF; PORTB=0x00;
	DDRC = 0xF0; PORTC = 0x0F; // LCD data lines
	DDRD = 0x7F; PORTD = 0x80; // LCD control lines
	unsigned long timeElapsed1=0x00;
	unsigned long timeElapsed2=0x00;
	unsigned long timeElapsed3=0x00;
	TimerOn();
	TimerSet(1);
	SM_state1=Start;
	SM_state2=Start2;
	SM_state3=Start3;
	SM_state4=Start4;
	SM_state5=Start5;
	LCD_init();
	while (1)
    {
		if(timeElapsed1>100)
		{
			SM_Tick1();
			timeElapsed1=0;
		}
		if(timeElapsed2>100)
		{
			SM_Tick2();
			timeElapsed2=0;
		}
		if(timeElapsed3>100)
		{
			SM_Tick3();
			timeElapsed3=0;
		}
		SM_Tick4();
		SM_Tick5();
		while(!TimerFlag){}
		TimerFlag=0;
		timeElapsed1++;
		timeElapsed2++;
		timeElapsed3++;
    }
}
