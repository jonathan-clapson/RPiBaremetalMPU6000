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
		uart_puts("select mode\r\n(1) calibration\r\n(2) run\r\n");
		
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
	
	struct reading_memory_type reading_memory[NUM_FACES] = {{0}}; /* = malloc(read_rate*read_time*sizeof(struct reading_memory_type)*NUM_FACES); */

	struct mpu60x0_stateType mpu60x0_state[NUM_FACES];
	
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
	/*if (init_failed)
		return;*/
		
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
	
	switch (mode)
	{
		case 1:
			static_calibration(mpu60x0_state);
			return;
		default:
			break;
	}
	
	while(1){
		//if ( falling edge detected say so )
		if (flag)
		{
			uart_puts("got int!\n");
		}
		if (1)
		{			
			//flag = 0;
			
			
			for (int i=0; i<NUM_FACES; i++)
			{
				mpu60x0_get_reading( shape_cs_mappings[i], mpu60x0_state[i], &(reading_memory[i]) );

				struct vector* result = NULL;
				struct vector* values = NULL;
				
				/* get velocity update */
				result = (struct vector *) &(reading_memory[i].v_x);
				values = (struct vector *) &(reading_memory[i].a_x);
				integrate_vector(result, values);
				
				/* get position update */
				result = (struct vector *) &(reading_memory[i].x);
				values = (struct vector *) &(reading_memory[i].v_x);
				integrate_vector(result, values);
				
				/* get orientation update */
				result = (struct vector *) &(reading_memory[i].o_x);
				values = (struct vector *) &(reading_memory[i].w_x);
				integrate_vector(result, values);
				
				/*char tmp[100];			
				sprintf(tmp, "dev: %d, xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
					i, reading_memory[i].a_x, reading_memory[i].a_y, reading_memory[i].a_z, reading_memory[i].temp, \
					reading_memory[i].w_x, reading_memory[i].w_y, reading_memory[i].w_z);
			
				uart_puts((unsigned char*) tmp);*/
			}
			
			/*char tmp[100];
			sprintf(tmp, "xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
				reading.accelX, reading.accelY, reading.accelZ, reading.temp, reading.gyroX, reading.gyroY, reading.gyroZ);
			
			uart_puts((unsigned char*) tmp);*/
					
			char chunk[500];
			
			uart_putc(0x02); //start of text (reading)
			uart_putc(NUM_FACES);
			
			for (int i=0; i<sizeof(reading_memory); i++)
			{
				char* cur_values = (char*) reading_memory;
				sprintf(chunk, "%02X", cur_values[i]);
				uart_puts(chunk);
			}
			uart_putc(0x03); //end of text (reading)
			uart_putc('\r');
			uart_putc('\n');
			
			/*gpio_set_output_level(16, GPIO_OUTPUT_HIGH);
			wait(1000*1000);
			gpio_set_output_level(16, GPIO_OUTPUT_LOW);
			wait(1000*1000);*/
		}
		
		wait(1); //WTF???!!!
	}
}
