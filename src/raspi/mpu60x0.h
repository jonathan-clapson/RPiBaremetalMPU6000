#ifndef __MPU60X0_H__
#define __MPU60X0_H__

#include "mpu60x0regs.h"

struct mpu60x0_stateType {
	enum inv_mpu60x0_gyro_fsr_e	gyro_rate;
	enum inv_mpu60x0_accl_fs_e	accel_rate;	
};

struct readingsType
{
	double_t accelX;
	double_t accelY;
	double_t accelZ;
	double_t temp;
	double_t gyroX;
	double_t gyroY;
	double_t gyroZ;	
};

void mpu60x0_get_reading(struct mpu60x0_stateType mpu60x0_state, struct readingsType *reading);
void mpu60x0_init(struct mpu60x0_stateType *mpu60x0_state);

#endif
