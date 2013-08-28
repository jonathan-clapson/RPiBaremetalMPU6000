#include "spi.h"
#include "support.h"
#include "gpio.h"

/* SPI0 Registers */
volatile unsigned int *SPI0_CONTROL =  (unsigned int *) 0x20204000;
volatile unsigned int *SPI0_FIFO    =  (unsigned int *) 0x20204004;
volatile unsigned int *SPI0_CLK     =  (unsigned int *) 0x20204008;
volatile unsigned int *SPI0_DLEN    =  (unsigned int *) 0x2020400C;
volatile unsigned int *SPI0_LTOH    =  (unsigned int *) 0x20204010;
volatile unsigned int *SPI0_DC      =  (unsigned int *) 0x20204014;

/* SPI_CONTROL Bitfields */
#define SPI_C_LEN_LONG	25
#define SPI_C_DMA_LEN	24
#define SPI_C_CSPOL2	23
#define SPI_C_CSPOL1	22
#define SPI_C_CSPOL0	21
#define SPI_C_RX		20
#define SPI_C_RXR		19
#define SPI_C_TXD		18
#define SPI_C_RXD		17
#define SPI_C_DONE		16
#define SPI_C_LEN		13
#define SPI_C_REN		12
#define SPI_C_ADCS		11
#define SPI_C_INTR		10
#define SPI_C_INTD		9
#define SPI_C_DMAEN		8
#define SPI_C_TA		7
#define SPI_C_CSPOL		6
#define SPI_C_CLEAR_RX	5
#define SPI_C_CLEAR_TX	4
#define SPI_C_CPOL		3
#define SPI_C_CPHA		2
#define SPI_C_CS1		1
#define SPI_C_CS0		0

uint8_t spi_read(uint8_t reg) 
{
	uint8_t ret;
	spi_begin();
	spi_transfer(reg|0x80);
	ret = spi_transfer(0);
	spi_end();
	return ret;
}

void spi_write(uint8_t reg, uint8_t data)
{
	spi_begin();
	spi_transfer(reg);
	spi_transfer(data);
	spi_end();
}

void spi_pin_init(void)
{
	unsigned int var;
	
	/* Set gpios to spi mode */
	gpio_function_select(GPIO_SPI0_MISO, GPIO_FUNC_ALT0);
	gpio_function_select(GPIO_SPI0_MOSI, GPIO_FUNC_ALT0);
	gpio_function_select(GPIO_SPI0_SCK, GPIO_FUNC_ALT0);
	
	/* chip selects are not setup as all chip selects will operate as gpio chip selects */
	//gpio_function_select(GPIO_SPI0_CS0, GPIO_FUNC_ALT0);
	//gpio_function_select(GPIO_SPI0_CS1, GPIO_FUNC_ALT0);
	
	/* clear control register */
	*SPI0_CONTROL = 0;
	
	/* Set spi clock to 1 MHz */
	*SPI0_CLK = 256;
}
void spi_begin(void)
{	
	unsigned int var;
        	
	/* Clear TX and RX fifos */
	*SPI0_CONTROL |= (1 << SPI_C_CLEAR_RX) | (1 << SPI_C_CLEAR_TX);
	
	/* set clock phase and sampling edge */
	var = *SPI0_CONTROL; 	 	
 	var |= (1<<SPI_C_CPHA);
	var |= (1<<SPI_C_CPOL);
	
	/* set to CS0 and Chip Select Polarity=LOW */
	var &= ~((1 << SPI_C_CS0) | (1 << SPI_C_CS1));
	var &= ~(1 << SPI_C_CSPOL0);
	
	/* Enable transfer on CS0 */
	var|= (1 << SPI_C_TA);

	/* Write back updated value */
	*SPI0_CONTROL = var;
}

unsigned int spi_transfer(unsigned char value)
{
	unsigned int var = 0;
	unsigned int ret = 0;
	var = *SPI0_CONTROL;
	
	/* Clear TX and RX fifos */
	var|= (1 << SPI_C_CLEAR_RX) | (1<<SPI_C_CLEAR_TX);
		
	/* Wait for TXD */
	while (!(*SPI0_CONTROL & (1 << SPI_C_TXD)))
		  wait(100);   
              
	/* Write to TX FIFO */
	*SPI0_FIFO = value;
	
	/* Wait for DONE to be set */
	while (!(*SPI0_CONTROL & (1 << SPI_C_DONE)))
		wait(100); 
		
	/* Read RX FIFO */
	ret = *SPI0_FIFO;

    return ret;
}

void spi_end(void)
{
 	/* Set all the SPI0 pins back to input */ //FIXME: why?
	unsigned int var;

    /* Set transfer to inactive */
    var = *SPI0_CONTROL;
    var &= ~(1 << SPI_C_TA);
	*SPI0_CONTROL =var;	
}
