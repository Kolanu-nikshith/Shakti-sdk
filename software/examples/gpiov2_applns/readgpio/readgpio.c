
#include <stdio.h>
#include "gpio.h"
#include "utils.h"
#include "platform.h"

void main()
{
    unsigned long data = 0;

    printf(" Read GPIO\n");
	write_word(GPIO_DIRECTION_CNTRL_REG, 0x00000000);

	while (1) 
    {
		data =read_word(GPIO_DATA_REG);             //copy GPIO_DATA_REG content
		log_info("\n Read Data is :%x", data);
		delay_loop(3000, 5000);
	}
}