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

#define CS 8
#define SCK 11
#define MISO 9
#define MOSI 10

typedef enum{
  INPUT = 0,
  OUTPUT = 1,
  ALT0 = 2,
  ALT1 = 3,
  ALT2 = 4,
  ALT3 = 5,
  ALT4 = 6,
  ALT5 = 7
} FSEL;


void GPIO_FSEL(unsigned int pin, FSEL sel)
{
	volatile unsigned int* GPIO_BASE = (unsigned int *)0x20200000;
	unsigned int curState = 0;

	// get right address for pin
	if(pin >= 10)
		GPIO_BASE++;
	if(pin >= 20)
		GPIO_BASE++;
	if(pin >= 30)
		GPIO_BASE++;
	if(pin >= 40)
		GPIO_BASE++;
	if(pin >= 50)
		GPIO_BASE++;

	pin %= 10; // we can set 10 pins per register

	curState = *(GPIO_BASE);       // get GPIO reg state

	curState &= ~(7 << (pin * 3)); // clear bits in question
	curState |= sel << (pin * 3);  // set bits in question

	*(GPIO_BASE) = curState;       // write state back to GPIO reg
}

int flag = 0; 

void gpio_irq ( void )
{
	volatile unsigned int *GPEDS0 =	(unsigned int*) 0x20200040;
	volatile unsigned int *GPEDS1 =	(unsigned int*) 0x20200044;
	
	printf("gpio_irq\n\r");

	flag = 1;
	
	//PUT32(GPEDS0, 0);
	//PUT32(GPEDS1, 0);
	*GPEDS0 = 0xFFFFFFFF;
	*GPEDS1 = 0xFFFFFFFF;
}

/********************************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date      : 2012

 version   :

**********************************************************************************************************************************************************/

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


	//set gpio 17 as input
	GPIO_FSEL(17, 0);
	
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
		
		mpu60x0_get_reading(mpu60x0_state, &reading);			
		
		char tmp[100];
		sprintf(tmp, "xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
			reading.accelX, reading.accelY, reading.accelZ, reading.temp, reading.gyroX, reading.gyroY, reading.gyroZ);
		/*uart_puts((unsigned char*) tmp);*/
				
		PUT32(GPCLR0, 1<<16);
		wait(1000*80);
		PUT32(GPSET0, 1<<16);
		wait(1000*80);
		
		volatile unsigned int *var = (unsigned int*) 0x20200034;
		int vari = *var;
		if (vari & (1<<17))
		{
			uart_puts("pinHigh!\n\r");
		}
		
		volatile unsigned int *GPEDS0 = (unsigned int*) 0x20200040;
		volatile unsigned int *GPEDS1 =	(unsigned int*) 0x20200044;
		unsigned int reg = *GPEDS0;
		if (reg != 0)
		{
			printf("polled! %x\n\r", reg);
			*GPEDS0 = 0xFFFFFFFF;
		}
		if (flag)
		{
			flag = 0;
			printf("got irq!\n\r");
		}
		wait(1000*100);
	}
}
