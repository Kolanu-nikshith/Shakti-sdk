/***************************************************************************
* Project               	        	:  shakti devt board
* Name of the file	            	    :  pwmled.c
* Brief Description of file             :  example file for pwm led.
* Name of Author    	                :  Abhinav Ramnath
* Email ID                              :  abhinavramnath13@gmail.com

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
/**
@file pwmled.c
@brief example file to make the led blink at specified frequency
@detail it runs the led and causes it to blink at specified frequency using the pwm module
*/

#include "pwmv2.h"
#include "pinmux.h"


#define PWM_0 0

/** @fn int main()
 * @brief main function that runs the code
 * @return zero
 */
int main()
{
	check_pwmv2();
	pwm_init();
	pwm_stop(PWM_0);
	// pwm_configure(int module_number, uint32_t period, uint32_t duty, pwm_interrupt_modes interrupt_mode , uint32_t deadband_delay, bool change_output_polarity); 
	pwm_configure(PWM_0, 0xf0, 0x80, no_interrupt, 0x1, false);
	*pinmux_config_reg = 0x80;
	pwm_start(PWM_0);
	pwm_show_values(PWM_0);
	return 0;
}
