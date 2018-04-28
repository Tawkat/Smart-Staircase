#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000
#include <util/delay.h>

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include "lcd.h"

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))
volatile int ofc1=0,ofc2=-1,ofc3=-1;
#define DURATION 124


ISR(TIMER1_OVF_vect)
{
	if(ofc1>0)
	{
		ofc1--;
		if(ofc1==0)
		{
			PORTD&=~(1<<PD0);
			//TIMSK=0x00;
		}
	}
	if(ofc2>0)
	{
		ofc2--;
		if(ofc2==0)
		{
			PORTD&=~(1<<PD1);
			//TIMSK=0x00;
		}
	}
	if(ofc3>0)
	{
		ofc3--;
		if(ofc3==0)
		{
			PORTD&=~(1<<PD2);
			//TIMSK=0x00;
		}
	}
}


int main(void)
{
	TCCR1A=0x00;
	TCCR1B=0x01;
	TIMSK=0x04;
	
	DDRB = 0b11111100; //PB0,PB1 are used as inputs and reset are used as outputs
	DDRA=0b00000000; //PA is used as input
	DDRD=0b11111111; //PD is used as output
	DDRC = 0xFF;
	int x=0; // taking a integer
	
	Lcd4_Init();
	sei();
	while(1)
	{
		Lcd4_Set_Cursor(1,1);
		char p[5];
		itoa(ofc1,p,10);
		Lcd4_Write_String(p);
		if (BIT_IS_CLEAR(PINB,1)) // Checking Light
		{
				if(BIT_IS_SET(PINA,0))//if PIR 0
				{
					PORTD |=(1<<PIND0);
					//_delay_ms(8000);
					//PORTD &=~(1<<PIND0);
					//_delay_ms(5000);
					ofc1=DURATION;
					//TIMSK=0x04;
				}
				if(BIT_IS_SET(PINA,1))//if PIR 1
				{
					PORTD |=(1<<PIND1);
					//_delay_ms(8000);
					//PORTD &=~(1<<PIND1);
					//_delay_ms(5000);
					ofc2=DURATION;
				}
				if(BIT_IS_SET(PINA,2))//if PIR 2
				{
					PORTD |=(1<<PIND2);
					//_delay_ms(8000);
					//PORTD &=~(1<<PIND2); //turn on transistor trigger
					//_delay_ms(5000);
					ofc3=DURATION;
				}
				/*if(x==1)
				{
					_delay_ms(8000);
					PORTD=0x00;
					x=0;
				}*/
			
		}
	}
}
