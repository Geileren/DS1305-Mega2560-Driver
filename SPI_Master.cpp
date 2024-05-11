/*
 * SPI_Driver.cpp
 *
 * Created: 11-05-2024 12:39:30
 *  Author: Geileren
 */ 

#include "SPI_Master.h"

void SPI_Master::InitSPI()
{
	// Setting MOSI, SCK and SS as output (thereby also setting MISO as input)
	DDRB = (1 << 2) | (1 << 1) | (1 << 0);
	
	// Enabling SPI and setting to master-mode. Clock is set to 1 MHz. Data-order is set MSB, and interrupt is disabled
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	// Setting Clock Polarity and Phase to rising leading edge and sampling on the trailing edge
	SPCR |= (1 << CPHA);
	
}

uint8_t SPI_Master::Transfer(uint8_t byteToSend)
{
	// Loading the byte to send. This starts the transfer
	SPDR = byteToSend;
	DDRB |= (1<<7);
	
	// Waiting until the transfer is complete
	while(!(SPSR & (1<<SPIF))){
		PORTB |= (1<<7);
	}

	// Return the received data
	uint8_t recievedByte = SPDR;
	return recievedByte;
}

void SPI_Master::SS_High()
{
	PORTB |= (1 << 0);
}

void SPI_Master::SS_Low()
{
	PORTB &= ~(1 << 0);
}