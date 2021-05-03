#include "gpiov2.h" 
#include "plic_driver.h"
#include "platform.h"



unsigned handle_button_press (unsigned num)
{
	log_info("GPIO %x (active low) interrupt occured\n", (num - PLIC_GPIO_OFFSET) );
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
	gpiov2_instance->intr_config = 0xff; //Make interrupts active low.
    plic_init();

    // configure_interrupt(PLIC_INTERRUPT_7);
	configure_interrupt(PLIC_INTERRUPT_8);
	// configure_interrupt(PLIC_INTERRUPT_9);
	// configure_interrupt(PLIC_INTERRUPT_10);
	// configure_interrupt(PLIC_INTERRUPT_11);
	// configure_interrupt(PLIC_INTERRUPT_12);
	// configure_interrupt(PLIC_INTERRUPT_13);
	// configure_interrupt(PLIC_INTERRUPT_14);
	// configure_interrupt(PLIC_INTERRUPT_15);

	// isr_table[PLIC_INTERRUPT_7] = handle_button_press;
	isr_table[PLIC_INTERRUPT_8] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_9] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_10] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_11] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_12] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_13] = handle_button_press;
	// isr_table[PLIC_INTERRUPT_14] = handle_button_press; 
	// isr_table[PLIC_INTERRUPT_15] = handle_button_press; 


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

	while(1){
		asm volatile(
			     "csrr %[retval], mip\n"
			     :
			     [retval]
			     "=r"
			     (retval)
			    );

		i++;

		if((i%10000000) == 0){
			printf(" retval = %x\n", retval);
		}

	}
}