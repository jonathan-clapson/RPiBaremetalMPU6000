/*!	\file irq.c
 * 	\brief HAL IRQ handling
 */


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

/*!	\fn int irq_gpio_falling_edge_en ( int pin )
 *	\brief Create irq's on GPIO falling edge events
 * 
 * 	Hardware detection has been provided for detecing when a GPIO configured as input has its logical value fall from high to low. This function enables that detection on a given GPIO pin.
 *	@param[in] pin the GPIO pin to enable detection on. Pin numbering is that described in http://elinux.org/RPi_Low-level_peripherals or in the BCM2708/BCM2835 documentation.
 * 	\return On success 0. On failure -1.
 * 	\todo make this conform to error.h
 */
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

/*!	\fn int enable_interrupt_for_irq(int irq)
 *	\brief The raspberry pi's processer can detect irq's from a variety of sources. This function enables requested irq's.
 * 
 *	@param[in] irq The irq to enable. Check BCM2708/BCM2835 docs for details.
 * 	\return 0 if the irq has been successfully set. -1 otherwise
 * 	\todo make this conform to error.h
 */
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

/*!	\var void (*irq_handlers[64]) ( void ); 
 *	\brief Table of interrupt callback functions.
 */
void (*irq_handlers[64]) ( void ); 

/*!	\fn void c_irq_handler ( void )
 *	\brief Main IRQ handling routine.
 * 	This routine is called from the assembly code in vectors.s when an interrupt occurs. It handles interrupts from two interrupt pending banks.
 * 	
 * 	\todo since moving to a multiple sensors something about this has got a little bit messed up, ie it doesn't seem to get called. Need to look into this :S
 */
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

/*!	\fn int register_irq_handler ( int irq, void (*irq_handler_func)(void) )
 *	\brief Registers an irq_handler with the irq_handlers list.
 * 
 * 	This function is used by programs to provide a callback function for certain interrupts. Remember to clear interrupts from the respective hardware device otherwise the interrupt handler will be called continuously.
 *	@param[in] irq the irq this handler is designed to be a callback for.
 * 	@param[in] irq_handler_func a function pointer to a callback function
 * 	\return 0 on success. -1 on invalid irq or handler already assigned
 * 	\todo integrate with error.h, create a function to deallocate interrupt handlers
 */
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

/*!	\fn void c_enable_irq( void )
 *	\brief Enable irq handling. Without calling this function, all interrupts are ignored.
 * 
 * 	This function initialises the irq_handlers list and then calls the assembly routine to enable interrupts. (Defined in vectors.s) 
 */
void c_enable_irq( void )
{  
    memset(irq_handlers, 0, sizeof(irq_handlers));    
    enable_irq();
}
