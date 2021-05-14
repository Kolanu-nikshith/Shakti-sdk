/***************************************************************************
* Project           		: shakti devt board
* Name of the file	     	: tglgpio.c
* Brief Description of file     : Control an led with the help of a button, gpio based.
* Name of Author    	        : Sathya Narayanan N
* Email ID                      : sathya281@gmail.com

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
@file tglgpio.c
@brief Control an led with the help of a button, gpio based.
*/

#include "platform.h"
#include "utils.h" 
#include "gpiov2.h" 

/** @fn main
 * @brief Initiates writing to gpio pins by calling tglgpio
 */ 
void main()
{
	check_gpiov2();
	printf("\n Starting Toggle GPIO tests \n");
	gpiov2_init();
	gpiov2_instance->direction = 0xff0fffff;

#if 0	
		/* Toggling All the pins using clear  */

	while(1)
	{
		gpiov2_instance->set = 0xff0fffff;
		delay_loop(3000, 1000);
		gpiov2_instance->set = 0;
		gpiov2_instance->clear = 0xff0fffff;
		delay_loop(3000, 1000);
		gpiov2_instance->clear = 0;
	}

#endif

#if 1
		/* Toggling by giving specific pins number*/

	while (1)
	{
		gpiov2_instance->toggle = 0xff0fffff;
		delay_loop(3000, 1000);
	}
#endif

}

