/*!	\file uart.c
 * 	\brief HAL uart functionality
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "support.h"
#include "gpio.h"

#include "mpu60x0.h"

/*!	\fn int calc_baud_num(int baud)
 *	\brief Converts a serial baud rate to the number that must be supplied to the baud register to achieve that rate
 * 
 *	@param[in] baud The desired baud rate
 * 	\return value to write to the baud rate register
 */
int calc_baud_num(int baud)
{
	const int SYSCLK_FREQ = 250000000;	
	
	int regval = SYSCLK_FREQ/((8*baud) -1);
	
	return regval;
}

/*!	\fn void uart_init(void)
 *	\brief Initialises the UART0 hardware
 */
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
		
    PUT32(AUX_MU_BAUD_REG, calc_baud_num(2000000));
    
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

/*!	\fn void uart_putc ( char c )
 *	\brief Send a character through UART0
 *	@param[in] c 8 bit value to send using the uart device
 */
void uart_putc ( char c )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    PUT32(AUX_MU_IO_REG,c);
}

/*!	\fn void uart_putc ( char c )
 *	\brief Receive a character through UART0 (Non-blocking)
 *	@param[in] c pointer to an 8 bit location to store the value received from the uart device
 * 	\return 0 on success, -1 on failure
 * 	\todo make this use the codes that should be defined in error.h
 */
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

/*!	\fn void uart_get_line(char *buf, size_t len)
 *	\brief Receive a line from uart (Blocking read)
 * 	
 * 	Reads data from uart0, storing in incremental locations starting from buf. Stops when '\n' is received or when len bytes have been read.
 *	@param[in] buf pointer to a memory location to write to. Must be able to hold at least len bytes.
 * 	@param[in] len the maximum number of bytes to read
 */
void uart_get_line(char *buf, size_t len)
{
	int i;
	for (i=0; i<len-1; i++)
	{
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

/*!	\fn void void uart_puts(char *string)
 *	\brief Writes a c string to the uart device (blocking)
 *	@param[in] string pointer to a c string to send through uart
 */
void uart_puts(char *string)
{
	for (; *string != '\0'; string++)
	{
		uart_putc(*string);
	}	
}

/*!	\fn void uart_putbuf(char *buf, int count)
 *	\brief Writes len characters stored in buf
 *	@param[in] buf data set to send through uart
 * 	@param[in] count number of bytes to send
 */
void uart_putbuf(char *buf, int count)
{
	for (int i=0; i<count; i++)
		uart_putc(*buf++);
}

/*!	\fn void uart_put_readings(struct reading_memory_type *reading_memory, int num_faces)
 *	\brief Writes a set of readings to the uart device, via the protocol which is described in the final report
 *	@param[in] reading_memory a set of readings to be sent using the protocol
 * 	@param[in] the number of readings in the set
 */
void uart_put_readings(struct reading_memory_type *reading_memory, int num_faces)
{
	char chunk[500];
	
	uart_putc(0x02); //start of text (reading)
	uart_putc(num_faces);

	for (int i=0; i<sizeof(struct reading_memory_type)*num_faces; i++)
	{
		char* cur_values = (char*) reading_memory;
		sprintf(chunk, "%02X", cur_values[i]);
		uart_puts(chunk);
	}
	uart_putc(0x03); //end of text (reading)
	uart_putc('\r');
	uart_putc('\n');	
}
