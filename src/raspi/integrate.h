/*!	\file integrate.h
 * 	\brief Provides integration routines
 */

#ifndef __INTEGRATE_H__
#define __INTEGRATE_H__

#include "integrate.h"
#include "vector.h"

void integrate_vector(struct vector *result, struct vector *values, struct vector *last_values, unsigned int timestep);

#endif /* __INTEGRATE_H__ */
