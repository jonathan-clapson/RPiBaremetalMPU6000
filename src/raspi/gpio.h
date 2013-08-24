#ifndef __GPIO_H__
#define __GPIO_H__

/* GPIO REGS */
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
  GPIO_FUNC_ALT5 = 2
} FSEL;
#define NUM_GPIO_FUNCS 8

#define NUM_GPIO_PINS 54

#define GPIO_UART0_TXD 14
#define GPIO_UART0_RXD 15

#define GPIO_SPI0_CS1	7
#define GPIO_SPI0_CS0	8
#define GPIO_SPI0_MISO	9
#define GPIO_SPI0_MOSI	10
#define GPIO_SPI0_SCK	11

int gpio_function_select(unsigned int pin, FSEL sel);

#endif /* __GPIO_H__ */
