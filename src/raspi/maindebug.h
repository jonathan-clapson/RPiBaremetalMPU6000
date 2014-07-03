/*!	\file maindebug.h
 * 	\brief This file contains routines for continuously taking 'raw' readings and writing them to the uart.
 * 	Check mpu60x0.c for details on the raw reading function
 */

#ifndef __MAINDEBUG_H__
#define __MAINDEBUG_H__

#include "mpu60x0.h"

void main_debug (struct mpu60x0_stateType *mpu60x0_state);

#endif /* __MAINDEBUG_H__ */
