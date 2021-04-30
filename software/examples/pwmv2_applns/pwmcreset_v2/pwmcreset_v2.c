#include "pwmv2.h"
#define PWM_0 0

extern pwm_struct *pwm_instance[];


void main()
{
	int dead_band_delay = 1;
	int i = 0;
	/**we need to set the period, duty cycle and the clock divisor in order
	 *to set it to the frequency required. Base clock is 50MHz
	 **/
	pwm_init();
	//pwm_configure(int module_number, uint32_t period, uint32_t duty, pwm_interrupt_modes interrupt_mode, uint32_t deadband_delay, bool change_output_polarity, bool enable_complementary_output)
	pwm_set_prescalar_value(0, 1000);
	pwm_stop(PWM_0);
	// pwm_configure(PWM_0, 20833, ( (i +1) * 2025), no_interrupt, 0x1, false, false);
	pwm_configure(PWM_0, 0xf0, 0x80, no_interrupt, 0x1, false);
	pwm_start(PWM_0);
	pwm_set_control(PWM_0, (PWM_ENABLE | PWM_UPDATE_ENABLE | PWM_OUTPUT_ENABLE | PWM_RISE_INTERRUPT_ENABLE | PWM_OUTPUT_POLARITY ));
	pwm_show_values(PWM_0);

	
	while(1)
	{
        int i = 0;
		pwm_instance[i]->control |= PWM_COUNTER_RESET;
		delay_loop(100, 100);
	}
		
}
