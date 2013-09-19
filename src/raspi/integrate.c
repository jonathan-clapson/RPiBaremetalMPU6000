#include "vector.h"
#include <math.h>

double findArea(double last_point, double current_point, double timestep)
{
	double rectSize = last_point*timestep;
	//printf("last_point: %f, current_point %f, timestep %f\n", last_point, current_point, timestep);
	
	/* triangle size takes off the extra if p1 > p2 otherwise it adds */
	double triSize = (current_point-last_point)*timestep/2.0f;
	
	//printf("rectsize: %f, trisize %f\n", rectSize, triSize);
	
	return rectSize+triSize;
}

/*
 * result = the location for integrand results to be stored
 * values = the location for x,y,z updates
 * timestep = the change in timestep between current and previous readings
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
