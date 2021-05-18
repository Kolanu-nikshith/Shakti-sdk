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

#define  WRITE_SIZE 256
#define  READ_SIZE 256

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
	int write_address = 0x00b00004;  // read/write from/to this address
	int read_address  = 0x00b00004;  // read/write from/to this address
	int data = 0xDEADBEEF; //32 bits of data can be written at a time
	uint32_t length = 11;

	sspi_init();

	printf("SPI init done\n");

	flash_device_id();
	waitfor(200);
	flash_erase(0x0b000000); //erases an entire sector
	printf("\nErase complete");

#if 0
	flash_read_locations(read_address, length);

	//flash write
	flash_write( write_address, 0x12345678);
	flash_write( write_address + 0x04, 0xaaaaaaaa);
	flash_write( write_address + 0x08, 0x55555555);
	flash_write( write_address + 0x0C, 0xAAAA5555);
	flash_write( write_address + 0x10, 0x5555AAAA);
	flash_write( write_address + 0x14, 0xaaa5aaa5);
	flash_write( write_address + 0x18, 0x555A555A);
	flash_write( write_address + 0x1c, 0xaa55aa55);
	flash_write( write_address + 0x20, 0x55aa55aa);
	flash_write( write_address + 0x24, 0x5a5a5a5a);
	flash_write( write_address + 0x28, 0xa5a5a5a5);

	printf("\nFlash write done on address %x and data %x \n", 
	       write_address, data);

	printf("\n After Write");
	flash_read_locations(read_address, length);
#else
	
	length = WRITE_SIZE;
	int write_data[WRITE_SIZE] = {0};
	int read_data[READ_SIZE] = {0};
	data = 0x0;
	for(int i = 0; i < length; i++)
	{
		flash_write( write_address + 0x04, data);
		write_data[i] = data;
		data += 0x01010101;
	}
	length = READ_SIZE;
	for(int i = 0; i < length; i++)
	{
		flash_read_locations( read_address, length, &read_data);
	}

	for(int i = 0; i < length; i++)
	{
		if(read_data[i] != write_data[i] )
		{
			log_error("\n Flash test failed");
			log_error("\n write_value: %x; read_value: %x", write_data[i], read_data[i]);
			return;
		}
	}
	log_info("\n Flash Test passed");
#endif
	asm volatile ("ebreak");
}
