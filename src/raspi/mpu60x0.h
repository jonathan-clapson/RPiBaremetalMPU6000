/*!	\file mpu60x0.h
 * 	\brief Methods for interacting with the mpu6000.
 * 
 * 	Methods for interacting with the mpu6000. Also defines the data type used to store unprocessed and processed sensor data
 */
#ifndef __MPU60X0_H__
#define __MPU60X0_H__

#include "mpu60x0regs.h"

struct mpu60x0_stateType {
	enum inv_mpu60x0_gyro_fsr_e	gyro_rate;
	enum inv_mpu60x0_accl_fs_e	accel_rate;	
	enum inv_mpu60x0_filter_e filter_cutoff;
};

struct reading_memory_type {
	/*
	 * timestamp is a long int to keep 8byte alignment
	 * it may be actually useful if the 64bit timer read function is used
	 */
	long int timestamp;
	double a_x, a_y, a_z;
	double temp;
	double w_x, w_y, w_z;
	
	double x,y,z;
	double v_x, v_y, v_z;
	
	double o_x, o_y, o_z;	
};

void mpu60x0_get_reading(int device, struct mpu60x0_stateType mpu60x0_state, struct reading_memory_type *reading);
void mpu60x0_get_reading_raw(int device, struct reading_memory_type *reading);
int mpu60x0_init(int device, struct mpu60x0_stateType *mpu60x0_state);

#endif
