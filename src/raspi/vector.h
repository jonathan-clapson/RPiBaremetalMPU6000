/*!	\file vector.h
 * 	\brief Defines the vector type which is used when working with XYZ values of single sensors
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

/*! \struct vector 
 * \brief this structure defines a set of doubles which are sequential in memory. Useful when working with a single sensor in X, Y and Z
 */
struct vector {
	double x, y, z;
};

#endif
