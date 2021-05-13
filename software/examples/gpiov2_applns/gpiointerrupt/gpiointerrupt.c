#include "gpiov2.h" 
#include "plic_driver.h"
#include "platform.h"



unsigned handle_button_press (unsigned num)
{
	log_info("GPIO %x interrupt occured\n", (num - PLIC_GPIO_OFFSET - 1) );

	/* When interrupt is triggered in the any pin GPIO_0 will toggle */

	gpiov2_instance->direction = 0x0001;
	gpiov2_instance->toggle = 1;
	gpiov2_instance->toggle = 0;

	return 0;
}


void main()
{
    unsigned int int_id = 7;
    register uint32_t retval;
    int i;

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

#if 0
/* Check the GPIO interrupt switch from Active high to Active low */

	/* Checking for active low */
	printf("\nIn active low");
	gpiov2_instance->intr_config = 0xff;
	configure_interrupt(PLIC_INTERRUPT_9);
	isr_table[PLIC_INTERRUPT_9] = handle_button_press;

	delay_loop(10000, 10000);
	printf("\n================================================================");
	// delay_loop(10000, 10000);


	/* Checking for active high */
	printf("\nIn active high");
	gpiov2_instance->intr_config = 0x00;
	configure_interrupt(PLIC_INTERRUPT_8);
	isr_table[PLIC_INTERRUPT_8] = handle_button_press;	
#endif	

#if 0
/* Check the GPIO interrupt switch from Active high to Active low */

	/* Checking for active high */
	gpiov2_instance->intr_config = 0x00;
	configure_interrupt(PLIC_INTERRUPT_8);
	isr_table[PLIC_INTERRUPT_8] = handle_button_press;

	delay_loop(1000, 1000);

	/* Checking for active low */
	gpiov2_instance->intr_config = 0xff;
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