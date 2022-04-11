/****************************************************************************
 * Project               	: shakti devt board
 * Name of the file	        : pinmux.h
 * Brief Description of file    : store pinmux configurations
 * Name of Author    	        : Sathya Narayanan N
 * Email ID                     : sathya281@gmail.com
 
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
/**
 * @file pinmux.h
 * @brief store pinmux configuration
 */

#ifndef PINMUX_H
#define PINMUX_H
#include <stdint.h>
#include "platform.h"

#ifdef  PIN_MUX_DRIVER
volatile unsigned int* pinmux_config_reg = (volatile unsigned int* ) PINMUX_CONFIGURE_REG;
#else
extern volatile unsigned int* pinmux_config_reg;
#endif


#define ENABLE_UART1_RX 1 << 0
#define ENABLE_UART1_TX 1 << 2
#define ENABLE_UART2_RX 1 << 4
#define ENABLE_UART2_TX 1 << 6
#define ENABLE_UART1 ENABLE_UART1_RX | ENABLE_UART1_TX
#define ENABLE_UART2 ENABLE_UART2_RX | ENABLE_UART2_TX
#define ENABLE_UART1_UART2 ENABLE_UART1 | ENABLE_UART2

#define ENABLE_SPI1     (1 << 14) | (1 << 16) | (1 << 18) | (1 << 20)

#define ENABLE_PWM0 2 << 6
#define ENABLE_PWM1 2 << 8
#define ENABLE_PWM2 2 << 10
#define ENABLE_PWM3 2 << 12
#define ENABLE_PWM4 2 << 14
#define ENABLE_PWM5 2 << 16
#define ENABLE_ALL_PWM ENABLE_PWM0 | ENABLE_PWM1 | ENABLE_PWM2 | \
        ENABLE_PWM3 | ENABLE_PWM4 | ENABLE_PWM5

/* 
#ifdef  PIN_MUX_DRIVER
*/
void pinmux_enable_uart1();
void pinmux_enable_uart2();
void pinmux_enable_uart1_uart2();
void pinmux_enable_spi1();
void pinmux_enable_pwm(uint8_t );
void pinmux_enable_all_pwms();
/*
#else

extern void pinmux_enable_uart1();
extern void pinmux_enable_uart2();
extern void pinmux_enable_uart1_uart2();
extern void pinmux_enable_spi1();
extern void pinmux_enable_pwm(uint8_t );
extern void pinmux_enable_all_pwms();
#endif
*/

#endif
