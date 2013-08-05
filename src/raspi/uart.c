#include "uart.h"
#include "spi.h"
#include "support.h"

void uart_init(void)
{
	//enable 
	PUT32(AUX_ENABLES, AUX_ENABLES_MINIUART_EN);
	PUT32(AUX_MU_IER_REG,0);
	PUT32(AUX_MU_CNTL_REG,0);
	
    PUT32(AUX_MU_LCR_REG,AUX_MU_LCR_REG_8BITMODE);
    
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    /*PUT32(AUX_MU_IIR_REG,
		AUX_MU_IIR_REG_EN_FIFOS |
		AUX_MU_IIR_REG_CLR_TX_FIFO |
		AUX_MU_IIR_REG_CLR_RX_FIFO);*/
		
	PUT32(AUX_MU_IIR_REG, 0xc6);
		
    PUT32(AUX_MU_BAUD_REG, 270);
    
    /* set GP14 to alt5 */
    unsigned int ra;
    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //clear GPIO14 bits
    ra|=2<<12;    //set alt5 on GPIO14
    PUT32(GPFSEL1,ra);
    
    /* set GP15 to alt5 */  
    ra=GET32(GPFSEL1);
    ra&=~(7<<15); //clear GPIO15 bits
    ra|=2<<15;    //set alt5 on GPIO15
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD, GPPUD_DISABLEPULLUPDOWN);
    for(ra=0;ra<150;ra++) dummy(ra);
    
    PUT32(GPPUDCLK0, GPPUDCLK_CLK14|GPPUDCLK_CLK15);
    for(ra=0;ra<150;ra++) dummy(ra);    
    PUT32(GPPUDCLK0, GPPUDCLK_PUDCLK_RESET);
    
    PUT32(AUX_MU_CNTL_REG,0x2|0x1);	
}

void uart_putc(uint8_t c)
{
	while(1) { 
		uint32_t lsr_reg = GET32(AUX_MU_LSR_REG);
		if (lsr_reg&AUX_MU_LSR_REG_TX_EMPTY) break; 
	}
	PUT32(AUX_MU_IO_REG, c);
}

int uart_getc(uint8_t *c)
{
	/* if data is available return it, otherwise indicate failure */
	uint32_t lsr_reg = GET32(AUX_MU_LSR_REG);
	if (lsr_reg&AUX_MU_LSR_REG_RX_NOT_EMPTY)
	{
		*c = GET32(AUX_MU_IO_REG);
		return 0;
	}
	return -1;
}

void uart_puts(uint8_t *string)
{
	for (; *string != '\0'; string++)
	{
		uart_putc(*string);
	}	
}
