#include "UART.h"

void init_usart9600()
{
    UBRR0L = 103;
    UBRR0H = 0;
    UCSR0B |= (1<<TXEN0) | (1<<RXEN0)|(1<<RXCIE0);           // Sender und Empfänger aktivieren
    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);         // 8-Bit Übertragung
 // Asynchrone Übertragung (UMSEL00=0 und UMSEL01=0)
}

void _putch(char ch)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = ch;
}

void _putslen(char st[],uint8_t len)
{
	for(int i=0;i<len;i++)
		_putch(st[i]);
}

void _puts(char st[])
{
	uint8_t i = 0;
	while (st[i])
		_putch(st[i++]);
}

char _getch(void)
{
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

uint8_t _gets(char* st)
{
	uint8_t len=0;
    do
    {
		len++;
        *st=_getch();
    } while (*st++); 
	return len;
}

void _newline()
{
    _putch(13);   // CR
    _putch(10);   // LF
}