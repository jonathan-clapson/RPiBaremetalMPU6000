/*
* Copyright (C) 2012 Invensense, Inc.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef __MPU60X0REGS_H__
#define __MPU60X0REGS_H__

#include "math.h" 

/* register and associated bit definition */
#define INV_MPU60x0_REG_SAMPLE_RATE_DIV     0x19
#define INV_MPU60x0_REG_CONFIG              0x1A
#define INV_MPU60x0_REG_GYRO_CONFIG         0x1B
#define INV_MPU60x0_REG_ACCEL_CONFIG	    0x1C

#define INV_MPU60x0_REG_FIFO_EN             0x23
#define INV_MPU60x0_BIT_ACCEL_OUT           0x08
#define INV_MPU60x0_BITS_GYRO_OUT           0x70

#define INV_MPU60x0_REG_INT_PIN_CFG			0x55
#define INV_MPU60x0_BITS_INT_ACTIVE_LOW		0x80
#define INV_MPU60x0_BITS_INT_OPEN_DRAIN		0x40
#define INV_MPU60x0_BITS_INT_LATCH_EN		0x20

#define INV_MPU60x0_REG_INT_ENABLE          0x38
#define INV_MPU60x0_BIT_DATA_RDY_EN         0x01
#define INV_MPU60x0_BIT_DMP_INT_EN          0x02

#define INV_MPU60x0_REG_RAW_ACCEL           0x3B
#define INV_MPU60x0_REG_TEMPERATURE         0x41
#define INV_MPU60x0_REG_RAW_GYRO            0x43

#define INV_MPU60x0_REG_USER_CTRL           0x6A
#define INV_MPU60x0_BIT_SIGCOND_RST 		0x01
#define INV_MPU60x0_BIT_I2C_MST_RST 		0x02
#define INV_MPU60x0_BIT_FIFO_RST            0x04
#define INV_MPU60x0_BIT_DMP_RST             0x08
#define INV_MPU60x0_BIT_I2C_IF_DIS			0x10
#define INV_MPU60x0_BIT_I2C_MST_EN          0x20
#define INV_MPU60x0_BIT_FIFO_EN             0x40
#define INV_MPU60x0_BIT_DMP_EN              0x80

#define INV_MPU60x0_REG_PWR_MGMT_1          0x6B
#define INV_MPU60x0_BIT_H_RESET             0x80
#define INV_MPU60x0_BIT_SLEEP               0x40
#define INV_MPU60x0_BIT_CLK_MASK            0x7

#define INV_MPU60x0_REG_PWR_MGMT_2          0x6C
#define INV_MPU60x0_BIT_PWR_ACCL_STBY       0x38
#define INV_MPU60x0_BIT_PWR_GYRO_STBY       0x07

#define INV_MPU60x0_REG_FIFO_COUNT_H        0x72
#define INV_MPU60x0_REG_FIFO_R_W            0x74

#define INV_MPU60x0_BYTES_PER_3AXIS_SENSOR   6
#define INV_MPU60x0_FIFO_COUNT_BYTE          2
#define INV_MPU60x0_FIFO_THRESHOLD           500
#define INV_MPU60x0_POWER_UP_TIME            100
#define INV_MPU60x0_TEMP_UP_TIME             100
#define INV_MPU60x0_SENSOR_UP_TIME           30
#define INV_MPU60x0_REG_UP_TIME              5

#define INV_MPU60x0_TEMP_OFFSET				12421
#define INV_MPU60x0_TEMP_SCALE				2941
#define INV_MPU60x0_MAX_GYRO_FS_PARAM		3
#define INV_MPU60x0_MAX_ACCL_FS_PARAM		3
#define INV_MPU60x0_THREE_AXIS				3
#define INV_MPU60x0_GYRO_CONFIG_FSR_SHIFT	3
#define INV_MPU60x0_ACCL_CONFIG_FSR_SHIFT	3

/* 6 + 6 round up and plus 8 */
#define INV_MPU60x0_OUTPUT_DATA_SIZE		24

/* init parameters */
#define INV_MPU60x0_INIT_FIFO_RATE			50
#define INV_MPU60x0_TIME_STAMP_TOR			5
#define INV_MPU60x0_MAX_FIFO_RATE			1000
#define INV_MPU60x0_MIN_FIFO_RATE			4
#define INV_MPU60x0_ONE_K_HZ				1000

#define INV_MPU60x0_REG_WHO_AM_I			0x75
#define INV_MPU60x0_BIT_ID					0x68

/* scan element definition */
enum inv_mpu60x0_scan {
	INV_MPU60x0_SCAN_ACCL_X,
	INV_MPU60x0_SCAN_ACCL_Y,
	INV_MPU60x0_SCAN_ACCL_Z,
	INV_MPU60x0_SCAN_GYRO_X,
	INV_MPU60x0_SCAN_GYRO_Y,
	INV_MPU60x0_SCAN_GYRO_Z,
	INV_MPU60x0_SCAN_TIMESTAMP,
};

enum inv_mpu60x0_filter_e {
	INV_MPU60x0_FILTER_256HZ_NOLPF2 = 0,
	INV_MPU60x0_FILTER_188HZ,
	INV_MPU60x0_FILTER_98HZ,
	INV_MPU60x0_FILTER_42HZ,
	INV_MPU60x0_FILTER_20HZ,
	INV_MPU60x0_FILTER_10HZ,
	INV_MPU60x0_FILTER_5HZ,
	INV_MPU60x0_FILTER_2100HZ_NOLPF,
	NUM_MPU60x0_FILTER
};

/* IIO attribute address */
enum INV_MPU60x0_IIO_ATTR_ADDR {
	ATTR_GYRO_MATRIX,
	ATTR_ACCL_MATRIX,
};

extern const double_t inv_mpu60x0_accl_conv_ratio[];

enum inv_mpu60x0_accl_fs_e {
	INV_MPU60x0_FS_02G = 0,
	INV_MPU60x0_FS_04G,
	INV_MPU60x0_FS_08G,
	INV_MPU60x0_FS_16G,
	NUM_ACCL_FSR
};

extern const double_t inv_mpu60x0_gyro_conv_ratio[];

enum inv_mpu60x0_gyro_fsr_e {
	INV_MPU60x0_FSR_250DPS = 0,
	INV_MPU60x0_FSR_500DPS,
	INV_MPU60x0_FSR_1000DPS,
	INV_MPU60x0_FSR_2000DPS,
	NUM_MPU60x0_FSR
};

enum inv_mpu60x0_clock_sel_e {
	INV_CLK_INTERNAL = 0,
	INV_CLK_PLL,
	NUM_CLK
};

#endif /*__MPU60X0REGS_H__ */
