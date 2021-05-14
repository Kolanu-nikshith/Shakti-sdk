/***************************************************************************
 * Project               	    		:  shakti devt board
 * Name of the file	            		:  sspi_full_duplex.c
 * Brief Description of file             :  sspi full duplex example code.
 * Name of Author    	                :  Kotteeswaran
 * Email ID                              :  kottee.1@gmail.com

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

#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "pinmux.h"
#define USE_INTERRUPT 1

#define SSPI_SIZE 18
#define DATA_SIZE 8

#define SSPI0_COMMCTRL		0x00020000
#define SSPI0_CLKCTRL		0x00020004
#define SSPI0_TXREG		0x00020008
#define SSPI0_RXREG		0x0002000C
#define SSPI0_INTR_EN		0x00020010
#define SSPI0_FIFOSTATUS	0x00020014
#define SSPI0_COMMSTATUS	0x00020018
#define SSPI0_INQUAL		0x0002001C

#define COMMCTRL_MOSI_OUTEN		(1<<25)
#define COMMCTRL_MISO_OUTEN		(1<<24)
#define COMMCTRL_SCLK_OUTEN		(1<<23)
#define COMMCTRL_CS_OUTEN		(1<<22)
#define COMMCTRL_RX_BITS(x)		(x<<14)
#define COMMCTRL_TX_BITS(x)		(x<<6)
#define COMMCTRL_COMM_MODE(x)		(x<<4)
#define COMMCTRL_LSB_FIRST		(1<<2)
#define COMMCTRL_SPI_EN			(1<<1)
#define COMMCTRL_MASTER_MODE		(1<<0)

#define CLKCTRL_T_CS(x)			(x<<18)
#define CLKCTRL_CS_T(x)			(x<<10)
#define CLKCTRL_PRESCALE(x)		(x<<2)
#define CLKCTRL_PHASE			(1<<1)
#define CLKCTRL_POLARITY		(1<<0)


#define SSPI1_COMMCTRL		0x00020100
#define SSPI1_CLKCTRL		0x00020104
#define SSPI1_TXREG		0x00020108
#define SSPI1_RXREG		0x0002010C
#define SSPI1_INTR_EN		0x00020110
#define SSPI1_FIFOSTATUS	0x00020114
#define SSPI1_COMMSTATUS	0x00020118
#define SSPI1_INQUAL		0x0002011C

volatile uint32_t* spi0_commctrl		=	(const uint32_t*) SSPI0_COMMCTRL;
volatile uint32_t* spi0_clkctrl 		=	(const uint32_t*) SSPI0_CLKCTRL;
volatile uint8_t* spi0_tx8			=	(const uint8_t*) SSPI0_TXREG;
volatile uint16_t* spi0_tx16			=	(const uint16_t*) SSPI0_TXREG;
volatile uint32_t* spi0_tx32			=	(const uint32_t*) SSPI0_TXREG;
volatile uint8_t* spi0_rx8			=	(const uint8_t*) SSPI0_RXREG;
volatile uint16_t* spi0_rx16			=	(const uint16_t*) SSPI0_RXREG;
volatile uint32_t* spi0_rx32			=	(const uint32_t*) SSPI0_RXREG;
volatile uint8_t* spi0_fifostatus		=	(const uint8_t*) SSPI0_FIFOSTATUS;
volatile uint8_t* spi0_commstatus	=	(const uint8_t*) SSPI0_COMMSTATUS;
volatile uint32_t* spi0_inqual		=	(const uint32_t*) SSPI0_INQUAL;


volatile uint32_t* spi1_commctrl		=	(const uint32_t*) SSPI1_COMMCTRL;
volatile uint32_t* spi1_clkctrl 		=	(const uint32_t*) SSPI1_CLKCTRL;
volatile uint8_t* spi1_tx8			=	(const uint8_t*) SSPI1_TXREG;
volatile uint16_t* spi1_tx16			=	(const uint16_t*) SSPI1_TXREG;
volatile uint32_t* spi1_tx32			=	(const uint32_t*) SSPI1_TXREG;
volatile uint8_t* spi1_rx8			=	(const uint8_t*) SSPI1_RXREG;
volatile uint16_t* spi1_rx16			=	(const uint16_t*) SSPI1_RXREG;
volatile uint32_t* spi_rx32			=	(const uint32_t*) SSPI1_RXREG;
volatile uint8_t* spi1_fifostatus		=	(const uint8_t*) SSPI1_FIFOSTATUS;
volatile uint8_t* spi1_commstatus	=	(const uint8_t*) SSPI1_COMMSTATUS;
volatile uint32_t* spi1_inqual		=	(const uint32_t*) SSPI1_INQUAL;
volatile uint16_t* spi1_intr_enable			=	(const uint16_t*) SSPI1_INTR_EN;

uint8_t spi0_tx[SSPI_SIZE] = {0};
uint8_t spi0_rx[SSPI_SIZE] = {0};

uint8_t spi1_tx[SSPI_SIZE] = {0};
uint8_t spi1_rx[SSPI_SIZE] = {0};

uint8_t spi0_intr_flag = 0;
uint8_t spi1_intr_flag = 0;

__attribute__((always_inline))
static inline void set_shakti32(uint32_t* addr, uint32_t val)
{
	*addr = val;
}

__attribute__((always_inline))
static inline void set_shakti16(uint16_t* addr, uint16_t val)
{
	*addr = val;
}

__attribute__((always_inline))                                                                      
static inline void set_shakti8(uint8_t* addr, uint8_t val)                                   
{                                                                                                   
    *addr = val;                                                                                    
} 

__attribute__((always_inline))
static inline uint32_t get_shakti32(uint32_t* addr)
{
	return *addr;
}

__attribute__((always_inline))
static inline uint16_t get_shakti16(uint16_t* addr)
{
	return *addr;
}


__attribute__((always_inline))
static inline uint8_t get_shakti8(uint8_t* addr)
{
	return *addr;
}

unsigned spi0_rx_interrupt (unsigned num)
{
	printf("Got intr:%d\n", num);
	for(int i = 0; i < SSPI_SIZE; i++)
	{
//		printf("\n i: %x", i);
		spi0_rx[i] =  *spi0_rx8;
//		spi1_rx[i] =  *spi1_rx8;
		
	}
	spi0_intr_flag = 1;
}

unsigned spi1_rx_interrupt (unsigned num)
{
	printf("\n ISR");
	printf("Got intr:%d\n", num);
	for(int i = 0; i < 32; i++)
	{
//		printf("\n i: %x", i);
//		spi0_rx[i] =  *spi0_rx8;
		spi1_rx[i] =  *spi1_rx8;
		
	}
	spi1_intr_flag = 1;
}


int spi_notbusy(char number)
{
	int temp = 0x00000001;
	while(temp & 0x00000001){
		waitfor(10);
		printf("\n SSPI %x Status is %x", number, temp);
		if(number == 0)
			temp = get_shakti8(spi0_commstatus);
		else if(number == 1)
			temp = get_shakti8(spi1_commstatus);
		else 
		{
			printf("\n Incorrect SPI number");
			return -1;
		}
	}
//	printf("\n SPI comm done \n");
	return 1;
}


/** @fn main() 
 * @brief main function that runs the code
 * @details runs the code
 * @warning No warning
 * @param[in] No input parameter 
 * @param[Out] No output parameter
 */
int main()
{

//	gpt_struct* instance;
	uint32_t temp = 0, retval = 0;
	uint8_t i = 0, j = 0;
	uint8_t temp_data = 0;
	*(pinmux_config_reg) = 0x154000;

	printf("\n SSPI1 Interrupt Check\n");

	printf("\n Configure SSPI");
//	temp = spi_notbusy(0);
//	printf("SSPI0 status: %x \n", temp);

	temp = spi_notbusy(1);
	printf("SSPI1 status: %x \n", temp);

	set_shakti32(spi1_clkctrl,(CLKCTRL_PRESCALE(60)|CLKCTRL_POLARITY | CLKCTRL_PHASE));
	set_shakti8(spi1_inqual, 0x00);
	temp = spi_notbusy(1);
	printf("SSPI0 status: %x \n", temp);

//	set_shakti32(sspi1_clkctrl,(CLKCTRL_PRESCALE(30) |CLKCTRL_POLARITY | CLKCTRL_PHASE));
	set_shakti8(spi1_inqual, 0x00);
	temp = spi_notbusy(1);
	printf("SSPI1 status: %x \n", temp);

	configure_interrupt(PLIC_INTERRUPT_30);
	printf("Setting prority to source 30 \n");
	set_shakti16(0x0C000078,4);
	uint16_t temp_prior = get_shakti16(0x0C000078);
	printf("Set priority to source 30 %d \n",temp_prior);
	set_shakti16(0x0C200000,2);
	temp_prior = get_shakti16(0x0C200000);
	printf("Set priority to source 30 %d \n",temp_prior);

	isr_table[PLIC_INTERRUPT_30] = spi1_rx_interrupt;
	// Enable Global (PLIC) interrupts.
	asm volatile("li	  t0, 8\t\n"
			 "csrrs   zero, mstatus, t0\t\n"
			);

	asm volatile("li	  t0, 0x800\t\n"
			 "csrrs   zero, mie, t0\t\n"
			);

	asm volatile(
			 "csrr %[retval], mstatus\n"
			 :
			 [retval]
			 "=r"
			 (retval)
			);

	printf(" retval = %x\n", retval);
	asm volatile(
			 "csrr %[retval], mie\n"
			 :
			 [retval]
			 "=r"
			 (retval)
			);

	printf(" retval = %x\n", retval);

	asm volatile(
			 "csrr %[retval], mip\n"
			 :
			 [retval]
			 "=r"
			 (retval)
			);

//	*spi0_intr_enable = 0x20;
	*spi1_intr_enable = 0x80;


	delay_loop(2000, 1000);

//	int count = 0;
//	while(count < 3)

	while(1)
	{
	
			temp_data = 0x01;
			for(i = 0; i < SSPI_SIZE; i++)
			{
				spi1_rx[i] = 0;
				spi1_tx[i] = temp_data;
//				spi1_rx[i] = 0;
//				spi1_tx[i] = temp_data + 0x10;
				temp_data += 0x01;
			}
			printf("\n Filling Fifo data");
			for(i = 0; i < SSPI_SIZE; i++)
			{
//				*spi0_tx8 = spi0_tx[i];
				*spi1_tx8 = spi1_tx[i];
			}
			
			set_shakti32(spi1_commctrl,(COMMCTRL_MOSI_OUTEN | COMMCTRL_SCLK_OUTEN | \
				COMMCTRL_CS_OUTEN | COMMCTRL_TX_BITS(SSPI_SIZE * DATA_SIZE) | \
				COMMCTRL_RX_BITS(SSPI_SIZE * DATA_SIZE) | COMMCTRL_COMM_MODE(3) | \
				COMMCTRL_SPI_EN | COMMCTRL_MASTER_MODE));
//			temp = spi_notbusy(0);
			temp = spi_notbusy(1);
		
			// sending data again
			printf("\n Sending data again");
			temp_data = 0x01;
			for(i = 0; i < SSPI_SIZE; i++)
			{
				spi1_rx[i] = 0;
				spi1_tx[i] = temp_data;
//				spi1_rx[i] = 0;
//				spi1_tx[i] = temp_data + 0x10;
				temp_data += 0x01;
			}
			printf("\n Filling Fifo data");
			for(i = 0; i < SSPI_SIZE; i++)
			{
//				*spi0_tx8 = spi0_tx[i];
				*spi1_tx8 = spi1_tx[i];
			}
			
			set_shakti32(spi1_commctrl,(COMMCTRL_MOSI_OUTEN | COMMCTRL_SCLK_OUTEN | \
				COMMCTRL_CS_OUTEN | COMMCTRL_TX_BITS(SSPI_SIZE * DATA_SIZE) | \
				COMMCTRL_RX_BITS(SSPI_SIZE * DATA_SIZE) | COMMCTRL_COMM_MODE(3) | \
				COMMCTRL_SPI_EN | COMMCTRL_MASTER_MODE));
//			temp = spi_notbusy(0);
			temp = spi_notbusy(1);

//			printf("\n SSPI 0 Commn status: %x", *spi0_commstatus );
//			printf("\n SSPI 0 Fifo status: %x", *spi0_fifostatus );
			printf("\n SSPI 1 Commn status: %x", *spi1_commstatus );
			printf("\n SSPI 1 Fifo status: %x", *spi1_fifostatus );
			
//			if( ( (0x4 & get_shakti8(spi1_commstatus) ) != 0) )
//			{
//			printf("\n SSPI 0 Commn status: %x", *spi0_commstatus );
//			printf("\n SSPI 0 Fifo status: %x", *spi0_fifostatus );
			printf("\n SSPI 1 Commn status: %x", *spi1_commstatus );
			printf("\n SSPI 1 Fifo status: %x", *spi1_fifostatus );

#ifdef USE_INTERRUPT
			if(spi1_intr_flag)
			{
#endif
#ifndef USE_INTERRUPT
				for(int i = 0; i < 32; i++)
				{
					printf("\n i: %x", i);
					spi1_rx[i] =  *spi1_rx8;
					
				}

#endif
				printf("\n SSPI READ VALUES");
				
				for(int i = 0; i < 32; i++)
				{
					printf("\n SSPI1[%x]: %x", i, spi1_rx[i]);
	//				printf("\n ADC[%x]:%x", i, adc_read[i]);
				}
#ifdef USE_INTERRUPT
			}
			else
			{
				printf("\n Didn't receive any data ");

			}
#endif
//			printf("Read values from ADC \n");
				delay_loop(2000, 1000);
		
//				count = count + 1;
			}
  return 0;
}


