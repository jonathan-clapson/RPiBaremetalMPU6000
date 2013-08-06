#include <stdint.h>

#include "mpu60x0regs.h"
#include "mpu60x0.h"
#include "spi.h"

void mpu60x0_init(struct mpu60x0_stateType *mpu60x0_state)
{
		uint8_t data;
		
		spi_write(INV_MPU60x0_REG_PWR_MGMT_1, INV_MPU60x0_BIT_H_RESET);
		wait(1000*80);
		
		data = spi_read(INV_MPU60x0_REG_WHO_AM_I);
		wait(1000*80);
		
		spi_write(INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_FIFO_RST|INV_MPU60x0_BIT_I2C_MST_RST|INV_MPU60x0_BIT_SIGCOND_RST);
		spi_write(INV_MPU60x0_REG_PWR_MGMT_1, 0x03);
		spi_write(INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_I2C_IF_DIS);
		spi_write(INV_MPU60x0_REG_CONFIG, INV_MPU60x0_FILTER_42HZ);
		
		mpu60x0_state->gyro_rate = INV_MPU60x0_FSR_2000DPS;
		spi_write(INV_MPU60x0_REG_GYRO_CONFIG, INV_MPU60x0_FSR_2000DPS);
		
		mpu60x0_state->accel_rate = INV_MPU60x0_FS_02G;
		spi_write(INV_MPU60x0_REG_ACCEL_CONFIG, INV_MPU60x0_FS_02G);
		
		char tmp[10];
		sprintf(tmp, "WhoAmI: %x\n\r", data);				
		uart_puts((unsigned char*) tmp);
}

void mpu60x0_get_reading(struct mpu60x0_stateType mpu60x0_state, struct readingsType *reading)
{
	uint8_t byte_H;
	uint8_t byte_L;
	int16_t conv;
			
	// We start a SPI multibyte read of sensors
	spi_begin();
	spi_transfer(INV_MPU60x0_REG_RAW_ACCEL|0x80);
	
	// Read AccelX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->accelX = conv/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate]; //fixme!!
	// Read AccelY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->accelY = conv/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate];
	// Read AccelZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->accelZ = conv/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate];
	// Read Temp
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->temp = conv/340 + 36.53;
	// Read GyroX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->gyroX = conv/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //fixme!!
	// Read GyroY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->gyroY = conv/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //fixme!!
	// Read GyroZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->gyroZ = conv/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //fixme!!
	
	spi_end();
}
