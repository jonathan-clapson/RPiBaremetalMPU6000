/********************************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date	   : 2012

 version   :

**********************************************************************************************************************************************************/

#include "spi.h"
#include "support.h"

/*GPIO PINS
 */
volatile unsigned int *gpioGPFSEL0 =  (unsigned int *) 0x20200000;
volatile unsigned int *gpioGPFSEL1 =  (unsigned int *) 0x20200004;
volatile unsigned int *gpioGPSET0  =  (unsigned int *) 0x2020001C;
volatile unsigned int *gpioGPCLR0  =  (unsigned int *) 0x20200028;
volatile unsigned int *gpioGPLEV0  =  (unsigned int *) 0x20200034;


/*SPI0 ADDRESS
 */
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

/* GPIO FUNCTION SELECT */
#define GPIO_SET_FUNCTION(num, func) *gpioGPFSEL0 = func << (3*num); /* FIXME */
#define GPIO_INPUT		0x0
#define GPIO_OUTPUT		0x1
#define GPIO_ALT0		0x4
/* ... */

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
	
	/* Set MISO and MOSI to ALT0 (SPI0) */
	var = *gpioGPFSEL1;
	var &=~((7<<0) | (7<<3)); // clear function selects
    var |= ((4<<0) | (4<<3)); // set to alt function ALT0
	*gpioGPFSEL1 = var;
	
	/* Set SCLK, CS0 and CS1 to ALT0 (SPI0) */
	var = *gpioGPFSEL0;
	var &=~((7<<21) | (7<<24) | (7<<27)); // clear function selects
    var |= ((4<<21) | (4<<24) | (4<<27)); // set to alt function ALT0
	*gpioGPFSEL0 =var;
	
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
	/* Read current value of GPFSEL1- pin 10 & 11 */
	//var = *gpioGPFSEL1;
	//var &=~((7<<0) | (7<<3)); // set as input pins GPIO = 000
    /* Write back updated value */
	//*gpioGPFSEL1 =var;
	
	/* Read current value of GPFSEL1- pin 7,8 & 9 */
	//var = *gpioGPFSEL0;
	//var &=~((7<<21) | (7<<24) | (7<<27)); // set as input pins GPIO = 000
    /* Write back updated value */           
	//*gpioGPFSEL0 =var;
	
	/* i added */    
    // Set TA = 0, 
    var = *SPI0_CONTROL;
    var &= ~(1 << SPI_C_TA);
    /* Write back updated value */
	*SPI0_CONTROL =var;	
}
