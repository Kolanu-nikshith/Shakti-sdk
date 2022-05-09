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


#include <stdint.h>
#include <stddef.h>
#include "log.h"
#include "uart.h"
#include "pinmux.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include <stdio.h>
#include <string.h>


#define USE_INTERRUPT
#define MAX_OUTPUT_SIZE 1024


unsigned int uart0_prev_rcv_isr_count_flag = 0;
unsigned int uart1_prev_rcv_isr_count_flag = 0;
unsigned int uart2_prev_rcv_isr_count_flag = 0;

unsigned int uart0_prev_tx_isr_count_flag = 0;
unsigned int uart1_prev_tx_isr_count_flag = 0;
unsigned int uart2_prev_tx_isr_count_flag = 0;


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


int enterATMode(uart_struct * uart_instance)
{
   delay(20);
    write_uart_character(uart_instance,'+');
    write_uart_character(uart_instance,'+');
    write_uart_character(uart_instance,'+');
   delay(20);

    u1_rcv_char[++uart1_rcv_isr_count] = '\0';

    if(strstr(u1_rcv_char,"OK")==NULL)
    {
        log_info("Entered AT Command Mode");
        return 1;
    }
    else {
        log_info("Failed to enter AT Mode");
        return 0;
    }
    uart1_rcv_isr_count = 0;
    memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);

}

int write_cmd(uart_struct * uart_instance, char *str, int length)
{
    delay(3);
    char *s1 = str;

    for(int i=0;i<length;i++){
        write_uart_character(uart_instance, *str);
        str++;
    }
    write_uart_character(uart_instance,'\r');

    delay(2);
    u1_rcv_char[++uart1_rcv_isr_count] = '\0';

    if(strstr(u1_rcv_char,"ERROR")==NULL)
    {
        log_info("%s command failed", s1);
    }
    else {
        log_info("%s command success", s1);
    }
    uart1_rcv_isr_count = 0;
    memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
}

void configure_coordinator(uart_struct * uart_instance){
    int attempts_flag=0;
   printf("Enterting AT Command Mode");
   for(int i=0;i<5;i++){
       if(enterATMode(uart_instance)==1){
           attempts_flag=1;
           break;
       };
   } 
   if(attempts_flag==0){
       printf("Could not enter AT Mode. Exiting Now.");
        exit(-1);
   }

   //Networking commands
   write_cmd(uart_instance, "ATID0", 5);
   write_cmd(uart_instance, "ATSC7FFF", 8);
   write_cmd(uart_instance, "ATSD3", 5);
   write_cmd(uart_instance, "ATZS0", 5);
   write_cmd(uart_instance, "ATNJFF",6);
   write_cmd(uart_instance, "ATNW0", 5);
   write_cmd(uart_instance, "ATJV0", 5);
   write_cmd(uart_instance, "ATJN1", 5);
   write_cmd(uart_instance, "ATCE1", 5);
   write_cmd(uart_instance, "ATDO8", 5);
   write_cmd(uart_instance, "ATDC0", 5);

   //Addressing commands
   write_cmd(uart_instance, "ATDH0", 5);
   write_cmd(uart_instance, "ATDLFFFF", 8);
   write_cmd(uart_instance, "ATNH1E", 6);
   write_cmd(uart_instance, "ATBH0", 5);
   write_cmd(uart_instance, "ATARFF", 6);
   write_cmd(uart_instance, "ATDDA0000", 9);
   write_cmd(uart_instance, "ATNT3C", 6);
   write_cmd(uart_instance, "ATNO0", 5);
   write_cmd(uart_instance, "ATCR3", 5);

   //Zigbee Addressing
   write_cmd(uart_instance, "ATSEE8", 6);
   write_cmd(uart_instance, "ATDEE8", 6);
   write_cmd(uart_instance, "ATCI11", 6);
   write_cmd(uart_instance, "ATTO0", 5);

   //RF Interfacing
   write_cmd(uart_instance, "ATPL4", 5);

   //Security
   write_cmd(uart_instance, "ATEE0", 5);
   write_cmd(uart_instance, "ATEO0", 5);
   write_cmd(uart_instance, "ATKY0", 5);
   write_cmd(uart_instance, "ATNK0", 5);
   
   
   //Serial Interfacing
   write_cmd(uart_instance, "ATBD3", 5);
   write_cmd(uart_instance, "ATNB0", 5);
   write_cmd(uart_instance, "ATSB0", 5);
   write_cmd(uart_instance, "ATRO3", 5);
   write_cmd(uart_instance, "ATD60", 5);
   write_cmd(uart_instance, "ATD71", 5);
   write_cmd(uart_instance, "ATAP1", 5);
   write_cmd(uart_instance, "ATAO0", 5);

   //AT Command Options
   write_cmd(uart_instance, "ATCT64", 6);
   write_cmd(uart_instance, "ATGT3E8",7);
   write_cmd(uart_instance, "ATCC2B", 6);
  
   //Sleep Modes
   write_cmd(uart_instance, "ATSP20",6 );
   write_cmd(uart_instance, "ATSN1", 5);
   write_cmd(uart_instance, "ATSM0", 5);
   write_cmd(uart_instance, "ATST1388", 8);
   write_cmd(uart_instance, "ATSO0", 5);
   write_cmd(uart_instance, "ATWH0", 5);
   write_cmd(uart_instance, "ATPO0", 5);

   //I/O Settings
   write_cmd(uart_instance, "ATD01", 5);
   write_cmd(uart_instance, "ATD10", 5);
   write_cmd(uart_instance, "ATD20", 5);
   write_cmd(uart_instance, "ATD30", 5);
   write_cmd(uart_instance, "ATD40", 5);
   write_cmd(uart_instance, "ATD51", 5);
   write_cmd(uart_instance, "ATD81", 5);
   write_cmd(uart_instance, "ATD91", 5);
   write_cmd(uart_instance, "ATP01", 5);
   write_cmd(uart_instance, "ATP10", 5);
   write_cmd(uart_instance, "ATP20", 5);
   write_cmd(uart_instance, "ATP31", 5);
   write_cmd(uart_instance, "ATP41", 5);
   write_cmd(uart_instance, "ATPR1FBF", 8);
   write_cmd(uart_instance, "ATPD1FFF", 8);
   write_cmd(uart_instance, "ATLT0",5 );
   write_cmd(uart_instance, "ATRP28",6 );

   //I/O Sampling
   write_cmd(uart_instance, "ATIR0", 5);
   write_cmd(uart_instance, "ATIC0", 5);
   write_cmd(uart_instance, "ATV+0", 5);
}

void writeAll(uart_struct * uart_instance){
    delay(3);
    write_uart_character(uart_instance,'A');
    write_uart_character(uart_instance,'T');
    write_uart_character(uart_instance,'W');
    write_uart_character(uart_instance,'R');
    write_uart_character(uart_instance,'\r');
    delay(2);

    u1_rcv_char[++uart1_rcv_isr_count] = '\0';
    if(strstr(u1_rcv_char,"ERROR")==NULL)
    {
        log_info("Flashing failed. The above configuration stays until next reboot!\n");
    }
    else {
        log_info("Flashing success. The Xbee is configued as a Coordinator.\n PANID: 1234\n ");
    }
    uart1_rcv_isr_count = 0;
    memset(u1_rcv_char,'\0', UARTX_BUFFER_SIZE);
}

int main(void){
    printf("***Running Program to Flash XBEE as Coordinator***\n");
	unsigned int int_id = 1;
	register uint32_t retval;
	int i;
//	printf("\tUart 1 - Uart 2 Interrupt Test\n");

	init_variables();

	pinmux_enable_uart1_uart2();
	set_baud_rate(uart_instance[0], 115200);
	set_baud_rate(uart_instance[1], 9600);
	
	plic_init();

	configure_interrupt(PLIC_INTERRUPT_26);
	isr_table[PLIC_INTERRUPT_26] = uart1_isr;
	
	uart1_prev_tx_isr_count_flag = uart1_tx_isr_count;
	uart1_prev_rcv_isr_count_flag = uart1_rcv_isr_count;
	free_uart_buffers();
	printf("Before Tx00\n");
	printf("\n UART 1 Status Register: %x", uart_instance[1]->status);
	enable_uart_interrupts(uart_instance[1],  ENABLE_RX_NOT_EMPTY );//| ENABLE_RX_FULL);
	
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
	
   

	return 0;
}





