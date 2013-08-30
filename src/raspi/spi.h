#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

int spi_read(int pin, uint8_t reg, uint8_t *data);
int spi_write(int pin, uint8_t reg, uint8_t data);
void spi_pin_init(void);
int spi_begin(int pin);
unsigned int spi_transfer(unsigned char value);
int spi_end(void);

#endif
