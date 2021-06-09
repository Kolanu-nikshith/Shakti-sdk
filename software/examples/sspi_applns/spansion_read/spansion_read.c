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

void jumpToRAM()
{
	printf("\n%s\n","Control transferred to RAM");
	asm volatile("fence.i");
	asm volatile( "li x30, 0x80000000" "\n\t"
			"jr x30" "\n\t"
			:
			:
			:"x30","cc","memory"
		    );
}


/** @fn void main()
 * @brief Configures the SPI flash and writes into a flash location.
 * @details Configures the SPI flash, Confirms the flash device id,erases a sector
 *           and then write into a flash location and prints the value.
 */
void main()
{
	int write_address = FLASH_BASE_ADDRESS;  // read/write from/to this address
	int read_address  = FLASH_BASE_ADDRESS;  // read/write from/to this address
	int data = 0xDEADBEEF, count = 0; //32 bits of data can be written at a time
	uint32_t length = 11;
	int* bram_address = (int*) 0x80000000;

	sspi_init();
	flash_init();
	printf("SPI init done\n");
	flash_register_read(0x35);
	flash_register_read(0x05);

	flash_device_id();
	waitfor(200);
	printf("\n Flash device id read complete");

#if 1
	count = flash_read(read_address);
	printf("\n count value: %x", count);
	read_address += 4;
	for(int i = 1; i < count; i++)
	{
		data = flash_read(read_address);
//		printf("\n Addr: %x; wdata: %x; rdata: %x", read_address, write_data[i], data);
		if(data != write_data[i])
			{
				printf("\n Error in read data @ Address: %x", read_address);
				return 1;
			}
				*(bram_address) = data;
	bram_address++;
	read_address += 4;
	}

	printf("\n\nread complete.\n");
	jumpToRAM();

#else
	int write_data[WRITE_SIZE] = {0};
	int read_data[READ_SIZE] = {0};
	printf("\n Data read after erase:")	;
	length = READ_SIZE;
//	for(int i = 0; i < length; i++)
	{
		flash_read_locations( read_address, length, &read_data);
	}
//	while(1);
	length = WRITE_SIZE;
	data = 0x0;
	for(int i = 0; i < length; i++)
	{
		flash_write( write_address, data);
		write_data[i] = data;
		data += 0x01010101;
		write_address += 0x04;
	}
	length = READ_SIZE;
		flash_read_locations( read_address, length, &read_data);

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
	asm volatile ("ebreak");
#endif
}
