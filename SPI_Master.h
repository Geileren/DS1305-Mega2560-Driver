
/*
 * SPI_Driver.h
 *
 * Created: 11-05-2024 12:39:18
 *  Author: Geileren
 */ 
#include <avr/io.h>
//#include <stdio.h>

class SPI_Master{
	public:
	void InitSPI();
	uint8_t Transfer(uint8_t byteToSend);
	void SS_High();
	void SS_Low();
	};
