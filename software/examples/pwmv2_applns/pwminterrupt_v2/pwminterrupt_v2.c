
#include "pwmv2.h"
#include "plic_driver.h"
#define PWM_0 0

void handle_pwm_interrupt( uint32_t num)
{
	// if(pwm_check_continuous_mode((6-num)) == 0)
	// {
	// 	pwm_set_control((6-num),0x80);
	// }
    pwm_set_control((6-num),0x80);
	log_info("pwm interrupt handled\n");
}

int main()
{

	printf("\n====================================================\n");

    register unsigned int retval;
	int i;

	plic_init();

	/*
	   Configure pwm module interrupt
	*/
	configure_interrupt(PLIC_INTERRUPT_6);
	// isr_table[PLIC_INTERRUPT_6] = handle_pwm_interrupt;
	isr_table[PLIC_INTERRUPT_6] = pwm_isr_handler0;

	/**we need to set the period, duty cycle and the clock divisor in order
	 *to set it to the frequency required. Base clock is 50MHz
	 **/

	asm volatile("li      t0, 8\t\n"
			"csrrs   zero, mstatus, t0\t\n"
		    );

	asm volatile("li      t0, 0x800\t\n"
			"csrrs   zero, mie, t0\t\n"
		    );

	asm volatile("csrr %[retval], mstatus\n"
			:
			[retval]
			"=r"
			(retval)
		    );

	log_info("mstatus = %x\n", retval);
	asm volatile("csrr %[retval], mie\n"
			:
			[retval]
			"=r"
			(retval)
		    );

	log_info("mie = %x\n", retval);

	asm volatile("csrr %[retval], mip\n"
			:
			[retval]
			"=r"
			(retval)
		    );

	log_info("mip = %x\n", retval);


    check_pwmv2();
	/* Enable when need rise interrupt */
#if 1	
	pwm_init();
    pwm_set_prescalar_value(PWM_0, 0xf2);
 	pwm_configure(PWM_0, 0xff, 0x01, rise_interrupt, 0x1, false);
    pwm_start(PWM_0);
    pwm_set_control(PWM_0, (PWM_ENABLE | PWM_UPDATE_ENABLE |PWM_OUTPUT_ENABLE | PWM_RISE_INTERRUPT_ENABLE | PWM_OUTPUT_POLARITY ));
	pwm_show_values(PWM_0);
#endif

	/* Enable when fall interrupt */
#if 0
	pwm_init();
    pwm_set_prescalar_value(PWM_0, 0xf2);
 	pwm_configure(PWM_0, 0xff, 0x01, fall_interrupt, 0x1, false);
    pwm_start(PWM_0);
    pwm_set_control(PWM_0, (PWM_ENABLE | PWM_UPDATE_ENABLE |PWM_OUTPUT_ENABLE | PWM_FALL_INTERRUPT_ENABLE | PWM_OUTPUT_POLARITY ));
	pwm_show_values(PWM_0);
#endif	
    return 0;
}