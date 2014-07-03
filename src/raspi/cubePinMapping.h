/*!	\file cubePinMapping.h
 * 	\brief This file stores information about the mapping of sensors when they are mounted to the cube block.
 * 
 * 	The necessary information to describe a sensor configuration should be described by this file in conjunction iwth cubePinMapping.c
 */

#ifndef __CUBEPINMAPPING_H__
#define __CUBEPINMAPPING_H__  // sets up so if it is included multiple times the compiler only defines the following once and wont throw errors at you

//FIXME: these things need a prefix, this is rather confusing without it :D

#include <stdint.h>

/*! NUM_FACES defines the number of sensors in the sensor configuration */
#define NUM_FACES 6

/*! \defgroup RIHM RPi Interrupt hardware mappings 
 * These define the mapping of RPi GPIO connected to MPU6000 Interrupt pins 
 * These should be numbered in the same order as the RPi ChipSelect mappings
 * @{
 */
#define INT1 	2
#define	INT2 	4
#define INT3	27
#define INT4	18
#define INT5	24
#define INT6	8
/*! @} */


/*! \defgroup RCSM RPi ChipSelect mappings
 * These define the mapping of RPi GPIO connected to MPU6000 Chip Select Pins
 * These should be numbered in the same order as the RPi Interrupt hardware mappings
 * @{ 
 */
#define CS1		3
#define CS2		17
#define CS3		22
#define CS4		23
#define CS5		25
#define CS6		7
/*! @} */

/*! \defgroup RDEVIDMACRO RPi Chip DeviceId Helper Macros
 * These define "device id's" which can be used to refer to devices in code
 * @{ 
 */
#define MPU_DEV1 CS1
#define MPU_DEV2 CS2
#define MPU_DEV3 CS3
#define MPU_DEV4 CS4
#define MPU_DEV5 CS5
#define MPU_DEV6 CS6
/*! @} */

extern char shape_cs_mappings[];

const double shape_accel_calibration[NUM_FACES][3];

const double shape_gyro_calibration[NUM_FACES][3];

const double shape_temp_calibration[NUM_FACES];

const double shape_orientation[NUM_FACES][3];
	
#endif /* __CUBEPINMAPPING_H__ */

