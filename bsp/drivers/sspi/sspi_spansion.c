/***************************************************************************
 * Project               	    	: "iisu project"
 * Name of the file	            	: sspi_driver.c
 * Brief Description of file            : Driver file to Standard SPI
 * Name of Author    	                : Sambhav Jain
 * Email ID                             : sambhav.jv@gmail.com;

  Copyright (C) 2021  IIT Madras. All rights reserved.

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
/*
  @file spi.c
  @brief read data from the flash by spi
  @detail Contains driver codes to read and write flash using SPI interface.
 */

#include "sspi.h"
#include "uart.h"
#include "utils.h"
#include "pinmux.h"

#define SSPI_DRIVER     1

 
int flash_write_enable(void)
{
 	sspi_configure_tx_rx_length(SSPI0,8,0);//Tx - 8, Rx - 0;
	sspi_configure_comm_mode(SSPI0,SIMPLEX_TX);
	SSPI0->data_tx = 6;	//MSB byte of address.
 	sspi_enable_txrx(SSPI0, ENABLE);
 	sspi_notbusy();
 	return 1;
}

int sspi_notbusy(void)
{
	unsigned char  read_data = 0x01;
	while(read_data & 0x01){
		waitfor(10);
		read_data = SSPI0->comm_status;
//		printf("\n Status Register: %x", read_data);
	}
//	printf("\n SPI comm done \n");
	return 1;
}


char flash_cmd_read(char command)
{
//	int dr5;
//	set_spi(spi_dr1, command);
//	set_spi(spi_dr5, command);
//	spi_tx_rx_start();
//	set_spi(spi_cr1, (SPI_BR(7)|SPI_TOTAL_BITS_TX(8)|SPI_TOTAL_BITS_RX(32)|SPI_SPE|SPI_CPHA|SPI_CPOL));
//	if(spi_rxne_enable()) {
//		dr5 = *spi_dr5;
//	}
//	return dr5;

	char recvDATA;
	SSPI0->data_tx =command;
	sspi_configure_tx_rx_length(SSPI0,8,8);
	sspi_configure_comm_mode(SSPI0,HALF_DUPLEX);
	sspi_enable_txrx(SSPI0, ENABLE);
	sspi_notbusy();	
	recvDATA = SSPI0->data_rx;
	return recvDATA;
}

int flash_status_register_read(void)
{
	char stat = 0x3;

	while (stat & 0x03)
	{
		stat = flash_cmd_read(0x05);
		log_debug("flash status register val %x\n", stat);
	}

	return 0;
}


/**
 * @fn int flash_cmd_addr(int command, int addr)
 * @brief Use for sending 8bit of command + 32 bit of address 
 * @details Useful for function like erase
 * @warning to move data drom dr register to fifo there must be some data into spi_dr5 
 * @param int (command (opcode))
 * @param int (addr (address after the opcode))
 * @return int
 */
int flash_cmd_addr(int command, int addr)
{

	SSPI0->data_tx =command;	//Do not shift  command when passing as argument.
	SSPI0->data_tx =(addr >> 24);	//MSB byte of address.
	SSPI0->data_tx =(addr >> 16);	//2nd byte of address.
	SSPI0->data_tx =(addr >> 8);	//3rd byte of address.
	SSPI0->data_tx =(addr >> 0);	//4th byte of address.
	sspi_configure_tx_rx_length(SSPI0,40,0);//Tx - 8, Rx - 0;
	sspi_configure_comm_mode(SSPI0,SIMPLEX_TX);
	sspi_enable_txrx(SSPI0, ENABLE);
	waitfor(20);
	sspi_notbusy();

	return 1;
}

void flash_erase(int address)
{
	flash_write_enable();
	flash_cmd_addr(0xdc, address);
	flash_status_register_read();
}


/**
 * @fn void flash_cmd_addr_data(int command, int addr, int data)
 * @brief useful for function like Write 
 * @details use for sending 8bit command +32bit of write address + 32 bit of write data
 * @warning to move data from data register to fifo there must be some data into spi_dr5
 * @param int (command (opcode))
 * @param int (addr(address after the opcode))
 * @param int (data (data after the address))
 */
void flash_cmd_addr_data(int command, int addr, int data)
{
	SSPI0->data_tx =command;	//Do not shift  command when passing as argument.
	SSPI0->data_tx =(addr >> 24);	//MSB byte of address.
	SSPI0->data_tx =(addr >> 16);	//2nd byte of address.
	SSPI0->data_tx =(addr >> 8);	//3rd byte of address.
	SSPI0->data_tx =(addr >> 0);	//4th byte of address.
	SSPI0->data_tx =(data >> 24);	//4th byte of data.
	SSPI0->data_tx =(data >> 16);	//4th byte of data.
	SSPI0->data_tx =(data >> 8);	//4th byte of data.
	SSPI0->data_tx =(data >> 0);	//4th byte of data.
	sspi_configure_tx_rx_length(SSPI0,72,0);//Tx - 72, Rx - 0;
	sspi_configure_comm_mode(SSPI0,SIMPLEX_TX);
	sspi_enable_txrx(SSPI0, ENABLE);
	sspi_notbusy();
	flash_status_register_read();
}


void flash_write(int address, int data)
{
	flash_write_enable();
	flash_cmd_addr_data(0x12, address, data);
	flash_status_register_read();
}

/**
 * @fn int flash_cmd_to_read(int command, int addr)
 * @briefUse useful for function like read
 * @details for sending command of 8bit + read address of 32bit + 8bit of dummy cycle and receive 
 *          32bit value from flash 
 * @warning As receive shoild start as soon as transmit state end, use spi_rx_tx_start() Before 
 *          setting control register 1
 * @param int (command (opcode))
 * @param int (addr(read_address))
 * @return int 
 */
int flash_cmd_to_read(int command, int addr)
{
	int recv_data = 0;
	SSPI0->data_tx =command;	//Do not shift  command when passing as argument.
	SSPI0->data_tx =(addr >> 24);	//MSB byte of address.
	SSPI0->data_tx =(addr >> 16);	//2nd byte of address.
	SSPI0->data_tx =(addr >> 8);	//3rd byte of address.
	SSPI0->data_tx =(addr >> 0);	//4th byte of address.
	sspi_configure_tx_rx_length(SSPI0,48,32);//Tx - 8, Rx - 0;
	sspi_configure_comm_mode(SSPI0,HALF_DUPLEX);
	sspi_enable_txrx(SSPI0, ENABLE);
	sspi_notbusy();
	// printf("\nrecv data 1");
	// recvData = SSPI0->data_rx;
	printf("\nrecv data 2");
	recv_data = (SSPI0->data_rx << 24) | (SSPI0->data_rx << 16) | (SSPI0->data_rx << 8 ) | (SSPI0->data_rx );
	return recv_data;
}

/**
 * @fn void flash_write(int address, int data)
 * @brief  Write 4bytes of data from given address
 * @details flash_cmd_addr_data with opcode 12h.  
 * @warning before writing into the flash one should enable the WEL bit spi_sr by using write_enable(void)
 * @param int (addres (write address))
 * @param int(data (write data))
 */

int flash_read(int address)
{
	int read_value = flash_cmd_to_read(0x0C,address);
	return read_value;
}



int flash_device_id(void)
{

	uint32_t recvData;
	SSPI0->data_tx =0x9f;		
	sspi_configure_tx_rx_length(SSPI0,8,24);//Tx - 8, Rx - 0;
	sspi_configure_comm_mode(SSPI0,HALF_DUPLEX);
	sspi_read_registers(SSPI0);
	sspi_enable_txrx(SSPI0, ENABLE);
	sspi_notbusy();
	printf("\nrecv data 2");
	recvData = (SSPI0->data_rx << 16) | (SSPI0->data_rx << 8 ) | (SSPI0->data_rx );
	return recvData;	
}


