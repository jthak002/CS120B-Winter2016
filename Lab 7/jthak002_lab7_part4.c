/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 7 Part 4
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "C:\includes\io.c"

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

//****************KEYPAD CODE***************************
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
#define KEYPADPORT PORTC
#define KEYPADPIN  PINC
#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define COL1 4
#define COL2 5
#define COL3 6
#define COL4 7

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets input from a keypad via time-multiplexing
//Parameter: None
//Returns: A keypad button press else '\0'
unsigned char GetKeypadKey() {

	// Check keys in col 1
	KEYPADPORT = SetBit(0xFF,COL1,0); // Set Px4 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(~KEYPADPIN,ROW1) ) { return '1'; }
	if ( GetBit(~KEYPADPIN,ROW2) ) { return '4'; }
	if ( GetBit(~KEYPADPIN,ROW3) ) { return '7'; }
	if ( GetBit(~KEYPADPIN,ROW4) ) { return '*'; }

	// Check keys in col 2
	KEYPADPORT = SetBit(0xFF,COL2,0); // Set Px5 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(~KEYPADPIN,ROW1) ) { return '2'; }
	if ( GetBit(~KEYPADPIN,ROW2) ) { return '5'; }
	if ( GetBit(~KEYPADPIN,ROW3) ) { return '8'; }
	if ( GetBit(~KEYPADPIN,ROW4) ) { return '0'; }

	// Check keys in col 3
	KEYPADPORT = SetBit(0xFF,COL3,0); // Set Px6 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(~KEYPADPIN,ROW1) ) { return '3'; }
	if ( GetBit(~KEYPADPIN,ROW2) ) { return '6'; }
	if ( GetBit(~KEYPADPIN,ROW3) ) { return '9'; }
	if ( GetBit(~KEYPADPIN,ROW4) ) { return '#'; }

	// Check keys in col 4
	KEYPADPORT = SetBit(0xFF,COL4,0); // Set Px7 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if (GetBit(~KEYPADPIN,ROW1) ) { return 'A'; }
	if (GetBit(~KEYPADPIN,ROW2) ) { return 'B'; }
	if (GetBit(~KEYPADPIN,ROW3) ) { return 'C'; }
	if (GetBit(~KEYPADPIN,ROW4) ) { return 'D'; }

	return '\0';
}
//*****************END_KEY_PAD_CODE***************************************
typedef struct Tasks {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;


unsigned char setKey=0x00;
enum SM1_states{go} SM1_state;
unsigned char cursor_pos=0x01;

int SMTick1(int state)
{
	switch(state)
	{
		default:
		setKey = GetKeypadKey();
		if(setKey != '\0')
		{
			LCD_WriteData(setKey);
			if(cursor_pos>15)
			{
				cursor_pos=1;
				LCD_Cursor(cursor_pos);
			}
			else
			{
				cursor_pos++;
				LCD_Cursor(cursor_pos);
			}
		}

		break;
	}
	return 0;
}
//unsigned char cursor_pos=0x01;
int main()
{
	DDRA=0xFF; PORTA=0x00;
	DDRB=0xFF; PORTB=0x00;
	DDRD=0xFF; PORTD=0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1;

	task *tasks[] = {&task1};

	unsigned long int task_period = 300;
	unsigned long int task_tick = 30;
	//const unsigned short numTasks = sizeof(task)/sizeof(task*);

	task1.state = -1;
	task1.period = task_period;
	task1.elapsedTime = task_tick;
	task1.TickFct = &SMTick1;

	TimerOn();
	TimerSet(task_period);
	LCD_init();
	const char* a = "Congratulations!";
	LCD_DisplayString(1,a);
	LCD_Cursor(cursor_pos);
	//LCD_WriteData(95);

	while(1)
	{
		tasks[0]->TickFct(0);
		while(!TimerFlag){}
		TimerFlag=0;
	}
}
