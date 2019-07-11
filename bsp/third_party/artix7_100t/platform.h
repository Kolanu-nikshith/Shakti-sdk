/*
platform.h - header file for specific platform

Created by Sathya Narayanan N
Email id: sathya281@gmail.com

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

*/

#ifndef PLATFORM_H
#define PLATFORM_H

#define MTIME 0x0200BFF8

#define MCAUSE_INT         0x80000000
#define MCAUSE_CAUSE       0x7FFFFFFF


/*
 *@brief RISCV - C CLASS SOC Memory mapping
 */
/*!Debugger Offset */
#define DBG_MEM_START 0x00000000

/*!Tightly Coupled Memory(TCM) Offset. Size 128kB */
#define DDR_MEM_START 0x80000000 /*! DDR3 Memory Start address */

/*! Core Local Interruptor CLINT */
#define CLINT_START 0x02000000

/*! Inter Integrated Circuit (I2C) Interface */
#define I2C_START 0x020C0000 /*! I2C Start Address */

/*! Programmable Logic Interrupt Interface */
#define PLIC_START 0x020D0000 /*! PLIC Interface Start */

/*!General Purpose Input / Output */
#define GPIO_START 0x02100000 //GPIO Start Address */
#define GPIO_OFFSET 0x04 /*!Generic offset used to access GPIO registers*/

/* 
 * General Purpose IOs supported 
 */

#define GPIO0 (1 <<  0)
#define GPIO1 (1 <<  1)
#define GPIO2 (1 <<  2)
#define GPIO3 (1 <<  3)
#define GPIO4 (1 <<  4)
#define GPIO5 (1 <<  5)
#define GPIO6 (1 <<  6)
#define GPIO7 (1 <<  7)
#define GPIO8 (1 <<  8)
#define GPIO9 (1 <<  9)
#define GPIO10 (1 << 10)
#define GPIO11 (1 << 11)
#define GPIO12 (1 << 12)
#define GPIO13 (1 << 13)
#define GPIO14 (1 << 14)
#define GPIO15 (1 << 15)

#define GPIO_COUNT  0x10

#endif



#endif
