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
#include "cubePinMapping.h"

#include <math.h>

int flag = 0; 

void gpio_irq ( void )
{
	volatile unsigned int *GPEDS0 =	(unsigned int*) 0x20200040;
	volatile unsigned int *GPEDS1 =	(unsigned int*) 0x20200044;

	flag = 1;
	
	*GPEDS0 = 0xFFFFFFFF;
	*GPEDS1 = 0xFFFFFFFF;
}

void gpio_set_interrupts()
{
	gpio_function_select(INT1, GPIO_FUNC_INPUT);
	gpio_function_select(INT2, GPIO_FUNC_INPUT);
	gpio_function_select(INT3, GPIO_FUNC_INPUT);
	gpio_function_select(INT4, GPIO_FUNC_INPUT);
	gpio_function_select(INT5, GPIO_FUNC_INPUT);
	gpio_function_select(INT6, GPIO_FUNC_INPUT);
}

extern void enable_interrupts (void);

__attribute__((no_instrument_function)) void not_main(void)
{	
	/* board initialisation */
	uart_init();
	spi_pin_init();
	
	struct mpu60x0_stateType mpu60x0_state[NUM_FACES];
	
	/* initialise all mpu 6000 boards */
	for (int i=0; i<sizeof(cs_mappings); i++)
		mpu60x0_init(cs_mappings[i], &(mpu60x0_state[i]));
	
	//Configure SD Status LED for output
	gpio_function_select(16, GPIO_FUNC_OUTPUT);
	
	/* interrupt pin config */
	
	//c_enable_irq();
	
	//assign function to handle gpio_irqs
	//register_irq_handler ( GPIO_INT0, gpio_irq );
	//register_irq_handler ( GPIO_INT1, gpio_irq );
	//register_irq_handler ( GPIO_INT2, gpio_irq );
	//register_irq_handler ( GPIO_INT3, gpio_irq );
		
	/* enable falling edge detection on irq 17 */
	//irq_gpio_falling_edge_en(17);
	
	//enable irq handling on gpio interrupts
	//enable_interrupt_for_irq(GPIO_INT0);
	//enable_interrupt_for_irq(GPIO_INT1);
	//enable_interrupt_for_irq(GPIO_INT2);
	//enable_interrupt_for_irq(GPIO_INT3);
	
	flag = 0;
	
	while(1){
		struct readingsType reading[NUM_FACES];
		
		//if (flag)
		if (1)
		{
			
			//flag = 0;
			
			for (int i=0; i<NUM_FACES; i++)
			{
				mpu60x0_get_reading( cs_mappings[i], mpu60x0_state[i], &(reading[i]) );
				
				char tmp[100];			
				sprintf(tmp, "dev: %d, xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
					i, reading[i].accelX, reading[i].accelY, reading[i].accelZ, reading[i].temp, reading[i].gyroX, reading[i].gyroY, reading[i].gyroZ);
			
				uart_puts((unsigned char*) tmp);
			}
			
			/*char tmp[100];
			sprintf(tmp, "xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
				reading.accelX, reading.accelY, reading.accelZ, reading.temp, reading.gyroX, reading.gyroY, reading.gyroZ);
			
			uart_puts((unsigned char*) tmp);*/
			
			
						
			/*char chunk[500];
			
			uart_putc(0x02); //start of text (reading)
			
			for (int i =0; i<sizeof(reading); i++)
			{
				sprintf(chunk, "%02X", ((char *) &reading)[i]);
				uart_puts(chunk);
			}
			uart_putc(0x03); //start of text (reading)
			uart_putc('\r');
			uart_putc('\n');*/
			
			/*gpio_set_output_level(16, GPIO_OUTPUT_HIGH);
			wait(1000*1000);
			gpio_set_output_level(16, GPIO_OUTPUT_LOW);
			wait(1000*1000);*/
						
		}
		
		wait(1); //WTF???!!!
	}
}
