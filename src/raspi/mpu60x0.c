/*!	\file mpu60x0.c
 * 	\brief Methods for interacting with the mpu6000.
 * 
 * 	Methods for interacting with the mpu6000. Also devines the data type used to store unprocessed and processed sensor data
 */
#include <stdint.h>

#include "support.h"
#include "mpu60x0regs.h"
#include "mpu60x0.h"
#include "spi.h"
#include "uart.h"
#include "systimer.h"

/*!	\fn void mpu60x0_enable_interrupt( int device )
 *	\brief Configures the MPU6000 so interrupt pins are triggered when new data is ready
 *	@param[in] device The spi device to enable interrupts on
 */
void mpu60x0_enable_interrupt( int device )
{
	//interrupt is by default active high, disabling latch (int pin is asserted for 50uS then dropped)
	spi_write(device, INV_MPU60x0_REG_INT_PIN_CFG, INV_MPU60x0_BITS_INT_LATCH_EN);
	
	//enable data ready interrupt
	spi_write(device, INV_MPU60x0_REG_INT_ENABLE, 0x01);//INV_MPU60x0_BIT_DATA_RDY_EN);	
	spi_write(device, INV_MPU60x0_REG_INT_ENABLE, INV_MPU60x0_BIT_DATA_RDY_EN);//INV_MPU60x0_BIT_DATA_RDY_EN);	
}

/*!	\fn int mpu60x0_init(int device, struct mpu60x0_stateType *mpu60x0_state)
 *	\brief Initialise an mpu6000 device
 * 	
 * 	Reset the device
 * 	Read the i2c whoami register to determine if the device is working correctly (at least as an spi device) 	
 *	@param[in] device The spi device to enable interrupts on
 * 	@param[out] mpu60x0_state pointer to a state structure to be updated
 * 	\return whether the read of the whoami register was successful
 */
int mpu60x0_init(int device, struct mpu60x0_stateType *mpu60x0_state)
{
	uint8_t data;
	
	//reset the device
	spi_write(device, INV_MPU60x0_REG_PWR_MGMT_1, INV_MPU60x0_BIT_H_RESET);
	wait(1000*80);
	
	//read i2c configuration data FIXME: maybe use this to identify whether the mpu sensors are connected?
	spi_read(device, INV_MPU60x0_REG_WHO_AM_I, &data);
	wait(1000*80);
	
	//reset fifos, i2c master and signal paths		
	spi_write(device, INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_FIFO_RST|INV_MPU60x0_BIT_I2C_MST_RST|INV_MPU60x0_BIT_SIGCOND_RST);
	//wake chip up, configure clock as gyroZ
	spi_write(device, INV_MPU60x0_REG_PWR_MGMT_1, 0x03);
	//disable i2c slave interface
	spi_write(device, INV_MPU60x0_REG_USER_CTRL, INV_MPU60x0_BIT_I2C_IF_DIS);
	//set low pass filter cutoff frequency
	//spi_write(device, INV_MPU60x0_REG_CONFIG, INV_MPU60x0_FILTER_5HZ);
	spi_write(device, INV_MPU60x0_REG_CONFIG, mpu60x0_state->filter_cutoff);
		
	//set gyro scale		
	mpu60x0_state->gyro_rate = INV_MPU60x0_FSR_250DPS;
	spi_write(device, INV_MPU60x0_REG_GYRO_CONFIG, mpu60x0_state->gyro_rate);
	
	//set accel scale
	mpu60x0_state->accel_rate = INV_MPU60x0_FS_02G;
	spi_write(device, INV_MPU60x0_REG_ACCEL_CONFIG, mpu60x0_state->accel_rate);
	
	mpu60x0_enable_interrupt(device);
		
	return data==INV_MPU60x0_BIT_ID;
}

/*!	\fn void mpu60x0_get_reading(int device, struct mpu60x0_stateType mpu60x0_state, struct reading_memory_type *reading)
 *	\brief Obtain a set of readings from the device
 * 
 * 	A set of readings is a new set of data for each of accelerometers, gyrometers, temperature sensing and timestamps.
 *	@param[in] device The spi device to enable interrupts on
 *	@param[in] mpu60x0_state a pointer to a state of the sensor which is used to determine the conversion ratios
 * 	@param[out] reading a pointer to a sensor reading structure this function will update
 */
void mpu60x0_get_reading(int device, struct mpu60x0_stateType mpu60x0_state, struct reading_memory_type *reading)
{
	uint8_t byte_H;
	uint8_t byte_L;
	int16_t conv;
	double conv_double;
			
	reading->timestamp = systimer_get_us_32bit();

	// We start a SPI multibyte read of sensors
	spi_begin(device);
	spi_transfer(INV_MPU60x0_REG_RAW_ACCEL|0x80);	
	
	// Read AccelX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->a_x = conv_double/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate]; //FIXME:
	// Read AccelY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->a_y = conv_double/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate];
	// Read AccelZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->a_z = conv_double/inv_mpu60x0_accl_conv_ratio[mpu60x0_state.accel_rate];
	// Read Temp
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->temp = conv_double/340.0 + 36.53;
	// Read GyroX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->w_x = conv_double/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //FIXME:
	// Read GyroY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->w_y = conv_double/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //fixme!!
	// Read GyroZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	conv_double = conv;
	reading->w_z = conv_double/inv_mpu60x0_gyro_conv_ratio[mpu60x0_state.gyro_rate]; //fixme!!
	
	spi_end();
}

/*
 * This is a fake raw function, it doesn't actually pass the raw data, but converts the raw data to a double so i can be lazy :)
 */
/*!	\fn void mpu60x0_get_reading_raw(int device, struct reading_memory_type *reading)
 *	\brief Obtain a set of "raw" readings from the device
 * 
 * 	The data returned from this function is actually the raw 16bit integer data converted to a double. This makes it easier when dealing with the serial protocol defined in the final report
 *	@param[in] device The spi device to enable interrupts on
 * 	@param[out] reading a pointer to a sensor reading structure this function will update
 */
void mpu60x0_get_reading_raw(int device, struct reading_memory_type *reading)
{
	uint8_t byte_H;
	uint8_t byte_L;
	int16_t conv;
	
	reading->timestamp = systimer_get_us_32bit();
			
	// We start a SPI multibyte read of sensors
	spi_begin(device);
	spi_transfer(INV_MPU60x0_REG_RAW_ACCEL|0x80);
	
	// Read AccelX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->a_x = (double) conv;
	
	// Read AccelY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->a_y = (double) conv;
	// Read AccelZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->a_z = (double) conv;
	// Read Temp
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->temp = (double) conv;
	// Read GyroX
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->w_x = (double) conv;
	// Read GyroY
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->w_y = (double) conv;
	// Read GyroZ
	byte_H = spi_transfer(0);
	byte_L = spi_transfer(0);
	conv = ((int16_t)byte_H<<8)| byte_L;
	reading->w_z = (double) conv;
	
	spi_end();
}
