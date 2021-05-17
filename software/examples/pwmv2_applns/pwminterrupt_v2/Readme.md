
The readme is a brief overview on how pwm based interrupt.

### OVERVIEW
  * This is the program tested different interrupt based on pwm.
  * src code:pwminterrupt_v2.c

### STEPS
  * The different type of interrupt are:
    - Rise Interrupt.
    - Fall Interrupt.
    - Half Period Interrupt.
    - Interrupt when Output is disable.

  * To get Active low or Active high:
    - Set the intr_config reg 0xff for low and 0x00 for high
    - To get the active low interrupt run the source code interrupt will come and to pause the      interrupt short the same GPIO pin as mentioned in the src code to VCC.
    - To get the active high interrupt run the source code interrupt will come when you short the same GPIO pin as mentioned in the src code to VCC.

   * To get  Active high to low or Active low to high:
   - Enable the macros as set in the src code

### RESULTS
  * Active high and low interrupt:
    - When the interrupt will occur one board led will toggle.

  * Active low to high or Active high to low:
    - Interrupt will switch from high to low and vice versa.  
