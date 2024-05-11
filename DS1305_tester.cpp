/*
 * DS1305_Driver.cpp
 *
 * Created: 11-05-2024 16:36:36
 * Author : Geileren
 */ 

#include <avr/io.h>
#include "DS1305_Driver.h"

#define F_CPU 16000000
#include <util/delay.h>

//UART-functions to print to terminal
#include <stdlib.h> //uses itoa()
void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char Rx_Int);
void SendChar(char Tegn);
void SendString(char* Streng);
void SendInteger(int Tal);

int main(void)
{
	DS1305_Driver rtc;
	
    rtc.Init_DS1305();
	
	InitUART(9600, 8, 0);
	SendString("Start of program\n\r");
	
	uint8_t a[7], b[7];
	// Loading a new time on the DS1305. The format is a follows:
	// s = 25, m = 31, h = 14, weekday = 6 (Saturday), calender_day=11, month=5, year=24
	// 24-5-11 14:31:25 on a saturday
	b[0]=25;b[1]=31;b[2]=14;b[3]=3;b[4]=11;b[5]=5;b[6]=24;
	rtc.LoadNewTime(b);
    while (1) 
    {
		//read the time
		rtc.ReadAllTimeData(a);
		//date
		SendInteger(a[4]);
		SendChar('-');
		SendInteger(a[5]);
		SendChar('-');
		SendInteger(a[6]);
		SendChar(' ');
		
		//time of day
		SendInteger(a[2]);
		SendChar(':');
		SendInteger(a[1]);
		SendChar(':');
		SendInteger(a[0]);
		
		SendString("\n\r");
		_delay_ms(1000);
		
    }
}

//UART functions
void InitUART(unsigned long BaudRate, unsigned char DataBit, unsigned char RX_Int)
{

	if ((BaudRate >= 300) && (BaudRate <= 112500) && (DataBit >= 5) && (DataBit <=8)){
		UCSR0B = 0b00011000; //no interrupts, receiver and transmitter enable
		UCSR0C = (DataBit - 5) << 1;
		// baud = f_cpu/(16*(UBRRN+1))
		// UBRRN = (f_CPU/(16*baud)) - 1
		UBRR0 = F_CPU/(16*BaudRate) - 1;
	}
	
	if (RX_Int){
		UCSR0B |= 0b10000000; //Enables Receiver interrupt
	}
}

void SendChar(char character)
{
	while((UCSR0A & 0b00100000) == 0){	}
	UDR0 = character;
}

void SendString(char* text)
{
	while(*text != 0){
		SendChar(*text);
		text++;
	}
}

void SendInteger(int Tal)
{
	char charArray[6]; //1 space for every possible character
	itoa(Tal, charArray, 10);
	SendString(charArray);
}

