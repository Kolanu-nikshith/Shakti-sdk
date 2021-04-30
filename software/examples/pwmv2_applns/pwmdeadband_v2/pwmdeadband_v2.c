#include "pwmv2.h"
#define PWM_0 0

/** @fn main()
 * @brief main function that runs the code
 * @details runs the code
 * @warning No warning
 * @param[in] No input parameter 
 * @param[Out] No output parameter
 */
int main()
{
	/**we need to set the period, duty cycle and the clock divisor in order
	 *to set it to the frequency required. Base clock is 50MHz
	 **/
	register uint32_t retval;
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

	log_info(" retval = %x\n", retval);
	asm volatile("csrr %[retval], mie\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	log_info(" retval = %x\n", retval);

	asm volatile("csrr %[retval], mip\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	log_info(" retval = %x\n", retval);

    check_pwmv2();
	pwm_init();
	pwm_configure(PWM_0,0xff,0xff,rise_interrupt,0xff,false);
    pwm_configure(PWM_0,0xff,0xff,rise_interrupt,0x0,false);
	//put a timer function in the isr and check for delay between interrupt 1 and interrupt 2 since they are configured using same settings

	while(1);

	return 0;
}
