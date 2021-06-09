/***************************************************************************
* Project                               : shakti devt board
* Name of the file                      : spi.c
* Brief Description of file             : Erase a sector of flash
* Name of Author                        : Sathya Narayanan
* Email ID                              : sathya281@gmail.com

 Copyright (C) 2020  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
/**
@file erase.c
@brief  Erase a sector of on-board flash in arty 35t/100t
@detail Contains the code to erase a sector of on-board flash using spi
protocol.
*/

#include <stdint.h>
#include "sspi.h"
#include "uart.h"
#include "flashdata.h"


#define  WRITE_SIZE write_data[0]
#define  READ_SIZE 10
#define FLASH_BASE_ADDRESS 0x00b00000

/** @fn static void flash_read_locations(uint32_t read_address, uint16_t length)
 * @brief Reads the flash memory contents starting from read_address.
          Totally length number of bytes will be read.
 * @param unsigned int (32-bit).
 * @param unsigned int (16-bit).           
 */
#if 0
static void flash_read_locations(uint32_t read_address, uint16_t length)
{
	for(int i = 0; i < length; ++i)
	{
		int read_value = flash_read(read_address);
		printf("\nReading from adddress %x and data \
			   %x\n",read_address,read_value);
		read_address = read_address+4;
	}
}
#else
static void flash_read_locations(uint32_t read_address, uint32_t length, int *read_data)
{
	for(int i = 0; i < length; ++i)
	{
		int read_value = flash_read(read_address);
		printf("\nReading from adddress %x and data \
			   %x\n",read_address,read_value);
		*read_data++ = read_value;
		read_address = read_address+4;
	}
}
#endif

/** @fn void main()
 * @brief Configures the SPI flash and writes into a flash location.
 * @details Configures the SPI flash, Confirms the flash device id,erases a sector
 *           and then write into a flash location and prints the value.
 */
void main()
{
	int write_address = FLASH_BASE_ADDRESS;  // read/write from/to this address
	int read_address  = FLASH_BASE_ADDRESS;  // read/write from/to this address
	int data = 0xDEADBEEF; //32 bits of data can be written at a time
	uint32_t length = 11;

	sspi_init();
	flash_init();
	printf("SPI init done\n");
	flash_register_read(0x35);
	flash_register_read(0x05);

	flash_device_id();
	waitfor(200);
	printf("\n Flash device id read complete");
	flash_erase(FLASH_BASE_ADDRESS); //erases an entire sector
	printf("\nErase complete");

	flash_write(read_address,write_data[0]);
	read_address+=4;

	printf("\nWriting...");

	for(int i =0; i< write_data[0]; i++)
	{
		waitfor(200);
		flash_write(read_address,write_data[i+1]);
		read_address+=4;

		if(i%512 == 0)
			printf(".");;
	}

	printf("\n\nWrite complete.\n");
	printf("\nPlease reset.\n");
	asm volatile ("ebreak");

}
