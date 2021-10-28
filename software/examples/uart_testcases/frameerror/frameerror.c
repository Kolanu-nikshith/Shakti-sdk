/***************************************************************************
* Project           		:  shakti devt board
* Name of the file	     	:  hello.c
* Brief Description of file     :  Does the printing of hello with the help of uart communication protocol.
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
/*
UART0: 19200, No parity; (miniterm)
UART1: 19200, stop bit=2 No parity; (cutecom)
Try sending chars from UART1 to UART0

UART 0 is expecting 2 stop bits whereas UART1 sends char with 1 stop bit, so frame error occurs*/



#include<uart.h>
#include "pinmux.h"

void main()
{
	int value = 0;
	*(pinmux_config_reg) =  0x55;
	set_baud_rate(uart_instance[0], 19200);
    	set_baud_rate(uart_instance[1], 19200);
	value = uart_instance[1]->control;
	uart_instance[1]->control = value | STOP_BITS(2);// configured stop bits as 2
	write_uart_string (uart_instance[0], "Hello World in uart 0 !\n");
	write_uart_string (uart_instance[1], "Hello World in uart1!\n");

	while(1)
	{
		while( 0 == (uart_instance[1]->status & STS_RX_NOT_EMPTY ) );
		if(0 != (uart_instance[1]->status & FRAME_ERROR) )
			write_uart_string (uart_instance[0], "\n Frame error");
		else
		{
			write_uart_string (uart_instance[0], "\n The received char: ");
			write_uart_character(uart_instance[0], uart_instance[1]->rcv_reg);
		}
	}
}
