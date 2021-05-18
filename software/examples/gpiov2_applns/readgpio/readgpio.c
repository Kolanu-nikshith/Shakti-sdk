/***************************************************************************
 * Project           	      : shakti devt board
 * Name of the file	      : readgpio.c
 * Brief Description of file  :Performs the reading operation with the help of gpio pins.
 * Name of Author             : Soutrick Roy Chowdhury
 * Email ID                   : soutrickofficial@gmail.com

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

*****************************************************************************/

#include "gpiov2.h"
#include "utils.h"
#include "platform.h"

/*
1. Write into the Direction register to configure GPIO pin as an input or output
2. Write appropriate values to data register
3. If any of GPIO pin is an input, set an appropriate input qualification cycles
in Qual register
4. Configure Set, Clear, Toggle or Interrupt config registers appropriately if
needed
*/


/**
  * @fn
  * @brief
  * @detiail
*/

void main () 
{

	check_gpiov2();
	printf("\n Read from GPIO Pins\n");
	gpiov2_init();
	gpiov2_instance->direction = 0x0;

	while(1)
	{
		printf("\n GPIO Read Data: %x", gpiov2_instance->data );
		delay_loop(1000, 1000);
	}

}