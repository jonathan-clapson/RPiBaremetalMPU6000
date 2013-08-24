#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

uint8_t spi_read(uint8_t reg) ;
void spi_write(uint8_t reg, uint8_t data);
void spi_pin_init(void);
void spi_begin(void);
unsigned int spi_transfer(unsigned char value);
void spi_end(void);

#endif
