/********************************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date      : 2012

 version   :

**********************************************************************************************************************************************************/

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
