/*!	\file config.h
 * 	\brief Header file used to set which pin mapping to use for the currently connected sensor configuration.
 * 
 * 	To change pin mapping configurations change the #include to include the file for the desired sensor configuration.
 * 	\todo It would be nice to be able to store 'sensor configuration files' on the sd card so the desired config can be chosen at runtime.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "cubePinMapping.h"

/* functions below here must be provided by the shape c file */

void shape_set_cs();
extern char shape_cs_mappings[NUM_FACES];

#endif /* __CONFIG_H__ */
