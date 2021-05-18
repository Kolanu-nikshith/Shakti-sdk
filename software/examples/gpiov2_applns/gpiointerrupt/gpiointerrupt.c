/***************************************************************************
 * Project           	      : shakti devt board
 * Name of the file	      : gpio_interrupt.c
 * Brief Description of file  : A application to demonstrate the interrupt based on GPIO
 * Name of Author             : Soutrick Roy Chowdhury
 * Email ID                   : soutrickofficial@gmail.com

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

*****************************************************************************/

#include "gpiov2.h" 
#include "plic_driver.h"
#include "platform.h"

#define ACTIVE_HIGH_to_LOW 0
#define ACTIVE_LOW_to_HIGH 0

unsigned handle_button_press (unsigned num)
{

#if ACTIVE_LOW_to_HIGH
	log_info("\nActive low interrupt");
#endif	

#if ACTIVE_HIGH_to_LOW
	log_info("\nActive high interrupt");
#endif

	log_info("GPIO %x interrupt occured\n", (num - PLIC_GPIO_OFFSET - 1) );

#if ACTIVE_LOW_to_HIGH
	/* Check the GPIO interrupt switch from Active low to Active high */
				/* TEST DONE on GPIO_2 */
	gpiov2_instance->direction = 0x0;
	if (gpiov2_instance->data == 4)
	{
		log_info("\nActive high interrupt");
		delay_loop(1000,1000);
		gpiov2_instance->intr_config = 0x00;
		configure_interrupt(PLIC_INTERRUPT_9);
		isr_table[PLIC_INTERRUPT_9] = handle_button_press;	
	}
#endif	

#if ACTIVE_HIGH_to_LOW
	/* Check the GPIO interrupt switch from Active low to Active high */
				/* TEST DONE on GPIO_2 */
	gpiov2_instance->direction = 0x0;
	if (gpiov2_instance->data == 0)
	{
		log_info("\nActive low interrupt");
		delay_loop(1000,1000);
		gpiov2_instance->intr_config = 0xff;
		configure_interrupt(PLIC_INTERRUPT_9);
		isr_table[PLIC_INTERRUPT_9] = handle_button_press;	
	}
#endif	

#if 0
	/* When interrupt is triggered in the any pin GPIO_0 will toggle */
	// gpiov2_instance->direction = 0x0001;
	// gpiov2_instance->toggle = 1;
	// gpiov2_instance->toggle = 0;
#endif

	return 0;
}


void main()
{
    unsigned int int_id = 7;
    register uint32_t retval;
    int i;

    gpiov2_instance->direction = 0xf0000;
    check_gpiov2();
    printf("\n Starting GPIO INTERRUPT tests \n");
    printf("\n GPIO_INTERRUPT_OFFSET: %x; PLIC_MAX_INTERRUPT_SRC: %x; Intr.Src: %x",PLIC_GPIO_OFFSET, PLIC_MAX_INTERRUPT_SRC, int_id);

	gpiov2_init();
    gpiov2_instance->direction = 0x00;

	/* Checking for active low */
	// gpiov2_instance->intr_config = 0xff;

	/* Chexking for active high */
	// gpiov2_instance->intr_config = 0x00;

    plic_init();

/* GPIO_0 */
    // configure_interrupt(PLIC_INTERRUPT_7);
	// isr_table[PLIC_INTERRUPT_7] = handle_button_press;

/* GPIO_1 */
	// configure_interrupt(PLIC_INTERRUPT_8);
	// isr_table[PLIC_INTERRUPT_8] = handle_button_press;

/* GPIO_2 */
	// configure_interrupt(PLIC_INTERRUPT_9);
	// isr_table[PLIC_INTERRUPT_9] = handle_button_press;
	
/* GPIO_3 */	
	// configure_interrupt(PLIC_INTERRUPT_10);
	// isr_table[PLIC_INTERRUPT_10] = handle_button_press;
	
/* GPIO_4 */	
	// configure_interrupt(PLIC_INTERRUPT_11);
	// isr_table[PLIC_INTERRUPT_11] = handle_button_press;

/* GPIO_5 */	
	// configure_interrupt(PLIC_INTERRUPT_12);
	// isr_table[PLIC_INTERRUPT_12] = handle_button_press;

/* GPIO_6 */	
	// configure_interrupt(PLIC_INTERRUPT_13);
	// isr_table[PLIC_INTERRUPT_13] = handle_button_press;

/* GPIO_7 */	
	// configure_interrupt(PLIC_INTERRUPT_14);
	// isr_table[PLIC_INTERRUPT_14] = handle_button_press; 

/* GPIO_8 */	
	// configure_interrupt(PLIC_INTERRUPT_15);
	// isr_table[PLIC_INTERRUPT_15] = handle_button_press; 

/* GPIO_9 */
	// configure_interrupt(PLIC_INTERRUPT_16);
	// isr_table[PLIC_INTERRUPT_16] = handle_button_press; 

/* GPIO_10 */
	// configure_interrupt(PLIC_INTERRUPT_17);
	// isr_table[PLIC_INTERRUPT_17] = handle_button_press; 

/* GPIO_11 */
	// configure_interrupt(PLIC_INTERRUPT_18);
	// isr_table[PLIC_INTERRUPT_18] = handle_button_press; 

/* GPIO_12 */
	// configure_interrupt(PLIC_INTERRUPT_19);
	// isr_table[PLIC_INTERRUPT_19] = handle_button_press; 

/* GPIO_13 */
	// configure_interrupt(PLIC_INTERRUPT_20);
	// isr_table[PLIC_INTERRUPT_20] = handle_button_press; 

/* GPIO_14 */
	// configure_interrupt(PLIC_INTERRUPT_21);
	// isr_table[PLIC_INTERRUPT_21] = handle_button_press; 

/* GPIO_15 */
	// configure_interrupt(PLIC_INTERRUPT_22);
	// isr_table[PLIC_INTERRUPT_22] = handle_button_press; 		

#if ACTIVE_LOW_to_HIGH
/* Check the GPIO interrupt switch from Active low to Active high */

	/* Checking for active low */
	gpiov2_instance->intr_config = 0xff;
	configure_interrupt(PLIC_INTERRUPT_9);
	isr_table[PLIC_INTERRUPT_9] = handle_button_press;

#endif	

#if ACTIVE_HIGH_to_LOW
/* Check the GPIO interrupt switch from Active high to Active low */

	/* Checking for active high */
	gpiov2_instance->intr_config = 0x00;
	configure_interrupt(PLIC_INTERRUPT_9);
	isr_table[PLIC_INTERRUPT_9] = handle_button_press;

#endif

	// Enable Global (PLIC) interrupts.
	asm volatile("li      t0, 8\t\n"
		     "csrrs   zero, mstatus, t0\t\n"
		    );

	// Enable Local (PLIC) interrupts.
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

	log_debug("mstatus = %x\n", retval);

	asm volatile(
		     "csrr %[retval], mie\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	log_debug("mie = %x\n", retval);

	asm volatile(
		     "csrr %[retval], mip\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	log_debug("mip = %u\n", retval);

	while(1){
		i++;

		if((i%10000000) == 0){

			asm volatile(
				     "csrr %[retval], mip\n"
				     :
				     [retval]
				     "=r"
				     (retval)
				    );
			log_debug("mip = %u\n", retval);
		}
	}
}
