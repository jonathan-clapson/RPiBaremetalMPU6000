#include <stdio.h>
#include <stdint.h>

#include "raspi.h"
#include "mpu60x0regs.h"
#include "spi.h"
#include "uart.h"

#define CS 8
#define SCK 11
#define MISO 9
#define MOSI 10

typedef enum{
  INPUT = 0,
  OUTPUT = 1,
  ALT0 = 2,
  ALT1 = 3,
  ALT2 = 4,
  ALT3 = 5,
  ALT4 = 6,
  ALT5 = 7
} FSEL;


void GPIO_FSEL(unsigned int pin, FSEL sel)
{
  volatile unsigned int* GPIO_BASE = (unsigned int *)0x20200000;
  unsigned int curState = 0;
  
  // get right address for pin
  if(pin >= 10)
    GPIO_BASE++;
  if(pin >= 20)
    GPIO_BASE++;
  if(pin >= 30)
    GPIO_BASE++;
  if(pin >= 40)
    GPIO_BASE++;
  if(pin >= 50)
    GPIO_BASE++;

  pin %= 10; // we can set 10 pins per register
  
  curState = *(GPIO_BASE);       // get GPIO reg state
  
  curState &= ~(7 << (pin * 3)); // clear bits in question
  curState |= sel << (pin * 3);  // set bits in question
  
  *(GPIO_BASE) = curState;       // write state back to GPIO reg
}

/*int spi_init()
{
  GPIO_FSEL(CS, INPUT);
  GPIO_FSEL(SCK, INPUT);
  GPIO_FSEL(MOSI, INPUT);
  GPIO_FSEL(MISO, OUTPUT);
}*/

/********************************************************************************************************************************************************
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 Author    : Geoffrey 
 
 Date      : 2012

 version   :

**********************************************************************************************************************************************************/
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

#define INV_MPU60x0_BIT_I2C_IFACE_DISABLE (1<<4)

struct readingsType
{
	int16_t accelX;
	int16_t accelY;
	int16_t accelZ;
	int16_t temp;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;	
};

void init_mpu60x0()
{
		uint8_t data;
		
		spi_write(INV_MPU60x0_REG_PWR_MGMT_1, INV_MPU60x0_BIT_H_RESET);
		wait(1000*80);
		
		data = spi_read(INV_MPU60x0_REG_WHO_AM_I);
		wait(1000*80);
		
		spi_write(INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_FIFO_RST|INV_MPU60x0_BIT_I2C_MST_RST|INV_MPU60x0_BIT_SIGCOND_RST);
		spi_write(INV_MPU60x0_REG_PWR_MGMT_1, 0x03);
		spi_write(INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_I2C_IFACE_DISABLE);
		spi_write(INV_MPU60x0_REG_CONFIG, INV_MPU60x0_FILTER_42HZ);
		spi_write(INV_MPU60x0_REG_GYRO_CONFIG, INV_MPU60x0_FSR_2000DPS);
		spi_write(INV_MPU60x0_REG_ACCEL_CONFIG, INV_MPU60x0_FS_02G);
		
		char tmp[10];
		sprintf(tmp, "WhoAmI: %x\n\r", data);				
		uart_puts((unsigned char*) tmp);
}

__attribute__((no_instrument_function)) void not_main(void)
{	
	/* board initialisation */
	uart_init();
	spi_pin_init();
	
	init_mpu60x0();
	
	UINT32 sel = READ32(GPFSEL1);
	sel &= ~(0b111 << 18);
	sel |= (0b001 << 18);
	WRITE32(GPFSEL1,sel);
	
	while(1){
		struct readingsType reading;
		//init_mpu60x0();
		
		uint8_t byte_H;
		uint8_t byte_L;
				
		// We start a SPI multibyte read of sensors
		spi_begin();
		spi_transfer(INV_MPU60x0_REG_RAW_ACCEL|0x80);
		
		// Read AccelX
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.accelX = ((int16_t)byte_H<<8)| byte_L;
		// Read AccelY
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.accelY = ((int16_t)byte_H<<8)| byte_L;
		// Read AccelZ
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.accelZ = ((int16_t)byte_H<<8)| byte_L;
		// Read Temp
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.temp = ((int16_t)byte_H<<8)| byte_L;
		// Read GyroX
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.gyroX = ((int16_t)byte_H<<8)| byte_L;
		// Read GyroY
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.gyroY = ((int16_t)byte_H<<8)| byte_L;
		// Read GyroZ
		byte_H = spi_transfer(0);
		byte_L = spi_transfer(0);
		reading.gyroZ = ((int16_t)byte_H<<8)| byte_L;
		
		spi_end();
		
		char tmp[100];
		sprintf(tmp, "xAccel: %+06d, yAccel: %+06d, zAccel: %+06d, temp: %+06d, xGyro: %+06d, yGyro %+06d, zGyro %+06d\r\n", \
			reading.accelX, reading.accelY, reading.accelZ, reading.temp, reading.gyroX, reading.gyroY, reading.gyroZ);
		uart_puts((unsigned char*) tmp);
				
		PUT32(GPCLR0, 1<<16);
		wait(1000*80);
		PUT32(GPSET0, 1<<16);
		wait(1000*80);
	}
}
