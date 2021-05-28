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
#include "pinmux.h"
#include "platform.h"
#define SSPI_INSTANCE 1

void dac_init()
{
 	sspi_enable_txrx(sspi_instance[SSPI_INSTANCE], DISABLE);
	sspi_configure_clock_in_hz(sspi_instance[SSPI_INSTANCE], 100000); /* Currently configuring for 100kHz, as the maximum clock is 20MHz */
	sspi_configure_clock_pol(sspi_instance[SSPI_INSTANCE], 1);
	sspi_configure_clock_phase(sspi_instance[SSPI_INSTANCE], 1);	
	sspi_configure_pins(sspi_instance[SSPI_INSTANCE], 1, 0 ,  1 , 1);
	sspi_configure_mas_slv(sspi_instance[SSPI_INSTANCE], SPI_MASTER);
	sspi_configure_lsb_first(sspi_instance[SSPI_INSTANCE], MSB_FIRST);

}
/** @fn int write_dac_value(unsigned dac_value)
 * @brief Writes the passed Value to DAC
 * @details Writes Digital value that needs to be convert into analog over SPI interface
 * @param dac_value
 * @return One.
 */
int write_mcp4921_value(unsigned dac_value)
{

#ifdef DISABLE_CS
	sspi_enable_txrx(sspi_instance[SSPI_INSTANCE], DISABLE);
#endif
	printf("\n Write data %x ",dac_value);
	//sspi_instance[SSPI_INSTANCE]->data_tx =  ((0x3 << 28) | (dac_value << 16));	//4th byte of data.
	sspi_instance[SSPI_INSTANCE]->data_tx = (0x30 | (dac_value >> 8)) & 0xFF ;	//4th byte of data.
	sspi_instance[SSPI_INSTANCE]->data_tx = (dac_value & 0xFF );	//4th byte of data.
	sspi_configure_tx_rx_length(sspi_instance[SSPI_INSTANCE], 16, 0);//Tx - 72, Rx - 0;
	sspi_configure_comm_mode(sspi_instance[SSPI_INSTANCE], SIMPLEX_TX);
	sspi_enable_txrx(sspi_instance[SSPI_INSTANCE], ENABLE);
#ifdef SHOW_REG
	sspi_read_registers(sspi_instance[SSPI_INSTANCE]);
#endif	
	sspi_notbusy();
	log_debug("\n DAC data is written");	
}

/** @fn void spi_dac()
 * @brief Configures DAC and write digital value to DAC.
 * @details configures the DAC and writes the digital value to DAC to convert 
 *          with digital to analog. Configuration is done using SPI.
 */
void spi_dac()
{
	*(pinmux_config_reg) =  0x154000;
	sspi_init();
	printf("SPI init done\n");
	dac_init();
	write_mcp4921_value(0xfff);
	log_debug("\nDac Value Written\n");

	while(1)
	{
		write_mcp4921_value(0xFFF);
		log_debug("Dac Value Written - 4096\n");

		delay_loop(0x1750, 0x1750);
		write_mcp4921_value(0x0);
		log_debug("Dac Value Written - 0\n");

		delay_loop(0x1750, 0x1750);
	}
} 


/** @fn void main()
 * @brief Configures the SPI flash and writes into a flash location.
 * @details Configures the SPI flash, Confirms the flash device id,erases a sector
 *           and then write into a flash location and prints the value.
 */
void main()
{
	spi_dac();
	asm volatile ("ebreak");
}
