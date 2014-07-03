/*!	\file gpio.h
 * 	\brief HAL functions for controlling the Raspberry Pi GPIO Hardware
 * 	
 * 	This file contains function signatures and types for using the GPIO Hardware
 */

#ifndef __GPIO_H__
#define __GPIO_H__

/* GPIO REGS */
/* FIXME: these should not need to be exported */
extern volatile unsigned int *GPIO_GPFSEL0;
extern volatile unsigned int *GPIO_GPSET0;
extern volatile unsigned int *GPIO_GPCLR0;
extern volatile unsigned int *GPIO_GPLEV0;

/* GPIO Function Selects */
typedef enum{
  GPIO_FUNC_INPUT = 0,
  GPIO_FUNC_OUTPUT = 1,
  GPIO_FUNC_ALT0 = 4,
  GPIO_FUNC_ALT1 = 5,
  GPIO_FUNC_ALT2 = 6,
  GPIO_FUNC_ALT3 = 7,
  GPIO_FUNC_ALT4 = 3,
  GPIO_FUNC_ALT5 = 2,
  GPIO_NUM_FUNCS = 8
} FSEL;

#define GPIO_NUM_PINS 54

typedef enum {
	GPIO_OUTPUT_LOW = 0,
	GPIO_OUTPUT_HIGH = 1
} GPIO_OUTPUT_LEVEL;

#define GPIO_UART0_TXD 14
#define GPIO_UART0_RXD 15

/* spi related pin mappings */
/*! \defgroup RSPIIDS RPi SPI Hardware GPIO Pins
 * These define pins which must be configured and used to use the Raspberry Pi's internal SPI hardware
 * @{ 
 */
#define GPIO_SPI0_CS1	7
#define GPIO_SPI0_CS0	8
#define GPIO_SPI0_MISO	9
#define GPIO_SPI0_MOSI	10
#define GPIO_SPI0_SCK	11
/* @} */

int gpio_function_select(unsigned int pin, FSEL sel);
int gpio_set_output_level (int pin, GPIO_OUTPUT_LEVEL level);

#endif /* __GPIO_H__ */
