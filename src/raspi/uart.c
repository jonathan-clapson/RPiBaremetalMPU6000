#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "support.h"
#include "gpio.h"

#include "mpu60x0.h"

void uart_init(void)
{
	//enable 
	PUT32(AUX_ENABLES, AUX_ENABLES_MINIUART_EN);
	PUT32(AUX_MU_IER_REG,0);
	PUT32(AUX_MU_CNTL_REG,0);
	
    PUT32(AUX_MU_LCR_REG,AUX_MU_LCR_REG_8BITMODE);
    
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    
    PUT32(AUX_MU_IIR_REG,
		AUX_MU_IIR_REG_EN_FIFOS |
		AUX_MU_IIR_REG_CLR_TX_FIFO |
		AUX_MU_IIR_REG_CLR_RX_FIFO);
		
    PUT32(AUX_MU_BAUD_REG, 270);
    
    /* set GP14 to alt5 */
    gpio_function_select(GPIO_UART0_TXD, GPIO_FUNC_ALT5);
    gpio_function_select(GPIO_UART0_RXD, GPIO_FUNC_ALT5);
    
    /* disable pullups/pulldowns on txd and rxd pins */
    unsigned int ra;
    
    PUT32(GPPUD, GPPUD_DISABLEPULLUPDOWN);
    for(ra=0;ra<150;ra++) dummy(ra);
    
    PUT32(GPPUDCLK0, GPPUDCLK_CLK14|GPPUDCLK_CLK15);
    for(ra=0;ra<150;ra++) dummy(ra);    
    PUT32(GPPUDCLK0, GPPUDCLK_PUDCLK_RESET);
    
    PUT32(AUX_MU_CNTL_REG,0x2|0x1);	
}

void uart_putc ( char c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}

int uart_getc(char *c)
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

void uart_get_line(char *buf, size_t len)
{
	int i;
	for (i=0; i<len-1; i++)
	{
		uart_puts("loop\n");
		//block until get a character
		while (uart_getc(buf+i) != 0);
		
		//strip line endings, end string on \n
		if (buf[i] == '\r')
			i--;
		if (buf[i] == '\n')
			break;
	}
	//make it a cstring
	buf[i] = '\0';
}

void uart_puts(char *string)
{
	for (; *string != '\0'; string++)
	{
		uart_putc(*string);
	}	
}

void uart_putbuf(char *buf, int count)
{
	for (int i=0; i<count; i++)
		uart_putc(*buf++);
}

void uart_put_readings(struct reading_memory_type *reading_memory, int num_faces)
{
	char chunk[500];
	
	uart_putc(0x02); //start of text (reading)
	uart_putc(num_faces);
	
	for (int i=0; i<sizeof(reading_memory); i++)
	{
		char* cur_values = (char*) reading_memory;
		sprintf(chunk, "%02X", cur_values[i]);
		uart_puts(chunk);
	}
	uart_putc(0x03); //end of text (reading)
	uart_putc('\r');
	uart_putc('\n');	
}
