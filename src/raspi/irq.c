
/* while this irq abstraction shows signs of the basic pending irq's, handling of these has not been implemented */
volatile unsigned int *IRQBASE 			=	(unsigned int*) 0x2000B000;
volatile unsigned int *IRQ_BASICPENDING = 	(unsigned int*) 0x2000B200;
volatile unsigned int *IRQ_PENDING1 	=	(unsigned int*) 0x2000B204;
volatile unsigned int *IRQ_FIQ_CONTROL	=	(unsigned int*) 0x2000B20C;
volatile unsigned int *IRQ_IRQ1ENABLE	=	(unsigned int*) 0x2000B210;
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
	
	PUT32(f_edge_reg, pin);
	
	return 0;	
}

int enable_irq(int irq)
{
	//if the irq does not exist return invalid
	if ( irq > IRQ_TOTAL-1 )
		return -1;
	volatile unsigned int* irq_reg = IRQ_IRQ1ENABLE + (irq/IRQ_BITS_PER_REG)*4; //select irq register based on irq number
	unsigned int irq_bit = 1<<(irq%IRQ_BITS_PER_REG);
	PUT32(irq_reg, irq_bit);	
	
	return 0;
}

#define NULL 0

void (*irq_handlers[64]) ( void ) = {
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL, /* IRQ10, IRQ11 */
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL, 
	NULL,	NULL, /* IRQ20, IRQ21 */
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL, /* IRQ28, AUX_INT */
	NULL,	NULL, /* IRQ30, IRQ31 */
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL, /* IRQ40, IRQ41 */
	NULL,	NULL, /* IRQ42, I2C_SPI_SLV_INT */
	NULL,	NULL, /* IRQ44, PWA0 */
	NULL,	NULL, /* PWA1, IRQ47 */
	NULL,	NULL, /* SMI, GPIO_INT0 */
	NULL,	NULL, /* GPIO_INT1, GPIO_INT2 */
	NULL,	NULL, /* GPIO_INT3, I2C_INT */
	NULL,	NULL, /* SPI_INT, PCM_INT */
	NULL,	NULL, /* IRQ56, UART_INT */
	NULL,	NULL,
	NULL,	NULL,
	NULL,	NULL
};

void irq_handler ( void )
{
	unsigned int reg, rc;
	while(1)
	{
		int offset = 0;
		reg = GET32(IRQ_PENDING1);
		//determine which bit is set, hand over to irq handler
		if (reg) {
			//call irq handler
			
		}
		offset++;
		reg = GET32(IRQ_PENDING1 + 4);
		//determine which bit is set, hand over to irq handler
	}	
}
