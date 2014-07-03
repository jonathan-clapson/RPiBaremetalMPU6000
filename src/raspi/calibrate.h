/*!	\file calibrate.h
 * 	\brief Header file exporting the main calibration routine
 */

#ifndef __CALIBRATE_H__
#define __CALIBRATE_H__

#include "mpu60x0.h"

void static_calibration(struct mpu60x0_stateType *mpu60x0_state);

#endif /* __CALIBRATE_H__ */
