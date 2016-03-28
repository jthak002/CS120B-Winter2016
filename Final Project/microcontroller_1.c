#include <avr/io.h>
#include <avr/interrupt.h>
#include "C:\includes\timer.h"
#include "C:\includes\usart.h"

#include <avr/io.h>

//*******************ADC******************
void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}
//******************************************
//GLOBAL VARIABLES
unsigned char SONG1 = 0;
unsigned char SONG2 = 0;
unsigned char SONG3 = 0;
unsigned char SONG4 = 0;
unsigned char SONG5 = 0;
unsigned char SONG7 = 0;


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


unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

/*State machine enums*/

enum SM_MELODY {
	WAIT,
	PLAY_NOTE,
	WAIT_NOTE,
	WAIT_PRESS,
	play_sw,

	wait_note_sw,
	play_journey,
	wait_journey,
	play_roses,
	wait_roses,
	play_iron,
	wait_iron,
	play_train,
	wait_train

} melody_state;

#define C4 261.63
#define D4 293.66
#define E4 329.63
#define F4 349.23
#define G4 392.00
#define A4 440.00
#define B4 493.88
#define C5 523.25
#define E7 2637.02
#define C7 2093
#define G7 3135.96
#define G6 1567.98
#define E6 1318.51
#define A6 1760.00
#define B6 1975.53
#define AS6 1864.66
#define A7 3520
#define F7 2793.83
#define C7 2093
#define D7 2349.32
#define F6 1396.91
#define DS7 2489.02
#define AS7 3729.31
#define F5 698.46
#define AS5 932.33
#define DS6 1244.51
#define D6 1174.66
#define C6 1046.5
#define GS7 3322.44
#define FS7 2959.96
#define B7 3951.07
#define CS6 1108.73
#define CS7 2217.46
#define GS6 1661.22
#define FS6 1479.98
#define AS3 233.08
#define AS4 466.16
#define DS3 155.56
#define DS4 311.13
#define F3 174.61
#define D3 146.83
#define A3 220
#define CS4 277.18
#define GS3 207.65
#define FS4 369.99
#define GS4 415.3
#define B3 246.94
#define G3 196
#define E3 164.81
#define FS4	369.99
#define B5 987.77
#define D5 587.33
#define CS5 554.37
#define FS5 739.99
#define A5 880
#define GS5 830.61
#define E5 659.25
#define FS3 185

#define MAX_NOTES 213
double sm_array[MAX_NOTES] = {E7, E7, 0, E7, 0, C7, E7, 0,
								G7, 0, 0, 0, G6, 0, 0, 0,
								C7, 0, 0, G6, 0, 0, E6, 0,
								0, A6, 0, B6, 0, AS6, A6, 0,
								G6, E7, G7, A7, 0, F7, G7, 0,
								E7, 0 , C7, D7, B6, 0, 0, C7,
								 0, 0, G6, 0, 0, E6,
								0, 0, A6, 0, B6, 0, AS6, A6, 0,
								G6, E7, G7, A7, 0, F7, G7, 0,
								E7, 0, C7, D7, B6, 0, 0,
								//underworld theme music
								C4, C5, A3, A4, AS3, AS4, 0, 0, C4, C5, A3, A4, AS3, AS4, 0 , 0,
								F3, F4, D3, D4, DS3, DS4, 0, 0, F3, F4, D3, D4, DS3, DS4, 0, 0,
								DS4, CS4, D4, CS4, DS4, DS4, GS3, G3, CS4, C4, FS4, F4, E3, AS4,
								A4, GS4, DS4, B3, AS3, A3, GS3, 0, 0 ,0,
								E7, E7, 0, E7, 0, C7, E7, 0,
									G7, 0, 0, 0, G6, 0, 0, 0,
									C7, 0, 0, G6, 0, 0, E6, 0,
									0, A6, 0, B6, 0, AS6, A6, 0,
									G6, E7, G7, A7, 0, F7, G7, 0,
									E7, 0 , C7, D7, B6, 0, 0, C7,
									0, 0, G6, 0, 0, E6,
									0, 0, A6, 0, B6, 0, AS6, A6, 0,
									G6, E7, G7, A7, 0, F7, G7, 0,
									E7, 0, C7, D7, B6, 0, 0, 1};

unsigned char sm_time_array[MAX_NOTES] = {1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1 , 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1,
											//underworld theme music
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 , 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1 ,1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1 , 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1,};

unsigned char sm_wait_array[MAX_NOTES] = {1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1 , 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1,
											//underworld theme music
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 , 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1 ,1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1 , 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1,};

#define sw_number 81
double sw_array[sw_number] = {F6, F6, F6, 0, AS6, 0, F7, 0,
							 DS7,  D7, C7, AS7, F7, 0,  DS7,
							 D7, C7,  AS7, F7, 0, DS7,
							D7, DS7, C7, 0,
							F6, F6, F6, 0, AS6, 0, F7, 0,
							DS7,  D7, C7, AS7, F7, 0,  DS7,
							D7, C7,  AS7, F7, 0, DS7,
							D7, DS7, C7, 0,
							F5, F5, F5, 0, AS5, 0, F6, 0,
							DS6,  D6, C6, AS6, F6, 0,  DS6,
							D6, C6,  AS6, F6, 0, DS6,
							D6, DS6, C6, 0, 1};

unsigned char sw_time_array[sw_number] = {1, 1, 1, 1, 15, 4, 15, 1,
											4, 2, 2, 15, 5, 1,
											4, 2, 2, 15, 5, 1,
											2, 2, 2, 15, 1,
											1, 1, 1, 1, 15, 4, 15, 1,
											4, 2, 2, 15, 5, 1,
											4, 2, 2, 15, 5, 1,
											2, 2, 2, 15, 1,
											1, 1, 1, 1, 15, 4, 15, 1,
											4, 2, 2, 15, 5, 1,
											4, 2, 2, 15, 5, 1,
											2, 2, 2, 15, 1,
											};

unsigned char sw_wait_array[sw_number] = {1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1, 1, 1};

//change journey to crazy train

#define journey_number 98
double journey_array[journey_number] = {FS3, FS3, 0, A3, A3, 0, E3, E3, 0, FS3, FS3, 0,
										D4, D4, 0, CS4, CS4, 0, FS3, FS3, 0, A3, A3, 0, E3, E3, 0, FS3, FS3, 0,
										D4, D4, 0, CS4, CS4, 0, FS3, FS3, 0, A3, A3, 0, E3, E3, 0, FS3, FS3, 0,
										//intro bass^
										FS5, FS5, CS6, FS5, D6, FS5, CS7, FS5,
										B5, A5, GS5, A5, B5, A5, GS5, E5,
										FS5, FS5, CS6, FS5, D6, FS5, CS7, FS5,
										B5, A5, GS5, A5, B5, A5, GS5, E5,
										FS5, FS5, CS6, FS5, D6, FS5, CS7, FS5,
										B5, A5, GS5, A5, B5, A5, GS5, E5, FS5, 1 };

unsigned char journey_time_array[journey_number] = {2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15,
													2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15,
													2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15, 2, 2, 15,
													//intro bass
													2, 2, 2, 2, 2, 2, 2, 2,
													2, 2, 2, 2, 2, 2, 2, 2,
													2, 2, 2, 2, 2, 2, 2, 2,
													2, 2, 2, 2, 2, 2, 2, 2,
													2, 2, 2, 2, 2, 2, 2, 2,
													2, 2, 2, 2, 2, 2, 2, 2, 10
													};

unsigned char journey_wait_array[journey_number] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
													//intro bass
													1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1,
													1, 1, 1, 1, 1, 1, 1, 1, 1
													};

#define roses_number 120 //95
double roses_array[roses_number] = {CS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									CS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									DS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									DS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									FS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									FS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									CS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									CS6, CS7, GS6, FS6, FS7, GS6, F7, GS6,
									//INTRO ^
									GS7, GS7, GS7, FS7, F7, FS7, GS7, DS7,
									CS7, FS7, F7, CS7, FS7, F7, CS7, FS7,
									F7, DS7, 1};

unsigned char roses_time_array[roses_number] = {
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												//INTRO^
												4, 8, 15, 4, 4, 4, 4, 4,
												4, 7, 5, 2, 7, 5, 2, 4,
												4, 4, 4, 4, 4, 4, 4, 4,
												4, 4, 4, 4, 4, 4, 4, 4};

unsigned char roses_wait_array[roses_number] = {1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1, 1, 1, 1};




#define iron_man 56
double iron_array[iron_man] = {B3, D4, D4, E4, E4, G4, FS4, G4, FS4, G4, FS4, D4, D4, E4, E4,
								0, 0, 0, 0, 0,
								B3, D4, D4, E4, E4, G4, FS4, G4, FS4, G4, FS4, D4, D4, E4, E4,
								0, 0, 0, 0, 0,
								B5, D6, D6, E6, E6, G6, FS6, G6, FS6, G6, FS6, D6, D6, E6, E6, 1};

unsigned char iron_time_array[iron_man] = {20, 15, 10, 8, 8, 2, 2, 2, 2, 2, 2, 8, 6, 8, 8,
											2, 2, 2, 2, 2,
											20, 15, 10, 8, 8, 2, 2, 2, 2, 2, 2, 8, 6, 8, 8,
											2, 2, 2, 2, 2,
											20, 15, 10, 8, 8, 2, 2, 2, 2, 2, 2, 8, 6, 8, 8,
												};

unsigned char iron_wait_array[iron_man] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
											1, 1, 1, 1, 1,
											1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
												};


//this is final countdown
#define train 85
//C# B C# F# D C# D C# B D C# D
//F# B A B A G# B A G# A B A B
//C# B A G# F D C# C# C# D C# B C#
double train_array[train] = {CS7, B6, CS7, FS6, 0, D7, CS7, D7, CS7, B6, 0, D7, CS7, D7,
							FS6, 0, B6, A6, B6, A6, GS6, B6, A6,
							GS6, A6, B6, A6, B6,
							CS7, B6, A6, GS6, F6, D7, CS7, CS7, CS7, D7, CS7, B6, CS7, 0,
							CS7, B6, CS7, FS6, 0, D7, CS7, D7, CS7, B6, 0, D7, CS7, D7,
							FS6, 0, B6, A6, B6, A6, GS6, B6, A6,
							GS6, A6, B6, A6, B6,
							CS7, B6, A6, GS6, F6, D7, CS7, CS7, CS7, D7, CS7, B6, CS7, 0, 1};

unsigned char train_time_array[train] = {1, 1, 7, 12, 6, 1, 1, 1, 1, 12, 6, 1, 1, 7, 12, 6,
										 1, 1, 1, 1, 1, 1, 7,
										1, 1, 7, 1, 1,
										7, 1, 1, 1, 7, 7, 7, 1, 1, 7,
										7, 7, 7, 6,
										1, 1, 7, 12, 6, 1, 1, 1, 1, 12, 6, 1, 1, 7, 12, 6,
										1, 1, 1, 1, 1, 1, 7,
										1, 1, 7, 1, 1,
										7, 1, 1, 1, 7, 7, 7, 1, 1, 7,
										7, 7, 7, 6
									};

unsigned char train_wait_array[train] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1, 1,
										1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
										1, 1, 1, 1
									};





void tick_melody(void) {
	static unsigned char index = 0;
	static unsigned char count = 0;
	switch (melody_state) { // Transitions

		case WAIT:
			index = 0;
			count = 0;
			if (SONG1) {
				melody_state = PLAY_NOTE;
				//SONG1 = 0;
			}
			else if (SONG2){
				melody_state = play_sw;
				//SONG2 = 0;
			}
			else if (SONG3){
				melody_state = play_journey;
				//SONG3 = 0;
			}
			else if(SONG4){
				melody_state = play_roses;
				//SONG4 = 0;
			}
			else if(SONG5){
				melody_state = play_iron;
				//SONG5 = 0;
			}
			else if(SONG7){
				melody_state = play_train;
			}
			else {
				melody_state = WAIT;
			}
			break;

		case PLAY_NOTE:
			if (count < sm_time_array[index])
			{
				set_PWM(sm_array[index]);
				count++;
				melody_state = PLAY_NOTE;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = WAIT_NOTE;
			}
			break;

		case WAIT_NOTE:
			if (count < sm_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = WAIT_NOTE;
			}
			else {
				count = 0;
				if (sm_array[index] != '1') {
					index++;
					melody_state = PLAY_NOTE;
				}
				else {
					SONG1 = 0;
					SONG7 = 1;
					melody_state = WAIT;
				}
			}
			break;

		case WAIT_PRESS:
			if (!GetBit(PINA, 0)) {
				melody_state = WAIT_PRESS;
			}
			else {
				melody_state = WAIT;
			}
			break;

		case play_sw:
			if (count < sw_time_array[index])
			{
				set_PWM(sw_array[index]);
				count++;
				melody_state = play_sw;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = wait_note_sw;
			}
			break;

		case wait_note_sw:
			if (count < sw_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = wait_note_sw;
			}
			else {
				count = 0;
				if (sw_array[index] != '1') {
					index++;
					melody_state = play_sw;
				}
				else {
					SONG2 = 0;
					SONG7 = 1;
					melody_state = WAIT;
				}
			}
			break;

		case play_journey:
			if (count < journey_time_array[index])
			{
				set_PWM(journey_array[index]);
				count++;
				melody_state = play_journey;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = wait_journey;
			}
			break;

		case wait_journey:
			if (count < journey_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = wait_journey;
			}
			else {
				count = 0;
				if (journey_array[index] != '1') {
					index++;
					melody_state = play_journey;
				}
				else {
					SONG3 = 0;
					SONG7 = 1;
					melody_state = WAIT;
				}
			}
			break;

		case play_roses:
			if (count < roses_time_array[index])
			{
				set_PWM(roses_array[index]);
				count++;
				melody_state = play_roses;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = wait_roses;
			}
			break;

		case wait_roses:
			if (count < roses_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = wait_roses;
			}
			else {
				count = 0;
				if (roses_array[index] != '1') {
					index++;
					melody_state = play_roses;
				}
				else {
					SONG4 = 0;
					SONG7 = 1;
					melody_state = WAIT;
				}
			}
			break;


			case play_iron:
			if (count < iron_time_array[index])
			{
				set_PWM(iron_array[index]);
				count++;
				melody_state = play_iron;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = wait_iron;
			}
			break;

		case wait_iron:
			if (count < iron_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = wait_iron;
			}
			else {
				count = 0;
				if (iron_array[index] != '1') {
					index++;
					melody_state = play_iron;
				}
				else {
					SONG5 = 0;
					SONG7 = 1;
					melody_state = WAIT;
				}
			}
			break;

		case play_train:
			if (SONG1 || SONG2 || SONG3 || SONG4 || SONG5){
				melody_state = WAIT;
				break;
			}
			if (count < train_time_array[index])
			{
				set_PWM(train_array[index]);
				count++;
				melody_state = play_train;
			}
			else {
				count = 0;
				/*if (index < MAX_NOTES -1){
					melody_state = PLAY_NOTE;
					++index;
				}
				else{
					melody_state = WAIT_PRESS;
				}*/
				melody_state = wait_train;
			}
			break;

		case wait_train:
			if (SONG1 || SONG2 || SONG3 || SONG4 || SONG5){
				melody_state = WAIT;
				break;
			}
			if (count < train_wait_array[index]) {
				set_PWM(0);
				count++;
				melody_state = wait_train;
			}
			else {
				count = 0;
				if (train_array[index] != 1) {
					index++;
					melody_state = play_train;
				}
				else {
					index = 0;
					melody_state = play_train;
				}
			}
			break;

		default:
			melody_state = WAIT;
			break;
	}
}

void transmit_data(unsigned char data) {
	int temp;

	for (temp = 7; temp >= 0 ; temp--) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> temp) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the �Shift� register to the
	//�Storage� register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;




}

enum States {init, display} state;
unsigned char seg7=0x00;
void Tick_display(){

	switch (state){
		case init:
			state = display;
			break;

		case display:
			state = display;
			break;
	}

	switch (state){

		case init:
			break;

		case display:
			if(seg7==0x00)
			{
				transmit_data(0xC0);
			}
			if (seg7 == 0x01){
				transmit_data(0xF9);
			}
			if (seg7 == 0x02){
				transmit_data(0xA4);
			}
			if (seg7 == 0x03){
				transmit_data(0xB0);
			}
			if (seg7 == 0x04){
				transmit_data(0x99);
			}
			if (seg7 == 0x05){
				transmit_data(0x92);
			}
			if (seg7 == 0x06){
				transmit_data(0x82);
			}
			if (seg7 == 0x07){
				transmit_data(0xF8);
			}
			if (seg7 == 0x08){
				transmit_data(0x80);
			}


			break;

	}
}
enum SM_States5 {Start5, Rec_SPI} SM_State5;
void SM_Tick5()
{
	switch (SM_State5)
	{
		case Start5:
		SM_State5=Rec_SPI;
		break;

		case Rec_SPI:
		seg7=(PIND&0x0F);
		SM_State5=Rec_SPI;
		break;

		default:break;
	}
}

enum SM_states6 {Start6, ADC_val} SM_state6;
void SM_Tick6()
{
	switch(SM_state6)
	{
		case Start6:
		SM_state6=ADC_val;
		break;

		case ADC_val:
		;
		unsigned short adcval=ADC;
		if(adcval<512)
		{
			PORTA=PORTA|0xFE;
		}
		else
		{
			PORTA=PORTA&0x01;
		}
		SM_state6=ADC_val;
		break;
	}
}

int main(void) {
	// Inputs
	DDRA = 0xFE; PORTA = 0x01;

	// Outputs
	DDRB = 0xFF; PORTB = 0x00;

	DDRC = 0XFF; PORTC = 0x00;
	DDRD=0x00; PORTD=0xFF;

	ADC_init();
	unsigned char time_counter = 0;
	// Timer setup
	TimerSet(1);
	TimerOn();

	// PWM Setup
	PWM_on();
	set_PWM(0);

	melody_state = WAIT;
	SM_State5=Start5;
	SM_state6=Start6;
	while(1) {
		unsigned char input1 = (PIND&0x70)>>4;
		if (input1==0x01){
			SONG1 = 1;
		}
		if (input1==0x02){
			SONG2 = 1;
		}
		if (input1==0x03){
			SONG3 = 1;
		}
		if (input1==0x04){
			SONG4 = 1;
		}
		if (input1==0x05){
			SONG5 = 1;
		}
		if (input1==0x00){
			SONG7 = 1;

		}

		SM_Tick5();
		SM_Tick6();
		Tick_display();

		if (time_counter == 35){
			tick_melody();
			time_counter=0;
		}
		++time_counter;
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
