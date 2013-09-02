#include "vector.h"
#include <math.h>

/*
 * Trapezium rule
 * p1 - last data point
 * p2 - new data point
 * timestep - size of time interval between data points
 * return value - 'area under the graph'
 */

double findArea(double last_point, double current_point, double timestep)
{
	double rectSize = last_point*timestep;
	
	/* triangle size takes off the extra if p1 > p2 otherwise it adds */
	double triSize = (current_point-last_point)/2.0f;
	
	return rectSize+triSize;
}

//infinityPath[1] is the same as *(infinityPath+1) where the * means 'pulling data from' and [] is making a pointer =)
//takes in both a vector you wish to integrate and 	a vector (one smalles than the first vector) to store the values of the integral
void integrate_vector(struct vector *result, struct vector *values)
{	
	static struct vector last_vector = {0};
	/* 
	 * Simulates the integration of a vector of acceleration data passed into the function
	 * The fake sample rate for this code (N) is set to 1 sample taken every 125us = 0.000125
	 */

	int time_step = 0.000125;
	
	result->x += findArea(last_vector.x, values->x, time_step);
	result->y += findArea(last_vector.y, values->y, time_step);
	result->z += findArea(last_vector.z, values->z, time_step);
	
	last_vector.x = values->x;
	last_vector.y = values->y;
	last_vector.z = values->z;
}

//creates the vector components of a circular acceleration pattern
/*void circVects(int vectSize, direcType vector[]){
	
	const double pi = 4*atan(1);
	
	int i=0;
	double thetaChange = 360/vectSize; // number of degrees the constant in magnitude acceleration changes direction
	double currentAngle = 0;
	
	for(i; i<vectSize; i++){
		vector[i].x = 10*sin(currentAngle*PI/180) + 10;
		vector[i].y = 10*cos(currentAngle*PI/180) + 10;
		currentAngle += thetaChange;
	}
	
	return;
}*/
