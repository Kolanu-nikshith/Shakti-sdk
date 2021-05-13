/* LED WILL GLOW ONLY ONCE */
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

	//Uncomment this for testing PWM 0
	pwm_set_prescalar_value(PWM_0, 50000);
	pwm_clear(PWM_0);
	pwm_stop(PWM_0);
	pwm_configure(PWM_0, 0xf0, 0x80, no_interrupt, 0x1, false);
	pwm_start(PWM_0);
	pwm_set_control(PWM_0, (PWM_ENABLE |PWM_OUTPUT_ENABLE | PWM_OUTPUT_POLARITY ));
	pwm_show_values(PWM_0);
	pwm_show_frequency(PWM_0, 50000, 0xf0);
    return 0;
}