#include "vector.h"
#include <math.h>

/*
 * Trapezium rule
 * p1 - last data point
 * p2 - new data point
 * timestep - size of time interval between data points
 * return value - 'area under the graph'
 */

double findArea(double p1, double p2, double timestep)
{
	double rectSize = p1*timestep;
	
	/* triangle size takes off the extra if p1 > p2 otherwise it adds */
	double triSize = (p2-p1)/2.0f;
	
	return rectSize+triSize;
}

//infinityPath[1] is the same as *(infinityPath+1) where the * means 'pulling data from' and [] is making a pointer =)
//takes in both a vector you wish to integrate and 	a vector (one smalles than the first vector) to store the values of the integral
void integrate_vector(struct direcType result[], struct direcType vectors[], int num_elements)
{	
	/* 
	 * Simulates the integration of a vector of acceleration data passed into the function
	 * The fake sample rate for this code (N) is set to 1 sample taken every 125us = 0.000125
	 */

	int time_step = 0.000125;
	
	//calculates the area under two points on the vector plot and saves it into a new vector
	//modify below function when using cubic interpolation
	/*for(int i=0; i<(vectSize-1); i++) {
		
		newVect[i].x = findArea(vector[i].x, vector[i+1].x, time_step);
		
		newVect[i].y = findArea(vector[i].y, vector[i+1].y, time_step);
		
		newVect[i].z = findArea(vector[i].z, vector[i+1].z, time_step);
	}*/
	
	result[num_elements-1].x = findArea(vectors[num_elements-1].x, vectors[num_elements].x, time_step);
	result[num_elements-1].y = findArea(vectors[num_elements-1].y, vectors[num_elements].y, time_step);
	result[num_elements-1].z = findArea(vectors[num_elements-1].z, vectors[num_elements].z, time_step);
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
