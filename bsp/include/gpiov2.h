
/**
 * @file  gpiov2.h
 * @project shakti devt board
 * @brief  header file for gpio driver
 */

#include <stdint.h>
#include "platform.h"

#define GPIO_DIRECTION_CNTRL_REG  (GPIO_START + (0 * GPIO_OFFSET ))
#define GPIO_DATA_REG             (GPIO_START + (1 * GPIO_OFFSET ))
#define GPIO_SET_REG              (GPIO_START + (2 * GPIO_OFFSET ))
#define GPIO_CLEAR_REG            (GPIO_START + (3 * GPIO_OFFSET ))
#define GPIO_TOGGLE_REG           (GPIO_START + (4 * GPIO_OFFSET ))
#define GPIO_QUAL_REG             (GPIO_START + (5 * GPIO_OFFSET ))
#define GPIO_INTERRUPT_CONFIG_REG (GPIO_START + (6 * GPIO_OFFSET ))

#define GPIO_IN  0
#define GPIO_OUT 1
#define GPIO_QUAL_MAX_CYCLES 15
#define ALL_GPIO_PINS -1


/* Struct to access GPIO registers as 16 bit registers */
typedef struct
{
	uint32_t  direction;	           /*! direction register */
	uint32_t  reserved0;                /*! reserved for future use */
	uint32_t  data;	        		   /*! data register */
	uint32_t  reserved1;                /*! reserved for future use */
	uint32_t  set;	         	   /*! set register */
	uint32_t  reserved2;                /*! reserved for future use */
	uint32_t  clear;	         	   /*! clear register */
	uint32_t  reserved3;                /*! reserved for future use */
	uint32_t  toggle;	            /*! toggle register */
	uint32_t  reserved4;                /*! reserved for future use */
	uint8_t  qualification;	   /*! qualification register */
	uint8_t  reserved5;                /*! reserved for future use */
	uint16_t  reserved6;              /*! reserved for future use */
	uint32_t  reserved12;              /*! reserved for future use */
	uint32_t  intr_config;	   /*! interrupt configuration register */
//	uint16_t  reserved13;              /*! reserved for future use */
	uint32_t  reserved7;              /*! reserved for future use */
} gpio_struct;

extern gpio_struct *gpiov2_instance;

void check_gpiov2();
void gpiov2_init();
long int gpiov2_read_word(int *addr);
void gpiov2_write_word(int *addr, unsigned long val);
void gpiov2_set_direction(unsigned long gpio_pin,int direction);
void gpiov2_set(unsigned long);
void gpiov2_clear(unsigned long);
void gpiov2_toggle(unsigned long);
void gpiov2_interrupt_config(unsigned long, int toggleInterupt);
void gpiov2_set_qualification_cycles(unsigned int);
long int gpiov2_read_data_register();
