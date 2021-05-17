
#include "pwmv2.h"
#include "pinmux.h"
#include <stdio.h>

#define PWM_0 0

/** @fn int main()
 * @brief main function that runs the code
 * @return zero
 */
int main()
{
	check_pwmv2();
	*pinmux_config_reg = 0x2aa80;
	pwm_init();

	pwm_set_prescalar_value(PWM_0, 50000);
	pwm_clear(PWM_0);
	pwm_stop(PWM_0);
	pwm_configure(PWM_0, 0xfff, 0xff, no_interrupt, 0x1, false);
	pwm_start(PWM_0);
    //Check by enable and disable polarity

	log_info("\n=========== With PWM_OUTPUT_POLARITY ===========");
	pwm_set_control(PWM_0, (PWM_ENABLE | PWM_UPDATE_ENABLE |PWM_OUTPUT_ENABLE | PWM_OUTPUT_POLARITY )); 
	pwm_show_values(PWM_0);

	delay_loop(10000,10000);

	log_info("\n\n=========== Without PWM_OUTPUT_POLARITY ===========");
	pwm_clr_control(PWM_0, PWM_OUTPUT_POLARITY); 
	pwm_show_values(PWM_0);

	delay_loop(10000,10000);
	pwm_clear(PWM_0);
}