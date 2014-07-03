/*!	\file main_readings.h
 * 	\brief This routine continuously receives data from MPU6000 sensors, integrates data, converts gyro data into the same coordinate space and then writes the data to uart.
 */

#ifndef __MAIN_READINGS_H__
#define __MAIN_READINGS_H__

#include "mpu60x0.h"

void main_readings (struct mpu60x0_stateType *mpu60x0_state);

#endif /* __MAINDEBUG_H__ */
