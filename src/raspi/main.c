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

int flag = 0; 

void gpio_irq ( void )
{
	volatile unsigned int *GPEDS0 =	(unsigned int*) 0x20200040;
	volatile unsigned int *GPEDS1 =	(unsigned int*) 0x20200044;

	flag = 1;
	
	*GPEDS0 = 0xFFFFFFFF;
	*GPEDS1 = 0xFFFFFFFF;
}

extern void enable_interrupts (void);

__attribute__((no_instrument_function)) void not_main(void)
{	
	/* board initialisation */
	uart_init();
	spi_pin_init();
	
	struct mpu60x0_stateType mpu60x0_state;
	
	mpu60x0_init(&mpu60x0_state);
	
	UINT32 sel = READ32(GPFSEL1);
	sel &= ~(0b111 << 18);
	sel |= (0b001 << 18);
	WRITE32(GPFSEL1,sel);

	gpio_function_select(17, GPIO_FUNC_INPUT);
	
	c_enable_irq();
	
	//assign function to handle gpio_irqs
	register_irq_handler ( GPIO_INT0, gpio_irq );
	register_irq_handler ( GPIO_INT1, gpio_irq );
	register_irq_handler ( GPIO_INT2, gpio_irq );
	register_irq_handler ( GPIO_INT3, gpio_irq );
		
	//enable falling edge detection on irq 17
	irq_gpio_falling_edge_en(17);
	
	//enable irq handling on gpio interrupts
	enable_interrupt_for_irq(GPIO_INT0);
	enable_interrupt_for_irq(GPIO_INT1);
	enable_interrupt_for_irq(GPIO_INT2);
	enable_interrupt_for_irq(GPIO_INT3);
	
	flag = 0;
	
	while(1){
		struct readingsType reading;
				
		PUT32(GPCLR0, 1<<16);
		wait(1000*80);
		PUT32(GPSET0, 1<<16);
		wait(1000*80);
		
		//if (flag)
		if (1)
		{
			flag = 0;
			
			mpu60x0_get_reading(mpu60x0_state, &reading);
			
			char tmp[100];
			sprintf(tmp, "xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
				reading.accelX, reading.accelY, reading.accelZ, reading.temp, reading.gyroX, reading.gyroY, reading.gyroZ);
			
			uart_puts((unsigned char*) tmp); 
			
			struct readingsType dummy;
			dummy.accelX = 1;
			dummy.accelY = 2;
			dummy.accelZ = 3;
			dummy.temp = 4;
			dummy.gyroX = 5;
			dummy.gyroY = 6;
			dummy.gyroZ = 7;
			
			char buf[5000];
			char chunk[500];
			buf[0] = '\0';
			
			printf("valid hex:");			
			for (int i =0; i<sizeof(dummy); i++)
			{
			/*	if (!(i%8))
				{
					uart_puts(buf);
					uart_putc('\n');
					buf[0] = '\0';
				}*/
				sprintf(chunk, "%2X", ((char *) &dummy)[i]);
				strcat(buf, chunk);
			}
			uart_putc('\r');
			uart_putc('\n');
						
			int i;
			for (i=0; i<sizeof(dummy); i++)
			{
				buf[i] = ((char *) &dummy)[i];
			}
			buf[i] = '\r';
			buf[i+1] = '\n';
			buf[i+2] = '\0';
						
			printf("sizeof dummy is: %d ", sizeof(dummy));
			printf("writing %d bytes\r\n", strlen(buf));
			uart_putbuf(buf, strlen(buf));
			//uart_puts((unsigned char*) buf); 
			printf("here3\r\n");
			
			//uart_putbuf((unsigned char*) &reading, sizeof(reading));
			//uart_putc('\n');
		}
		
		wait(1); //WTF???!!!
	}
}
