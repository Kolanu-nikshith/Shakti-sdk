/***************************************************************************
* Project           		:  shakti devt board
* Name of the file	     	:  baudrate.c
* Brief Description of file     :  Sends chars between UART 0 and 1 in different baudrates
* Name of Author    	                :  Akshaya B
* Email ID                              :  akshayabarat@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

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
/* Procedure
Set up Stop bits -1, No parity for both UARTs
UART0 Miniterm - change the baud rate as required
UART1 Miniterm baud rate is set as 19200 
*/

#include "uart.h"
#include "pinmux.h"

void main()
{
    *(pinmux_config_reg) =  0x55;

    int t;
	set_baud_rate(uart_instance[0], 19200); // Set the required baudrate from 300 to 540,000
	//set_baud_rate(uart_instance[1], 500000);
  write_uart_string (uart_instance[0], "\nHello World from UART0");
  write_uart_string (uart_instance[1], "\nHello World from UART1");
  while(1)
    {
        while( 0 == (uart_instance[1]->status & STS_RX_NOT_EMPTY ) );
        write_uart_string (uart_instance[0], "\n The received char: ");
        write_uart_character(uart_instance[0], uart_instance[1]->rcv_reg);
    }
}
