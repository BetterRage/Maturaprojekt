#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "defines.h"
#include "stepper.h"
#include <stdlib.h>

static uint16_t target_xfreq=0;
static uint16_t target_yfreq=0;
static int8_t man_xvel=0;
static int8_t man_yvel=0;
char strcoords[20]; //21 byte f�r anzahl der punkte und 2 byte pro punkt mit max. 10 punkte
target targets[10];
uint8_t targetcnt=0;
uint8_t pointsreceived = 0;
steptypes man_dirx;
steptypes man_diry;
char stringx[7];
char stringy[7];
uint8_t mode=1;
//LED PB5

void init_adc();
uint8_t read_adc(uint8_t pinmsk);

ISR(USART_RX_vect)
{
	uint8_t sreg = SREG;
	SREG&=~(1<<7);
	targetcnt = _getch();
	uint8_t i;
	for(i=0;i<2*targetcnt;i++)
		strcoords[i]=_getch();
	strcoords[i]=0;
	_putslen(strcoords,2*targetcnt);
	
	for(i=0;i<targetcnt;i++)
	{
		targets[i].x=strcoords[2*i];
		targets[i].y=strcoords[2*i+1];
		targets[i].pos=pointfrompos(targets[i].x,targets[i].y);
	}
	pointsreceived=1;
	SREG = sreg;
}
ISR(TIMER0_OVF_vect)
{
	uint8_t sreg = SREG;
	cli();
	int8_t target_xvel = read_adc(0)-128;
	int8_t target_yvel = read_adc(1)-128;
	
	
	itoa(target_yvel,stringy,10);
	itoa(target_xvel,stringx,10);
	_putch('Y');
	_puts(stringy);
	_putch('X');
	_puts(stringx);
	_newline();
	_delay_ms(10);
	
	DDRB|=(1<<5);
	PORTB|=(1<<5);
	if(abs(target_xvel)<10)
		target_xvel=0;
	if(abs(target_yvel)<10)
		target_yvel=0;
		
	
			
	if(target_xvel>man_xvel)
		man_xvel++;
	if(target_xvel<man_xvel)
		man_xvel--;
	if(target_yvel>man_yvel)
		man_yvel++;
	if(target_yvel<man_yvel)
		man_yvel--;
	
	
	
	if(man_xvel>0)
		man_dirx  = XGO;
	else 
		man_dirx = XCOME;
	if(man_yvel>0)
		man_diry  = YGO;
	else
		man_diry = YCOME;
		
	target_xfreq = abs(man_xvel)<<4;
	target_yfreq = abs(man_yvel)<<4;
	
	
	
	uint16_t ocrx_target=(uint16_t)15625.0/target_xfreq;
	uint16_t ocry_target=(uint16_t)15625.0/target_yfreq;
	uint8_t ocrx = ocrx_target<=255?ocrx_target:255;
	uint8_t ocry = ocry_target<=255?ocry_target:255;
	
	
	
	
	if(man_xvel==0)
		TCCR1B&=0b11111000;
	else
	{
		OCR1A=ocrx;
		TCCR1B|=0b101;
	}
	if(man_yvel==0)
		TCCR2B&=0b11111000;
	else
	{
		OCR2A=ocry;
		TCCR2B|=0b111;
	}
	
	SREG = sreg;
}
ISR(TIMER1_COMPA_vect) //stepper x routine
{
	uint8_t sreg = SREG;
	cli();
	step(man_dirx);
	SREG = sreg;
}
ISR(TIMER2_COMPA_vect) //stepper y routine
{
	uint8_t sreg = SREG;
	cli();
	step(man_diry);
	SREG = sreg;
}

int main(void)
{
	DDRC=0;
	PORTC|=1<<JOYSTICK_SW;
	
	DDRD|=0b111<<3;
	DDRB|=0b111;
	
	mode =1;
	
	SREG|=(1<<7);
		
	#pragma region AutoMode
	init_usart9600();
	while (1)
	{
		if(pointsreceived)
		{
			for(uint8_t i=0;i<targetcnt;i++)
			{
				move(XGO,targets[i].pos.x);
				move(YGO,targets[i].pos.y);
				move(YCOME,targets[i].pos.y);
				move(XCOME,targets[i].pos.x);
			}
			pointsreceived=0;
		}
	}
	#pragma endregion
	#pragma region ManualMode
	manual:
		init_adc();
		init_usart9600();
		SREG|=(1<<7);
		TCCR0A = 0;
		TCCR0B = 0b101; //prescaler 1/1024
		TIMSK0 |=1<<TOIE0;
		
		__asm__ volatile("sei");	
		TCCR1A = 0;
		TCCR1B = 0b00001000;
		TIMSK1 |= 1<<OCIE1A;
		TCCR2A = 2;
		TCCR2B=0;
		TIMSK2 |=OCIE2A;
		while(1);
	#pragma endregion
}
void init_adc()
{
	ADCSRA=0x87;
	ADMUX=0x60;
}
uint8_t read_adc(uint8_t pinmsk)
{
	ADMUX &= ~ (0b1111);
	ADMUX |= pinmsk;
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	return ADCH;
}

