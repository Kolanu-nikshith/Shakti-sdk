/***************************************************************************
* Project               	  : shakti devt board
* Name of the file	          : gpiov2.c
* Brief Description of file       : GPIO Driver Code.
* Name of Author    	          : Soutrick Roy Chowdhury
* Email ID                        : soutrickofficial@gmail.com

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
 * @file  gpiov2.c
 * @project shakti devt board
 * @brief  driver for gpio driver
 */

#include <stdio.h>
#include "gpiov2.h"
#include "platform.h"


gpio_struct *gpiov2_instance;

void check_gpiov2()
{
    printf("\nHey!! I am form GPIO_V2\n");
}

/** @fn gpiov2_init()
 * @brief initializes the gpio instances with the gpio's  base address
 * @param[in]  none
 * @param[Out] none
 */
void gpiov2_init()
{
	gpiov2_instance =(gpio_struct*)GPIO_START;
}

/** @fn long int gpio_read_word(int *addr)
 * @brief returns the value stored at a given address
 * @param addr address of the register to be read
 * @return value strored in the register
 */
long int gpiov2_read_word(int *addr) {
    return *addr;
}

/** @fn void gpio_write_word(int *addr, unsigned log val)
 * @brief  writes a value to an address
 * @details
 * @param addr address of the register to which the value has to be written
 * @param val value that has to be written to the register
 */
void gpiov2_write_word(int *addr, unsigned long val) {
        *addr = val;
}

/** @fn void gpiov2_set_direction(unsigned long gpio_pin,int direction)
 * @brief  writes a value to an direction register
 * @details
 * @param gpio_pin set the pin number you want to work with
 * @param direction pin will set and made to work
 */
void gpiov2_set_direction(unsigned long gpio_pin,int direction) {
	if (direction == GPIO_OUT)
		gpiov2_instance->direction |= gpio_pin;
	else
		gpiov2_instance->direction &= (~(gpio_pin));
}

/**
 * @fn void gpio_set(unsigned long pinNo)
 * @brief Sets the value of GPIODATA register using GPIOSET register
 * @param pinNo GPIO pin number which has to be set
 */
void gpiov2_set(unsigned long pinNo) {
	gpiov2_instance->set |= pinNo;
	gpiov2_instance->set|= 0;
}


/** @fn void gpio_clear(unsigned long pinNo)
 * @brief  Clears the value of GPIODATA register using GPIOCLEAR register
 * @param pinNo GPIO pin which has to be cleared
 */
void gpiov2_clear(unsigned long pinNo) {
		gpiov2_instance->clear |= pinNo;
		gpiov2_instance->clear = 0;
}


/**
 * @fn void gpio_toggle(unsigned long pinNo)
 * @brief  sets the value of GPIODATA register using GPIOTOGGLE register
 * @param pinNo GPIO pin which has to be toggled
 */
void gpiov2_toggle(unsigned long pinNo) {
		gpiov2_instance->toggle |= pinNo;
		gpiov2_instance->toggle= 0;
}


/**
 * @fn void gpio_interrupt_config(unsigned long pinNo, int toggleInterrupt)
 * @brief  When active low interrupts are required, the respective GPIO is set in GPIOINTRCONFIG register
 * @details  When active low interrupts are required, the respective GPIO is set in GPIOINTRCONFIG register
             to set bit toggleInterupt value should be 1, to clear it toggleInterrupt bit should be 0
 * @param pinNo Pin number
 * @param toggleInterrupt
*/
void gpiov2_interrupt_config(unsigned long pinNo, int low_ena) {
	if(low_ena )
		gpiov2_instance->intr_config |= pinNo;
	else
		gpiov2_instance->intr_config &= ~(pinNo);
}

/**
 * @fn void void gpio_set_qualification_cycles(unsigned int)
 * @brief  sets GPIO Input Qualification Control Register
 * @details qualification cycles can be from 0 to 15
 * @param cycles number of cycles to wait before seting the GPIODATA register for pins configured as input
 */
void gpiov2_set_qualification_cycles(unsigned int cycles) {
	if (cycles > GPIO_QUAL_MAX_CYCLES)
		cycles = GPIO_QUAL_MAX_CYCLES;
		gpiov2_instance->qualification|= cycles;
}


/** @fn long int gpio_read_data_register()
 * @brief returns the value stored at a given address
 * @return value strored in the data register
 */
long int gpiov2_read_data_register() {
		return gpiov2_instance->data;
}

/** @fn void gpio_write_data_register()
 * @brief writes the passed value into gpio data register
 * @return Nil
 */
void gpiov2_write_data_register(unsigned long data_word) {
	gpiov2_instance->data = data_word;
}

