

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
 * @brief Header file for PWM Driver.
 */

#include<stdbool.h>

typedef struct
{
	uint16_t clock;    			/*! pwm clock register 16 bits*/
	uint16_t reserved0;
	uint16_t control;    			/*! pwm control register 16 bits*/
	uint16_t reserved1;
	uint32_t period;			/*! pwm period register 32 bits */
	uint32_t duty;				/*! pwm duty cycle register 32 bits*/
	uint16_t deadband_delay;		/*! pwm deadband delay register 16 bits */
	uint16_t reserved2;
}pwm_struct;


typedef enum
{
	rise_interrupt,				//Enable interrupt only on rise
	fall_interrupt,				//Enable interrupt only on fall
	halfperiod_interrupt,			//Enable interrupt only on halfperiod
	rise_fall_interrupt,			//Enable interrupt on rise and fall
	fall_halfperiod_interrupt,		//Enable interrupt on fall and halfperiod
	rise_halfperiod_interrupt,		//Enable interrupt on rise and halfperiod
	rise_fall_halfperiod_interrupt,		//Enable interrupt on rise, fall and halfperiod
	no_interrupt				//Disable interrupts
}pwm_interrupt_modes;

// Control Register Individual Bits 
#define PWM_ENABLE 	 			0x00000001
#define PWM_START          			0x00000002
#define PWM_OUTPUT_ENABLE  			0x00000004
#define PWM_OUTPUT_POLARITY     		0x00000008
#define PWM_COUNTER_RESET   			0x00000010
#define PWM_HALFPERIOD_INTERRUPT_ENABLE         0x00000040
#define PWM_FALL_INTERRUPT_ENABLE       	0x00000080
#define PWM_RISE_INTERRUPT_ENABLE       	0x00000100
#define PWM_HALFPERIOD_INTERRUPT      		0x00000200
#define PWM_FALL_INTERRUPT      		0x00000400
#define PWM_RISE_INTERRUPT      		0x00000800
#define PWM_UPDATE_ENABLE			0x00001000

//function prototype
void check();
void pwm_init();
void pwm_clear(int module_number);
void pwm_set_duty_cycle(int module_number, uint32_t duty);
void pwm_set_periodic_cycle(int module_number, uint32_t period);
int pwm_set_control(int module_number, uint32_t value);
void pwm_stop(int module_number);
int pwm_set_deadband_delay(int module_number, uint32_t value);
int configure_control(bool update, pwm_interrupt_modes interrupt_mode, bool change_output_polarity);
void pwm_set_prescalar_value(int cluster_number, uint16_t prescalar_value);
void pwm_reset_all();
void pwm_configure(int module_number, uint32_t period, uint32_t duty, pwm_interrupt_modes interrupt_mode , uint32_t deadband_delay, bool change_output_polarity); 
void pwm_update(int module_number, uint32_t period, uint32_t duty, pwm_interrupt_modes interrupt_mode , bool change_output_polarity);
void show_values(int module_number);
void pwm_isr_handler0();
void pwm_isr_handler1();
void pwm_isr_handler2();
void pwm_isr_handler3();
void pwm_isr_handler4();
void pwm_isr_handler5();


