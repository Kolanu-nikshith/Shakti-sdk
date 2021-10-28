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
UART0: 19200, stop bit=1 No parity; (miniterm)
UART1: 19200, Stop bit1, even parity (miniterm)
Try sending chars from UART1 to UART0
*/




#include "uart.h"
#include "pinmux.h"

void main()
{
    int value = 0;
      *(pinmux_config_reg) =  0x55;

    set_baud_rate(uart_instance[0], 19200);
    set_baud_rate(uart_instance[1], 19200);
    value = uart_instance[1]->control;
    uart_instance[1]->control = value | PARITY(2);
    write_uart_string (uart_instance[1],"\n Hello world UART1");
    write_uart_string (uart_instance[0],"\n Hello world UART0");
    while(1)
    {
        while( 0 == (uart_instance[1]->status & STS_RX_NOT_EMPTY ) );
        if(uart_instance[1]->status & PARITY_ERROR)
	{
        write_uart_string (uart_instance[0], "\n Even Parity error with UART 0");
	break;
	}
        else
        {
            write_uart_string (uart_instance[0], "\n The received char: ");
            write_uart_character(uart_instance[0], uart_instance[1]->rcv_reg);
        }
    }
}
