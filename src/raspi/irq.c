#include "uart.h"
#include <string.h>

extern void enable_irq ( void );

/* while this irq abstraction shows signs of the basic pending irq's, handling of these has not been implemented */
volatile unsigned int *IRQBASE 			=	(unsigned int*) 0x2000B000;
volatile unsigned int *IRQ_BASICPENDING = 	(unsigned int*) 0x2000B200;
volatile unsigned int *IRQ_PENDING1 	=	(unsigned int*) 0x2000B204;
volatile unsigned int *IRQ_PENDING2 	=	(unsigned int*) 0x2000B208;
volatile unsigned int *IRQ_FIQ_CONTROL	=	(unsigned int*) 0x2000B20C;
volatile unsigned int *IRQ_IRQ1ENABLE	=	(unsigned int*) 0x2000B210;
volatile unsigned int *IRQ_IRQ2ENABLE	=	(unsigned int*) 0x2000B214;
volatile unsigned int *IRQ_BASICENABLE	=	(unsigned int*) 0x2000B218;
volatile unsigned int *IRQ_IRQ1DISABLE	=	(unsigned int*) 0x2000B21C;
volatile unsigned int *IRQ_BASICDISABLE	=	(unsigned int*) 0x2000B224;
unsigned int IRQ_TOTAL = 64;

/* GPIO Interrupts */
volatile unsigned int GPIO_INT0 = 49;
volatile unsigned int GPIO_INT1 = 50;
volatile unsigned int GPIO_INT2 = 51;
volatile unsigned int GPIO_INT3 = 52;
volatile unsigned int *GPIO_FEN0 = (unsigned int *) 0x20200058;

const int GPIO_TOTAL = 54;
const int IRQ_BITS_PER_REG = 32;

int irq_gpio_falling_edge_en ( int pin )
{
	//if the pin does not exist return invalid
	if ( pin > GPIO_TOTAL-1 )
		return -1;
		
	volatile unsigned int* f_edge_reg = GPIO_FEN0 + (pin/IRQ_BITS_PER_REG)*4;
	pin = pin%IRQ_BITS_PER_REG;
	
	PUT32( f_edge_reg, 1<<pin );
	
	return 0;	
}

int enable_interrupt_for_irq(int irq)
{
	//if the irq does not exist return invalid
	if ( irq > IRQ_TOTAL-1 )
		return -1;
	
	//select irq register based on irq number
	volatile unsigned int* irq_reg = IRQ_IRQ1ENABLE + (irq/IRQ_BITS_PER_REG)*4; 
	unsigned int irq_bit = 1<<(irq%IRQ_BITS_PER_REG);
	PUT32(irq_reg, irq_bit);	
	
	return 0;
}

void (*irq_handlers[64]) ( void ); 

void c_irq_handler ( void )
{
	unsigned int reg;
	
	uart_puts("irq_entered!\n");
	
	//hand off to handlers associated with irq pending1
	while( (reg = GET32(IRQ_PENDING1)) != 0 ) {		
		for (int i=0; i<IRQ_BITS_PER_REG; i++) {
			if ( (reg&(1<<i)) && irq_handlers[i] ) {
				uart_puts("irq1!\n");
				irq_handlers[i]();
				reg = GET32(IRQ_PENDING1);
			}
		}
	}
	//hand off to handlers associated with irq pending2
	while( (reg = GET32(IRQ_PENDING2)) != 0 ) {
		for (int i=0; i<IRQ_BITS_PER_REG; i++){
			if (reg&(1<<i) && irq_handlers[i+IRQ_BITS_PER_REG] ) {
				uart_puts("irq2!\n");
				irq_handlers[i+IRQ_BITS_PER_REG]();
				reg = GET32(IRQ_PENDING1);
			}
		}
	}
}

int register_irq_handler ( int irq, void (*irq_handler_func)(void) )
{
	//if irq is invalid, fail
	if (irq > IRQ_TOTAL)
		return -1;
	
	if (irq_handlers[irq] != NULL)
		return -1;
		
	irq_handlers[irq] = irq_handler_func;
	
	return 0;	
}

void c_enable_irq( void )
{  
    memset(irq_handlers, 0, sizeof(irq_handlers));    
    enable_irq();
}
