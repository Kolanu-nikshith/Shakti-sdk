
/***************************************************************************
 * Project               	    : shakti devt board
 * Name of the file	            : pwmv2.h
 * Brief Description of file    : Header file for PWM V2 Driver.
 * Name of Author    	        : Sathya Narayanan, Kotteeswaran
 * Email ID                     : kottee.off@gmail.com

 Copyright (C) 2021  IIT Madras. All rights reserved.

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
 * @file pwmv2.c
 * @project shakti devt board
 * @brief Driver source file for PWM V2 Driver.
 */

#include <stdio.h>
#include "pwmv2.h"
#include "platform.h"
#include "log.h"



#define PERIOD_REGISTER_MAX		0xFFFFFFFF 
#define DUTY_REGISTER_MAX   	   	0xFFFFFFFF 
#define CONTROL_REGISTER_MAX        	0x0000FFFF
#define DEADBAND_DELAY_REGISTER_MAX 	0x0000FFFF

#define PWM_MAX_COUNT 6

volatile pwm_struct *pwm_instance[PWM_MAX_COUNT];


//volatile uint16_t *pwm_output_control = PWM_OUTPUT_CONTROL;
volatile int *pwm_output_control = 0x10200;

void check()
{
	printf("In PWMV2_driver");
}

/** @fn  pwm_init
 * @brief Function to initialize all pwm modules
 * @details This function will be called to initialize all pwm modules
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void pwm_init()
{
	pwm_instance[0]= PWM_BASE_ADDRESS;
	pwm_instance[1]= PWM_BASE_ADDRESS + 0x100;
	pwm_instance[2]= PWM_BASE_ADDRESS + 0x200;
	pwm_instance[3]= PWM_BASE_ADDRESS + 0x300;
	pwm_instance[4]= PWM_BASE_ADDRESS + 0x400;
	pwm_instance[5]= PWM_BASE_ADDRESS + 0x500;
}

/** @fn  pwm_set_control
 * @brief Function to set the control register of the selected pwm module
 * @details This function will be called to set the value of the control register for the selected module
 * @param[in] uint32_t (module_number- specifies the pwm smodule to be selected)
 *            uint32_t (value - value to be set between 0x0000 to 0xffff.)
 * @param[Out] uint32_t (returns 1 on success, 0 on failure.)
 */
int pwm_set_control(int module_number, int value)
{
	pwm_instance[module_number]->control=value;

	log_debug("\n Control Register of module number %d set to %x", module_number, value);

	return 1;
}

/** @fn  pwm_set_deadband_delay
 * @brief Function to set the deadband delay register of the selected pwm module
 * @details This function will be called to set the value of the deadband_delay register for the selected module
 * @param[in] uint32_t (module_number- specifies the pwm module to be selected)
 *            uint32_t (value - value to be set between 0x0000 to 0xff.)
 * @param[Out] uint32_t (returns 1 on success, 0 on failure.)
 */
int pwm_set_deadband_delay(int module_number, int value)
{
	pwm_instance[module_number]->deadband_delay=value;

	log_debug("\n DeadBand Delay Register of module number %d set to %x", module_number, value);

	return 1;
}

/** @fn pwm_clear
 * @brief Function to clear all registers in a specific pwm module
 * @details This function will be called to clear all registers in a specific pwm module
 * @param[in] uint32_t (module_number- specifies the pwm module to be selected)
 * @param[Out] No output parameter
 */
void pwm_clear(int module_number)
{
	pwm_instance[module_number]->control=0;
	pwm_instance[module_number]->duty=0;
	pwm_instance[module_number]->period=0;
	pwm_instance[module_number]->deadband_delay=0;
	log_debug("\n All registers of module number %d cleared", module_number);
}

/** @fn  configure_control_mode
 * @brief Function to set value of control register based on parameteres
 * @details This function will set value of control register based on parameters
 * @param[in]  bool          (update                      - specifies if the module is to be updated)
 *           interrupt_mode  (interrupt_mode              - it specifes the mode of the interrupt)
 *           bool            (change_output_polarity      - it specifies if the output polarity should be changed) 
 * @param[Out] uint32_t (returns value to be set in the control register.)
 */
inline int configure_control(bool update, pwm_interrupt_modes interrupt_mode, bool change_output_polarity)
{
	int value = 0x0;

	if(update==1)
	{
		value |=PWM_UPDATE_ENABLE;
	}

	if(interrupt_mode==0 || interrupt_mode==3 || interrupt_mode==5 || interrupt_mode==6)
	{
		value |= PWM_RISE_INTERRUPT_ENABLE;
	}

	if(interrupt_mode==1 || interrupt_mode==3 || interrupt_mode==4 || interrupt_mode==6)
	{
		value |= PWM_FALL_INTERRUPT_ENABLE;
	}

	if(interrupt_mode==2 || interrupt_mode==4 || interrupt_mode==5 || interrupt_mode==6)
	{
		value |= PWM_HALFPERIOD_INTERRUPT_ENABLE;
	}

	if(change_output_polarity)
	{
		value |= PWM_OUTPUT_POLARITY;
	}

	return value;
}

/** @fn  pwm_set_duty_cycle
* @brief Function to set the duty cycle value of a specific pwm module 
 * @details This function will set the duty cycles value of a specific pwm module
 * @param[in]uint32_t(module_number-  the pwm module to be selected )
 *           uint32_t(duty - value of duty cycles to be used to decide how many period cycles the pwm signal is set to 1.
 * @param[Out] No output parameter
 */
void pwm_set_duty_cycle(int module_number, int duty)
{
	pwm_instance[module_number]->duty=duty;

	log_debug("\n Duty Register of module number %d set to %x", module_number, duty);

	return 1;
}

/** @fn pwm_set_periodic_cycle
 * @brief Function to set the period cycles value of a specific pwm module
 * @details This function will set the period cycles value of a specific pwm module
 * @param[in] uint32_t (module_number-  the pwm module to be selected)
 *            uint32_t (clock_divisor-  value of period cycles which is used to further divide the frequency into fixed period cycles.)
 * @param[Out] No output parameter
 */
void pwm_set_periodic_cycle(int module_number, int period)
{
	pwm_instance[module_number]->period=period;

	log_debug("\n Period Register of module number %d set to %x", module_number, period);

	return 1;
}

/** @fn pwm_set_prescalar_value
 * @brief Function to set the prescalar value of a specific pwm cluster		
 * @details This function will set the prescalar value of a specific pwm cluster
 * @param[in] uint32_t (cluster_number-  the pwm cluster to be selected)
 *            uint32_t (prescalar_value-  value of prescalar values which is used to divide the clock frequency.)
 * @param[Out] No output parameter
 */
void pwm_set_prescalar_value(int module_number, int prescalar_value)
{
	pwm_instance[module_number]->clock = (prescalar_value << 1);
}

/** @fn pwm_reset_all
 * @brief Function to reset all pwm modules
 * @details This function will be called to reset all pwm modules
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void pwm_reset_all(int module_number)
{
	int i = PWM_MAX_COUNT  - 1 ;
	for(;i>0;i--)
	{
		pwm_clear(i);
	}

	pwm_instance[module_number]->clock=0;
	log_debug("\n All registers cleared");
}

/** @fn  pwm_configure
 * @brief Function to configure the pwm module with all the values required like module_number, period, duty, interrupt_mode, deadband_delay, change_output_polarity_
 * @details This function configure the pwm module
 * @param[in] uint32_t (module_number - the pwm module to be selected)
 *           uint32_t(period - value of periodic cycle to be used. the signal resets after every count of the periodic cycle)
 *           uint32_t(duty - value of duty cycle. It specifies how many cycles the signal is active out of the periodic cycle )
 *           pwm_interrupt_modes(interrupt_mode - value of interrupt mode. It specifies the interrupt mode to be used)
 *           uint32_t(deadband_delay - value of deadband_delay. It specifies the deadband_delay to be used.)
 *           bool (change_output_polarity - value of change_output_polarity. It specifies if output polarity is to be changed.)
 * @param[Out] No output parameter
 */
void pwm_configure(int module_number, int period, int duty, pwm_interrupt_modes interrupt_mode, int deadband_delay, bool change_output_polarity)
{
	pwm_instance[module_number]->duty=duty;
	pwm_instance[module_number]->period=period;
	pwm_instance[module_number]->deadband_delay = deadband_delay;

	int control = configure_control( false, interrupt_mode, change_output_polarity);

	pwm_instance[module_number]->control=control;

	log_debug("PWM %d succesfully configured with %x",module_number, pwm_instance[module_number]->control);
}

/** @fn pwm_start
 * @brief Function to start a specific pwm module
 * @details This function will start the specific pwm module
 * @param[in] uint32_t (module_number-  the pwm module to be selected)
 * @param[Out] No output parameter
 */
void pwm_start(int module_number)
{
	int value= 0x0;
	value = pwm_instance[module_number]->control ;

	value |= (PWM_UPDATE_ENABLE | PWM_ENABLE | PWM_START);

	pwm_instance[module_number]->control = value;
}

/** @fn  pwm_update
 * @brief Function to update the pwm module with all the values required like module_number, period, duty, interrupt_mode, deadband_delay, change_output_polarity
 * @details This function udate the pwm module
 * @param[in] uint32_t (module_number - the pwm module to be selected)
 *           uint32_t(period - value of periodic cycle to be used. the signal resets after every count of the periodic cycle)
 *           uint32_t(duty - value of duty cycle. It specifies how many cycles the signal is active out of the periodic cycle )
 *           bool (rise_interrupt - value of rise_interrupt. It specifies if rise_interrupt is to be used.)
 *           pwm_interrupt_modes(interrupt_mode - value of interrupt mode. It specifies the interrupt mode to be used)
 * @param[Out] No output parameter
 */
void pwm_update(int module_number, int period, int duty, pwm_interrupt_modes interrupt_mode , bool change_output_polarity)
{
	pwm_instance[module_number]->duty=duty;
	pwm_instance[module_number]->period=period;

	int control = configure_control( true, interrupt_mode , change_output_polarity);

	pwm_instance[module_number]->control = control;
	
    log_debug("PWM %d succesfully updated",module_number);
}

/** @fn  pwm_stop
 * @brief Function to stop a specific pwm module
 * @details This function will stop a specific pwm module
 * @param[in] uint32_t (module_number-  the pwm module to be selected)
 * @param[out] No output parameter 
 */
void pwm_stop(int module_number)
{
	int value = 0xfff8;  //it will set pwm_enable,pwm_start,pwm_output_enable  to zero
	pwm_instance[module_number]->control &= value;
	log_debug("\n PWM module number %d has been stopped", module_number);
}

void pwm_show_values(int module_number)
{	
	log_info("\n MODULE SPECIFIC REGISTERS");
	log_info("\n PWM %d Period Register %d" ,module_number, pwm_instance[module_number]->period);
	log_info("\n PWM %d Control Register %d" ,module_number, pwm_instance[module_number]->control);
	log_info("\n PWM %d Duty Register %d" ,module_number, pwm_instance[module_number]->duty);
	log_info("\n PWM %d DeadBand Delay Register %d" ,module_number, pwm_instance[module_number]->deadband_delay);
}

//6 pwm
/** @fn  pwm_isr_handler0
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler0(){ 
	
	if(pwm_instance[0]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[0]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[0]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 0 Interrupt");
}

/** @fn  pwm_isr_handler1
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler1(){ 
 
	if(pwm_instance[1]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[1]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[1]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 1 Interrupt");
 
}

/** @fn  pwm_isr_handler2
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler2(){ 
	
	if(pwm_instance[2]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[2]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[2]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 2 Interrupt"); 
}

/** @fn  pwm_isr_handler3
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler3(){ 
	
	if(pwm_instance[3]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[3]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[3]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 3 Interrupt"); 
}

/** @fn  pwm_isr_handler4
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler4(){ 
	
	if(pwm_instance[4]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[4]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[4]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 4 Interrupt"); 
}

/** @fn  pwm_isr_handler5
 * @brief Function to handle isr
 * @details This function will print logs which specify what interrupt has been triggered- rise,fall,halfperiod
 * @param[in] No Input parameter
 * @param[out] No output parameter 
 */
void pwm_isr_handler5(){ 
	
	if(pwm_instance[5]->control & PWM_RISE_INTERRUPT)
		log_debug("Rising Edge Interrupt Triggered");
		
	if(pwm_instance[5]->control & PWM_FALL_INTERRUPT)
		log_debug("Falling Edge Interrupt Triggered");

	if(pwm_instance[5]->control & PWM_HALFPERIOD_INTERRUPT)
		log_debug("HalfPeriod Interrupt Triggered");		

	log_debug("\n PWM 5 Interrupt"); 
}

