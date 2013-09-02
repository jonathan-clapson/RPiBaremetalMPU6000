#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "cubePinMapping.h"

/* functions below here must be provided by the shape c file */

void shape_set_cs();
extern char shape_cs_mappings[NUM_FACES];

#endif /* __CONFIG_H__ */
