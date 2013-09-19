#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <irq.h>

#include "raspi.h"
#include "mpu60x0.h"
#include "uart.h"
#include "support.h"
#include "spi.h"
#include "gpio.h"
#include "vector.h"
#include "calibrate.h"
#include "integrate.h"
#include "main_readings.h"

#include "config.h"

#include <math.h>

int flag; 

void gpio_irq ( void )
{
	volatile unsigned int *GPEDS0 =	(unsigned int*) 0x20200040;
	volatile unsigned int *GPEDS1 =	(unsigned int*) 0x20200044;

	flag = 1;
	
	*GPEDS0 = 0xFFFFFFFF;
	*GPEDS1 = 0xFFFFFFFF;
	uart_puts("interrupt triggered\n");
}

void gpio_set_interrupts()
{
	/* set interrupt pins as input */
	gpio_function_select(INT1, GPIO_FUNC_INPUT);
	gpio_function_select(INT2, GPIO_FUNC_INPUT);
	gpio_function_select(INT3, GPIO_FUNC_INPUT);
	gpio_function_select(INT4, GPIO_FUNC_INPUT);
	gpio_function_select(INT5, GPIO_FUNC_INPUT);
	gpio_function_select(INT6, GPIO_FUNC_INPUT);
	
	/* enable falling edge detection */
	irq_gpio_falling_edge_en(INT1);
	irq_gpio_falling_edge_en(INT2);
	irq_gpio_falling_edge_en(INT3);
	irq_gpio_falling_edge_en(INT4);
	irq_gpio_falling_edge_en(INT5);
	irq_gpio_falling_edge_en(INT6);
}

/* Mode select on powerup 
 * currently only supports numbers from 0 to 9
 */
int select_mode()
{
	char menu_select[3] = {0};
	
	while (menu_select[0] > '9' || menu_select[0] < '0')
	{
		//FIXME: should clear uart rxbuf here
		uart_puts("select mode\r\n(0)get readings\r\n(1) calibration\r\n(2) run\r\n");
		
		uart_get_line(menu_select, sizeof(menu_select));
		uart_puts(menu_select);		
	}	
	return menu_select[0] - '0';
}

//extern void enable_interrupts (void);

__attribute__((no_instrument_function)) void not_main(void)
{	
	/* board initialisation */
	uart_init();
	spi_pin_init();
	
	flag = 0;
	
	struct mpu60x0_stateType mpu60x0_state[NUM_FACES];
	for (int i=0; i<NUM_FACES; i++)
	{
		mpu60x0_state[i].gyro_rate = INV_MPU60x0_FSR_250DPS;
		mpu60x0_state[i].accel_rate = INV_MPU60x0_FS_02G;
		mpu60x0_state[i].filter_cutoff = INV_MPU60x0_FILTER_256HZ_NOLPF2;
	}
	
	int init_failed = 0;
	/* initialise all mpu 6000 boards */
	for (int i=0; i<NUM_FACES; i++)
	{
		if (!mpu60x0_init(shape_cs_mappings[i], &(mpu60x0_state[i])))
		{
			init_failed = 1;
			printf("MPUDev: %d failed\n", i);
		}
	}
	if (init_failed)
		return;
		
	//Configure SD Status LED for output
	gpio_function_select(16, GPIO_FUNC_OUTPUT);
	
	int mode = select_mode();
	printf("mode is: %d\n", mode);	
	
	/* interrupt pin config */
	gpio_set_interrupts();
	
	c_enable_irq();
	
	//assign function to handle gpio_irqs
	register_irq_handler ( GPIO_INT0, gpio_irq );
	register_irq_handler ( GPIO_INT1, gpio_irq );
	register_irq_handler ( GPIO_INT2, gpio_irq );
	register_irq_handler ( GPIO_INT3, gpio_irq );
	
	//enable irq handling on gpio interrupts
	enable_interrupt_for_irq(GPIO_INT0);
	enable_interrupt_for_irq(GPIO_INT1);
	enable_interrupt_for_irq(GPIO_INT2);
	enable_interrupt_for_irq(GPIO_INT3);
	
	flag = 0;
	
	unsigned int prevTs = 0;
	GPIO_OUTPUT_LEVEL prevLevel = GPIO_OUTPUT_HIGH;
	
	switch (mode)
	{
		case 0:
			main_readings(mpu60x0_state);
			return;
		case 1:
			static_calibration(mpu60x0_state);
			return;		
		case 2:
			main_debug(mpu60x0_state);
			return;
		default:
			printf("invalid mode\n");
			return;
	}
}
