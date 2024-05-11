/*
 * DS1305_Driver.cpp
 *
 * Created: 11-05-2024 16:38:28
 *  Author: Geileren
 */ 
#include "DS1305_Driver.h"

const uint8_t writeAddressOffset = 0x80;

void DS1305_Driver::Init_DS1305()
{
	master.InitSPI();
	
	
	// Set control register, to enable clock and disable Write-protect.
	WriteToAddress(0x8f, 0x00);
	// Set to 24-hour mode
	WriteToAddress(0x82, 0x00);
	
}

uint8_t DS1305_Driver::ReadFromAddress(uint8_t address)
{
	uint8_t recievedData;
	
	// Enable DS1305
	master.SS_High();
		
	// set the adress and read the data
	master.Transfer(address);
	recievedData = master.Transfer(0x00);
		
	// End transaction
	master.SS_Low();
	
	return recievedData;
}

void DS1305_Driver::WriteToAddress(uint8_t address, uint8_t dataByte)
{
	// Enable DS1305
	master.SS_High();
	
	// Set the address and transfer the dataByte
	master.Transfer(address);
	master.Transfer(dataByte);
	
	// End transaction
	master.SS_Low();
}


uint8_t DS1305_Driver::ConvertDataToNumber(uint8_t data)
{
	// The tens are stored in the upper nibble while the ones are stored in the lower nibble
	uint8_t convertedNumber, upperNibble, lowerNibble;
	upperNibble = (data >> 4);
	lowerNibble = (data & 0b00001111);
	// adding them together
	convertedNumber = upperNibble * 10 + lowerNibble;
	return convertedNumber;
}

uint8_t DS1305_Driver::ConvertNumberToData(uint8_t number)
{
	// The tens are stored in the upper nibble while the ones are stored in the lower nibble
	uint8_t convertedData;
	// Finding the tens
	convertedData = (number / 10) << 4;
	// Finding the ones
	convertedData += (number % 10);
	return convertedData;
}

void DS1305_Driver::ReadAllTimeData(uint8_t *time)
{
	// looping through the read time addresses of the DS1305
	for (uint8_t address=0; address <= 6; address++){
		// reads from address and converts to real number-format
		time[address] = ConvertDataToNumber(ReadFromAddress(address));
	}
	
}

void DS1305_Driver::LoadNewTime(uint8_t *time)
{
	
	// Looping through the write time addresses (adding 0x80)
	for (uint8_t address=0; address <= 6; address++){
		// Converts number to data-format and writes to address
		WriteToAddress(address+writeAddressOffset, ConvertNumberToData(time[address]));
	}
}
