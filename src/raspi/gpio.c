#include "gpio.h"
#include "error.h"

volatile unsigned int *GPIO_GPFSEL0 =  (unsigned int *) 0x20200000;
volatile unsigned int *GPIO_GPSET0  =  (unsigned int *) 0x2020001C;
volatile unsigned int *GPIO_GPCLR0  =  (unsigned int *) 0x20200028;
volatile unsigned int *GPIO_GPLEV0  =  (unsigned int *) 0x20200034;

int gpio_function_select(unsigned int pin, FSEL sel)
{
	/* get correct register for pin */
	volatile unsigned int* gpio_pin_base = GPIO_GPFSEL0 + pin/10;	
	unsigned int curState = 0;
	
	if (pin < 0 || pin > GPIO_NUM_PINS-1 || sel < 0 || sel > GPIO_NUM_FUNCS-1 )
		return ERR_GPIO_INVALID_PIN;	
	
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
	
	return ERR_NOERR;
}


/*
 * Sets a pin configured as an output to logic high or low
 * GPIO_OUTPUT_LOW = low
 * GPIO_OUTPUT_HIGH = high
 */
int gpio_set_output_level (int pin, GPIO_OUTPUT_LEVEL level)
{
	if (pin < 0 || pin > GPIO_NUM_PINS-1)
		return ERR_GPIO_INVALID_PIN;
	
	/* select register, 32 pins per reg */
	unsigned int reg_offset = pin/32;	
	
	/* select bit in reg */
	unsigned int bit = pin%32;
		
	if (level == GPIO_OUTPUT_LOW)
	{
		*(GPIO_GPCLR0 + reg_offset) = 1<<bit;
	}
	else
	{
		*(GPIO_GPSET0 + reg_offset) = 1<<bit;
	}
	return ERR_NOERR;
}
