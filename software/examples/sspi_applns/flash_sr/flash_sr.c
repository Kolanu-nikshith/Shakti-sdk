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

#define  WRITE_SIZE 10
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
#define SSPI_INSTANCE 0
void test_spi()
{
	sspi_instance[SSPI_INSTANCE]->data_tx = 0;	//MSB byte of address.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0xb0;	//2nd byte of address.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0;	//3rd byte of address.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0;	//4th byte of address.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0x55;	//4th byte of data.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0xaa;	//4th byte of data.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0x55;	//4th byte of data.
	sspi_instance[SSPI_INSTANCE]->data_tx = 0xaa;	//4th byte of data.
	sspi_configure_tx_rx_length(sspi_instance[SSPI_INSTANCE], 64, 0);//Tx - 72, Rx - 0;
	sspi_configure_comm_mode(sspi_instance[SSPI_INSTANCE], SIMPLEX_TX);
	sspi_enable_txrx(sspi_instance[SSPI_INSTANCE], ENABLE);
#ifdef SHOW_REG
	sspi_read_registers(sspi_instance[SSPI_INSTANCE]);
#endif	
//	sspi_notbusy();



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
	int data = 0xDEADBEEF; //32 bits of data can be written at a time
	uint32_t length = 11;

	sspi_init();
	flash_init();
	while(1)
//		test_spi();	
		flash_status_register_read();

	asm volatile ("ebreak");
}
