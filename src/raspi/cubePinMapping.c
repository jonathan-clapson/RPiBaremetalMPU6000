#include <stdint.h>

#include "gpio.h"
#include "cubePinMapping.h"
#include "config.h"

char shape_cs_mappings[NUM_FACES] = { CS1, CS2, CS3, CS4, CS5, CS6 };

void shape_set_cs()
{		
	gpio_function_select(CS1, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS2, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS3, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS4, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS5, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS6, GPIO_FUNC_OUTPUT);
	
	gpio_set_output_level(CS1, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS2, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS3, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS4, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS5, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS6, GPIO_OUTPUT_HIGH);	
}
