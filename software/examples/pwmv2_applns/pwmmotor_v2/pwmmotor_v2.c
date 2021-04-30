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
	pwm_init();
	*pinmux_config_reg = 0x2aa80;
	pwm_set_prescalar_value(PWM_0, 1000);
	pwm_stop(PWM_0);
	pwm_configure(PWM_0, 0xf0, 0x80, no_interrupt, 0x1, false);
	pwm_start(PWM_0);
	pwm_set_control(PWM_0, (PWM_ENABLE | PWM_UPDATE_ENABLE |PWM_OUTPUT_ENABLE | PWM_RISE_INTERRUPT_ENABLE | PWM_OUTPUT_POLARITY ));
	pwm_show_values(PWM_0);

}