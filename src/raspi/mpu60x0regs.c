#include "mpu60x0regs.h"

/* 
 * Accelerometer Mappings
 * 0 ±2g	16384LSB/g
 * 1 ±4g	8192 LSB/g
 * 2 ±8g	4096 LSB/g
 * 3 ±16g	2048 LSB/g
 */
const double_t inv_mpu60x0_accl_conv_ratio[] = 
{
	16384,
	8192,
	4096,
	2048
};

/*
 * Gyrometer Mappings
 * 0 ± 250 °/s 131 LSB/°/s
 * 1 ± 500 °/s 65.5 LSB/°/s
 * 2 ± 1000 °/s 32.8 LSB/°/s
 * 3 ± 2000 °/s 16.4 LSB/°/s
 */
const double_t inv_mpu60x0_gyro_conv_ratio[] = 
{
	131,
	65.5,
	32.8,
	16.4
};
