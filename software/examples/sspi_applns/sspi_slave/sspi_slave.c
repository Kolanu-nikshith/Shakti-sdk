#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "pinmux.h"

#define SSPI_SIZE 8
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

uint32_t* spi0_commctrl		=	(const uint32_t*) SSPI0_COMMCTRL;
uint32_t* spi0_clkctrl 		=	(const uint32_t*) SSPI0_CLKCTRL;
uint8_t* spi0_tx8			=	(const uint8_t*) SSPI0_TXREG;
uint16_t* spi0_tx16			=	(const uint16_t*) SSPI0_TXREG;
uint32_t* spi0_tx32			=	(const uint32_t*) SSPI0_TXREG;
uint8_t* spi0_rx8			=	(const uint8_t*) SSPI0_RXREG;
uint16_t* spi0_rx16			=	(const uint16_t*) SSPI0_RXREG;
uint32_t* spi0_rx32			=	(const uint32_t*) SSPI0_RXREG;
uint8_t* spi0_fifostatus		=	(const uint8_t*) SSPI0_FIFOSTATUS;
uint8_t* spi0_commstatus	=	(const uint8_t*) SSPI0_COMMSTATUS;
uint32_t* spi0_inqual		=	(const uint32_t*) SSPI0_INQUAL;


uint32_t* spi1_commctrl		=	(const uint32_t*) SSPI1_COMMCTRL;
uint32_t* spi1_clkctrl 		=	(const uint32_t*) SSPI1_CLKCTRL;
uint8_t* spi1_tx8			=	(const uint8_t*) SSPI1_TXREG;
uint16_t* spi1_tx16			=	(const uint16_t*) SSPI1_TXREG;
uint32_t* spi1_tx32			=	(const uint32_t*) SSPI1_TXREG;
uint8_t* spi1_rx8			=	(const uint8_t*) SSPI1_RXREG;
uint16_t* spi1_rx16			=	(const uint16_t*) SSPI1_RXREG;
uint32_t* spi_rx32			=	(const uint32_t*) SSPI1_RXREG;
uint8_t* spi1_fifostatus		=	(const uint8_t*) SSPI1_FIFOSTATUS;
uint8_t* spi1_commstatus	=	(const uint8_t*) SSPI1_COMMSTATUS;
uint32_t* spi1_inqual		=	(const uint32_t*) SSPI1_INQUAL;


uint8_t spi1_tx[SSPI_SIZE] = {0};
uint8_t spi1_rx[SSPI_SIZE] = {0};

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
	//gpt_struct* instance;
	uint32_t temp = 0, retval = 0;
	uint8_t i = 0, j = 0;
	uint8_t temp_data = 0;
	*(pinmux_config_reg) = 0x154000;

	printf("\n SSPI1 Slave Full Duplex Check\n");

	printf("\n Configure SSPI");
	temp = spi_notbusy(1);
	printf("SSPI1 status: %x \n", temp);

	set_shakti8(spi1_inqual, 0x00);
	temp = spi_notbusy(1);
	printf("SSPI1 status: %x \n", temp);

	delay_loop(2000, 1000);
	while(1)
	{
	
			temp_data = 0x01;
			for(i = 0; i < SSPI_SIZE; i++)
			{
				spi1_rx[i] = 0;
				spi1_tx[i] = temp_data + 0x10;
				temp_data += 0x01;
			}
			printf("\n Filling Fifo data");
			for(i = 0; i < SSPI_SIZE; i++)
			{
				*spi1_tx8 = spi1_tx[i];
			}

			set_shakti32(spi1_commctrl,(COMMCTRL_MISO_OUTEN | 
				COMMCTRL_TX_BITS(SSPI_SIZE * DATA_SIZE) | \
				COMMCTRL_RX_BITS(SSPI_SIZE * DATA_SIZE) | COMMCTRL_COMM_MODE(3) | \
				COMMCTRL_SPI_EN));

			temp = spi_notbusy(1);

			printf("\n SSPI 1 Commn status: %x", *spi1_commstatus );
			printf("\n SSPI 1 Fifo status: %x", *spi1_fifostatus );
			
			if(  (  ( (0x4 & get_shakti8(spi1_commstatus) ) != 0) ) )
			{
			printf("\n SSPI 1 Commn status: %x", *spi1_commstatus );
			printf("\n SSPI 1 Fifo status: %x", *spi1_fifostatus );

				for(int i = 0; i < SSPI_SIZE; i++)
				{
					printf("\n i: %x", i);
					spi1_rx[i] =  *spi1_rx8;
					
				}
				printf("\n SSPI READ VALUES");
				
				for(int i = 0; i < SSPI_SIZE; i++)
				{
					printf("\n SSPI1[%x]: %x", i, spi1_rx[i]);
				}
			}
			else
			{
				printf("\n Didn't receive any data ");

			}
				delay_loop(2000, 1000);
		
			}

  return 0;
}


