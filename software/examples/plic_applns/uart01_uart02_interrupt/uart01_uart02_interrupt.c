/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  uart1_uart2_interrupt.c
 * Created date			        :  31.03.2022
 * Brief Description of file             :  Transmits data from UART1 - UART2 & vice versa.
 * Name of Author    	                :  E. Kotteeswaran
 * Email ID                              :  kottee.1@gmail.com

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
//#include "gpiov2.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "uart.h"
#include "pinmux.h"

unsigned int uart0_prev_rcv_isr_count_flag = 0;
unsigned int uart1_prev_rcv_isr_count_flag = 0;
unsigned int uart2_prev_rcv_isr_count_flag = 0;

unsigned int uart0_prev_tx_isr_count_flag = 0;
unsigned int uart1_prev_tx_isr_count_flag = 0;
unsigned int uart2_prev_tx_isr_count_flag = 0;
#if 1
	uint8_t uart1_buf[] = {"abcdefghijklmnopqrstuvwxyz"};
	uint8_t uart2_buf[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
#else
	uint8_t uart1_buf[] = {"abcdef"};
	uint8_t uart2_buf[] = {"ABCDEF"};
#endif

void init_variables()
{
	uart0_complete = 0;
	uart1_complete = 0;
	uart2_complete = 0;
	uart0_tx_isr_count = 0;
	uart0_rcv_isr_count = 0;
	uart1_tx_isr_count = 0;
	uart1_rcv_isr_count = 0;
	uart2_tx_isr_count = 0;
	uart2_rcv_isr_count = 0;
	memset(u0_rcv_char,'\0', UARTX_BUFFER_SIZE);
	memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
	memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
	uart0_prev_rcv_isr_count_flag = 0;
	uart1_prev_rcv_isr_count_flag = 0;
	uart2_prev_rcv_isr_count_flag = 0;

	uart0_prev_tx_isr_count_flag = 0;
	uart1_prev_tx_isr_count_flag = 0;
	uart2_prev_tx_isr_count_flag = 0;
}
extern volatile unsigned int* pinmux_config_reg;

void free_uart_buffers()
{
	flush_uart( uart_instance[0] );
	flush_uart( uart_instance[1] );
	flush_uart( uart_instance[2] );

}

int main(void){

	unsigned int int_id = 1;
	register uint32_t retval;
	int i;
	printf("\tUart 1 - Uart 2 Interrupt Test\n");

	init_variables();

	pinmux_enable_uart1_uart2();
	set_baud_rate(uart_instance[0], 115200);
	set_baud_rate(uart_instance[1], 19200);
	set_baud_rate(uart_instance[2], 19200);

	plic_init();

	configure_interrupt(PLIC_INTERRUPT_26);
	isr_table[PLIC_INTERRUPT_26] = uart1_isr;
	configure_interrupt(PLIC_INTERRUPT_27);
	isr_table[PLIC_INTERRUPT_27] = uart2_isr;

	uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
	uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
	uart2_prev_tx_isr_count_flag = uart2_tx_isr_count;
	uart2_prev_rcv_isr_count_flag = uart2_rcv_isr_count;
	free_uart_buffers();
	printf("Before Tx\n");
	printf("\n UART 1 Status Register: %x", uart_instance[1]->status);
	printf("\n UART 2 Status Register: %x", uart_instance[2]->status);
	enable_uart_interrupts(uart_instance[1],  ENABLE_RX_NOT_EMPTY | ENABLE_RX_FULL);
	enable_uart_interrupts(uart_instance[2],  ENABLE_RX_NOT_EMPTY | ENABLE_RX_FULL);

	// Enable Global (PLIC) interrupts.
	asm volatile("li      t0, 8\t\n"
		     "csrrs   zero, mstatus, t0\t\n"
		    );

	asm volatile("li      t0, 0x800\t\n"
		     "csrrs   zero, mie, t0\t\n"
		    );

	asm volatile(
		     "csrr %[retval], mstatus\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);
	asm volatile(
		     "csrr %[retval], mie\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);

	asm volatile(
		     "csrr %[retval], mip\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );
	printf(" retval = %x\n", retval);



	i = 0;
	write_uart_string(uart_instance[1], uart1_buf);
	write_uart_string(uart_instance[2], uart2_buf);
//	write_uart_character(uart_instance[1], 'a');
	while(1)
	{
		delay_loop(2000,1000);
		if(uart1_prev_tx_isr_count_flag != uart1_tx_isr_count)
		{
			uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
		}
		if(uart1_prev_rcv_isr_count_flag != uart1_rcv_isr_count)
		{
			uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
		}
		if(uart2_prev_tx_isr_count_flag != uart2_tx_isr_count)
		{
			uart2_prev_tx_isr_count_flag = uart2_tx_isr_count;
		}
		if(uart2_prev_rcv_isr_count_flag != uart2_rcv_isr_count)
		{
			uart2_prev_rcv_isr_count_flag = uart2_rcv_isr_count;
		}
		if(i++ >= 10)
		{
			if(uart1_rcv_isr_count != 0)
			{
				u1_rcv_char[++uart1_rcv_isr_count] = '\0';
				printf("\n UART 1 TxISR Count Flag: %d", uart1_tx_isr_count);
				printf("\n UART 1 Rcv ISR Count Flag: %d", uart1_rcv_isr_count);
				printf("\n UART 1 recv. Chars: %s", u1_rcv_char);
				uart1_rcv_isr_count = 0;
				memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
			}
			if(uart2_rcv_isr_count != 0)
			{
				u2_rcv_char[++uart2_rcv_isr_count] = '\0';
				printf("\n UART 2 TxISR Count Flag: %d", uart2_tx_isr_count);
				printf("\n UART 2 Rcv ISR Count Flag: %d", uart2_rcv_isr_count);
				printf("\n UART 2 recv. Chars: %s", u2_rcv_char);
				uart2_rcv_isr_count = 0;
				memset(u2_rcv_char,'\0', UARTX_BUFFER_SIZE);
				//break;
			}
				i = 0;
		}
	};


	return 0;
}
