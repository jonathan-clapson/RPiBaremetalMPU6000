/*!	\file integrate.c
 * 	\brief Provides integration routines
 */

#include "vector.h"
#include <math.h>

/*!	\fn double findArea(double last_point, double current_point, double timestep)
 *	\brief Calculates the area between two points with a given timestep assuming they form a trapezium shape.
 *  
 *	@param[in] last_point the data point before the current one
 * 	@param[in] current_point the current data point
 * 	@param[in] timestep the different in time between the current and last data point
 * 	\return The area of trapezium made up from the input data
 */
double findArea(double last_point, double current_point, double timestep)
{
	double rectSize = last_point*timestep;
	//printf("last_point: %f, current_point %f, timestep %f\n", last_point, current_point, timestep);
	
	/* triangle size takes off the extra if p1 > p2 otherwise it adds */
	double triSize = (current_point-last_point)*timestep/2.0f;
	
	//printf("rectsize: %f, trisize %f\n", rectSize, triSize);
	
	return rectSize+triSize;
}

/*!	\fn void integrate_vector(struct vector *result, struct vector *values, struct vector *last_values, unsigned int timestep)
 *	\brief Updates the running integration total.
 * 	This integration uses a running total of trapezium rule. Ie as new data points are read, they must be fed to this function. 
 *  
 *	@param[in] result pointer to a vector to store the result
 * 	@param[in] values pointer to the most recent values
 * 	@param[in] last_values pointer to the next most recent values
 * 	@param[in] timestep the different in time between values and last_values being read
 */
void integrate_vector(struct vector *result, struct vector *values, struct vector *last_values, unsigned int timestep)
{		
	double double_ts = (double) timestep/1e6;
	
	//printf("last x: %f, y: %f z:%f\n", last_values->x, last_values->y, last_values->z);
	//printf("cur x: %f, y: %f z:%f\n", values->x, values->y, values->z);	
	result->x += findArea(last_values->x, values->x, double_ts);
	result->y += findArea(last_values->y, values->y, double_ts);
	result->z += findArea(last_values->z, values->z, double_ts);
	
	//printf("found x area:%f\n", findArea(last_values->x, values->x, double_ts));
	//printf("found y area:%f\n", findArea(last_values->y, values->y, double_ts));
	//printf("found z area:%f\n", findArea(last_values->z, values->z, double_ts));

}
