/* Partner 1 Name & E-mail: Jeet Thakkar jthak002@ucr.edu
* Partner 2 Name & E-mail: Kevin Nguyen knguy092@ucr.edu
* Lab Section: 021
* Assignment: Lab 1 Exercise 6
* Exercise Description: [In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is 1 if a driver is
seated, and PA6 is 1 if the driver's seatbelt is fastened. PC7 should light a "Fasten
seatbelt" icon if a key is in the ignition, the driver is seated, but the belt is not
fastened.]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>


int main(void)
{
    DDRA=0x00;PORTA=0xFF;
	DDRC=0xFF;PORTC=0x00;
	
	unsigned char ucValA=0x00;
	unsigned char ucSetC=0x00;
    while (1) 
    {
		ucValA=PINA;
		ucSetC=0x00;
		if( ((ucValA&0x0F)==0x00) )
		{
			ucSetC=ucSetC|0x40;	
		}
		if( ((ucValA&0x0F)==0x01) || ((ucValA&0x0F)==0x02) )
		{
			ucSetC=ucSetC|0x60;	
		}
		if( ((ucValA&0x0F)==0x03) || ((ucValA&0x0F)==0x04) )
		{
			ucSetC=ucSetC|0x70;
		}
		if( ((ucValA&0x0F)==0x05) || ((ucValA&0x0F)==0x06) )
		{
			ucSetC=ucSetC|0x38;
		}
		if( ((ucValA&0x0F)==0x07) || ((ucValA&0x0F)==0x08) || ((ucValA&0x0F)==0x09) )
		{
			ucSetC=ucSetC|0x3C;
		}
		if( ((ucValA&0x0F)==0x0A) || ((ucValA&0x0F)==0x0B) || ((ucValA&0x0F)==0x0C) )
		{
			ucSetC=ucSetC|0x3E;
		}
		if( ((ucValA&0x0F)==0x0D) || ((ucValA&0x0F)==0x0E) || ((ucValA&0x0F)==0x0F) )
		{
			ucSetC=ucSetC|0x3F;
		} 
		
		if( ((ucValA&0x10)==0x10) && ((ucValA&0x20)==0x20) && ((ucValA&0x40)==0x00)  )
		{
			ucSetC=ucSetC|0x80;
		}
		PORTC=ucSetC;
		
    }
}



