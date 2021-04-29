

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