#include "gpio.h"

volatile unsigned int *GPIO_GPFSEL0 =  (unsigned int *) 0x20200000;
volatile unsigned int *GPIO_GPSET0  =  (unsigned int *) 0x2020001C;
volatile unsigned int *GPIO_GPCLR0  =  (unsigned int *) 0x20200028;
volatile unsigned int *GPIO_GPLEV0  =  (unsigned int *) 0x20200034;

int gpio_function_select(unsigned int pin, FSEL sel)
{
	/* get correct register for pin */
	volatile unsigned int* gpio_pin_base = GPIO_GPFSEL0 + pin/10;	
	unsigned int curState = 0;
	
	if (pin < 0 || pin > NUM_GPIO_PINS-1 || sel < 0 || sel > NUM_GPIO_FUNCS-1 )
		return -1;	
	
	/* set pin number in register */
	pin %= 10; 	

	/* set register without disturbing other pin configurations */
	curState = *(gpio_pin_base);

	/* clear bits for this pin */
	curState &= ~(7 << (pin * 3)); 
	/* set bits for this pin */
	curState |= sel << (pin * 3);
	
	/* write state back to GPIO reg */
	*gpio_pin_base = curState;
	
	return 0;
}
