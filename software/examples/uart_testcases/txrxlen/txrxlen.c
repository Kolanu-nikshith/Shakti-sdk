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
/*Configure UART1 (cutecom) and UART0 (miniterm) with same baudrate, no of bits & Stop bits to be transmitted and parity.
Continuously send some char from cutecom (UART1) to UART0
UART0 is expecting 2 Stop bits where as UART1 is Sending data with 1 stop bit.
We will get an framing error bit set.*/


/**
@file
@brief
@detail
*/
#include "uart.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include "log.h"


/** @fn
 * @brief
 *
 * @details
 *
 *
 * @warning
 *
 * @param[in]
 * @param[Out]
 */


int main(void) {

	unsigned int int_id = 1;
	register uint32_t retval;
	uint16_t value;
	char readString[30];
	char a;
	int i;
  uint16_t control;
  uart_init();
  uart_struct *uart1 = uart_instance[1];
  set_baud_rate(uart1,19200);
  value = uart_instance[1]->control;
  value = 0;
	printf("UART Char test\n");
	//printf("\n UART char size set to 0");
  //control = 0;

  //printf("\n UART char size set to 1");
  //control = value | 1 << 5;

  //printf("\n UART char size set to 4");
  //control = value | 4 << 5;
  //printf("\n UART char size set to 7");
  //control = value | 7 << 5;
  //printf("\n UART char size set to 15");
  //control = value | 15 << 5;
  //printf("\n UART char size set to 22");
  //control = value | 22 << 5;
  //printf("\n UART char size set to 32");
  //control = value | 32 << 5;

  printf("\n UART char size set to 22");
  control = value | 8 << 5;
  set_uart_control(uart1,control);
  printf("\n UART control is set to %d",control);
  write_uart_string(uart1,"Hello World abc");
  //read_uart_string(uart1,readString);
  //printf("\n read from uart1 %s",readString);
  //write_uart_character(uart1,"W");
  //read_uart_character(uart1,&a);
  //printf("\n read from uart1 %c",a);

}
