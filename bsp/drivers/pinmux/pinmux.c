/****************************************************************************
 * Project                               : shakti devt board
 * Name of the file                      : uart.c
 * Brief Description of file             : src  file for uart
 * Name of Author                        : Kotteeswaran and Niketan Shahapur
 * Email ID                              : <kottee.1@gmail.com>  <niketanshahpur@gmail.com>

 Copyright (C) 2019 IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
  @file uart.c
  @brief Contains the driver routines for UART interface.
  @detail The UART driver .has software functions to configure, transmit
  and receive over UART interface.
 */

#define PIN_MUX_DRIVER 1
#include "pinmux.h"
#include "utils.h"


void pinmux_enable_uart1()
{
	*pinmux_config_reg = ENABLE_UART1;
}


void pinmux_enable_uart2()
{
	*pinmux_config_reg = ENABLE_UART2;
}

void pinmux_enable_uart1_uart2()
{
	*pinmux_config_reg = ENABLE_UART1 | ENABLE_UART2;
}


void pinmux_enable_spi1()
{
	*pinmux_config_reg = ENABLE_SPI1;
}

void pinmux_enable_pwm(uint8_t pwm_no)
{
	switch(pwm_no)
	{
		case 0:
			*pinmux_config_reg = ENABLE_PWM0;
			break;

		case 1:
			*pinmux_config_reg = ENABLE_PWM1;
			break;
			
		case 2:
			*pinmux_config_reg = ENABLE_PWM2;
			break;
			
		case 3:
			*pinmux_config_reg = ENABLE_PWM3;
			break;
			
		case 4:
			*pinmux_config_reg = ENABLE_PWM4;
			break;
			
		case 5:
			*pinmux_config_reg = ENABLE_PWM5;
			break;

		default:
			break;
			
	}
}

void pinmux_enable_all_pwms()
{
	*pinmux_config_reg = ENABLE_ALL_PWM;
}
