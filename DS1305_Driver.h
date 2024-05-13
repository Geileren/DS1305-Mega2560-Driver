/*
 * DS1305_Driver.cpp
 *
 * Created: 11-05-2024 16:38:39
 *  Author: Geileren
 */ 

#include "SPI_Master.h"

class DS1305_Driver{
	public:
		void Init_DS1305();
		uint8_t ReadFromAddress(uint8_t address);
		void WriteToAddress(uint8_t address, uint8_t dataByte);
		void ReadAllTimeData(uint8_t *time);
		void LoadNewTime(uint8_t *time);
		
	private:
		SPI_Master master;
		uint8_t ConvertDataToNumber(uint8_t data);
		uint8_t ConvertNumberToData(uint8_t number);
	};
