/***************************************************************************
 * Project           			: shakti devt board
 * Name of the file	     		: 
 * Brief Description of file            :  
 * Name of Author    	                :  
 * Email ID                             :  

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
/**
@file 
@brief 
@detail 
*/

#include "gpio.h"
#include "pinmux.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "uart.h"

#ifdef USE_INTERRUPT
unsigned int uart0_prev_rcv_isr_count_flag = 0;
unsigned int uart1_prev_rcv_isr_count_flag = 0;
unsigned int uart2_prev_rcv_isr_count_flag = 0;

unsigned int uart0_prev_tx_isr_count_flag = 0;
unsigned int uart1_prev_tx_isr_count_flag = 0;
unsigned int uart2_prev_tx_isr_count_flag = 0;

/** @fn void init_variables()
 * @brief 
 * @details 
 * @warning
 */
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

#endif

/** @fn int main(void)
 * @brief 
 * @details 
 * @warning 
 * @return 
 */
int main(void){
#ifdef USE_INTERRUPT

	unsigned int int_id = 1;
	register uint32_t retval;
	int i;
	log_debug("\tUart 1 Interrupt Test\n");

	init_variables();
	
	pinmux_config_reg = PINMUX_CONFIGURE_REG;
	*(pinmux_config_reg) = 0x55;
	set_baud_rate(uart_instance[0], 115200);
	set_baud_rate(uart_instance[1], 115200);

	plic_init();

	configure_interrupt(PLIC_INTERRUPT_26);
	isr_table[PLIC_INTERRUPT_26] = uart1_isr;

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

	uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
	uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;


	enable_uart_interrupts(uart_instance[1],  ENABLE_RX_NOT_EMPTY | ENABLE_RX_FULL);
	printf("Before Tx\n");
	printf("\n Status Register: %x", uart_instance[1]->status);
	i = 0;
	write_uart_string(uart_instance[1], "Uart 1 Interrupt TeSt");
	while(1)
	{
#if 0
		if(i % 2)
			write_uart_string(uart_instance[1], "TeSt");
		else
			write_uart_string(uart_instance[1], "tEsT");
#endif			
		delay_loop(2000,1000);
		if(uart1_prev_tx_isr_count_flag != uart1_tx_isr_count)
		{
			uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
		}
		if(uart1_prev_rcv_isr_count_flag != uart1_rcv_isr_count)
		{
			uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
		}
		if(i++ >= 10)
		{
			u1_rcv_char[++uart1_rcv_isr_count] = '\0';
//			printf("\n TxISR Count Flag: %d", uart1_tx_isr_count);
			printf("\n Rcv ISR Count Flag: %d", uart1_rcv_isr_count);
//			printf("\n recv. Chars: %s", u1_rcv_char);
			i = 0;
			uart1_rcv_isr_count = 0;
			memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
//			break;
		}
	};

#endif
	return 0;
}
