#ifndef __CUBEPINMAPPING_H__
#define __CUBEPINMAPPING_H__  // sets up so if it is included multiple times the compiler only defines the following once and wont throw errors at you

//FIXME: these things need a prefix, this is rather confusing without it :D

#include <stdint.h>

#define NUM_FACES 6

#define INT1 	2
#define CS1		3
#define	INT2 	4
#define CS2		17
#define INT3	27
#define CS3		22
#define INT4	18
#define CS4		23
#define INT5	24
#define CS5		25
#define INT6	8
#define CS6		7


#define MPU_DEV1 CS1
#define MPU_DEV2 CS2
#define MPU_DEV3 CS3
#define MPU_DEV4 CS4
#define MPU_DEV5 CS5
#define MPU_DEV6 CS6

extern char shape_cs_mappings[];

const double shape_accel_calibration[NUM_FACES][3];

const double shape_gyro_calibration[NUM_FACES][3];

const double shape_temp_calibration[NUM_FACES];

const double shape_orientation[NUM_FACES][3];
	
#endif /* __CUBEPINMAPPING_H__ */

