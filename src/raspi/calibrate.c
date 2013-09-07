#include <string.h>

#include "calibrate.h"
#include "mpu60x0.h"
#include "uart.h"

#include "config.h"

void find_orientation(struct reading_memory_type *reading_memory, double orientations[NUM_FACES][sizeof(struct reading_memory_type)/sizeof(double)])
{
	for (int i=0; i<NUM_FACES; i++)
	{
		double *r_mem_ptr = (double *) &(reading_memory[i]);
				
		for (int j=0; j<sizeof(struct reading_memory_type)/sizeof(double); j++)
		{			
			if (r_mem_ptr[j] > 0.5)
				orientations[i][j] = 1.0;
			else if (r_mem_ptr[j] < -0.5)
				orientations[i][j] = -1.0;
			else
				orientations[i][j] = 0.0;
		}
	}	
}

/*void remove_orientation_bias(struct reading_memory_type *reading_memory, double orientations[NUM_FACES][sizeof(struct reading_memory_type)/sizeof(double)])
{
	double *reading_ptr = (double *) results;
	
	int num_iters = sizeof(struct reading_memory_type)*NUM_FACES/sizeof(double);
	for (int i=0; i<NUM_FACES; i++)
	{
		for (int j=0; j<sizeof(struct reading_memory_type)/sizeof(double); j++)
			reading_ptr[i] -= orientations[i][j];
	}
	
}*/

/* 
 * used to update the current totals of all sensors
 */
void sum_results(struct reading_memory_type *results, struct reading_memory_type *values)
{
	double *result_ptr = (double *) results;
	double *values_ptr = (double *) values;
	
	int num_iters = sizeof(struct reading_memory_type)*NUM_FACES/sizeof(double);
	for (int i=0; i<num_iters; i++)
		result_ptr[i] += values_ptr[i];	
}

void compute_averages(struct reading_memory_type *averages, struct reading_memory_type *totals, double divisor)
{
	double *averages_ptr = (double *) averages;
	double *totals_ptr = (double *) totals;
	
	int num_iters = sizeof(struct reading_memory_type)*NUM_FACES/sizeof(double);
	for (int i=0; i<num_iters; i++)
		averages_ptr[i] = totals_ptr[i]/divisor;	
}
/*
 * main routine to statically calibrate sensors 
 */

void static_calibration(struct mpu60x0_stateType *mpu60x0_state)
{
	struct reading_memory_type reading_memory[NUM_FACES] = {{0}};
	struct reading_memory_type totals[NUM_FACES] = {{0}};
	struct reading_memory_type averages[NUM_FACES] = {{0}};
	
	//initial dodgey values
	for (int i=0; i<NUM_FACES; i++)
		mpu60x0_get_reading( shape_cs_mappings[i], mpu60x0_state[i], &(reading_memory[i]) );
	
	double orientations[NUM_FACES][sizeof(struct reading_memory_type)/sizeof(double)];
	find_orientation(reading_memory, orientations);
	
	//make sure to send initial cal values
	uart_puts("begin initial calibration values\n");
	uart_put_readings(reading_memory, NUM_FACES);
	uart_puts("end initial calibration values\n");
	
	long unsigned int divisor = 0;
	
	while (1)
	{
		//start main averaging loop	
		for (int i=0; i<NUM_FACES; i++)
		{
			divisor++;
			
			mpu60x0_get_reading( shape_cs_mappings[i], mpu60x0_state[i], &(reading_memory[i]) );
			
			sum_results(totals, reading_memory);
			
			compute_averages(averages, totals, (double) divisor);
			
			printf("divisor: %f\n", divisor);
			uart_puts("readings:\n");
			uart_put_readings(reading_memory, NUM_FACES);
			uart_puts("averages:\n");			
			uart_put_readings(averages, NUM_FACES);
			
			/*struct vector* result = NULL;
			struct vector* values = NULL;*/
			
			/* get velocity update */
			/*result = (struct vector *) &(reading_memory[i].v_x);
			values = (struct vector *) &(reading_memory[i].a_x);
			integrate_vector(result, values);*/
			
			/* get position update */
			/*result = (struct vector *) &(reading_memory[i].x);
			values = (struct vector *) &(reading_memory[i].v_x);
			integrate_vector(result, values);*/
			
			/* get orientation update */
			/*result = (struct vector *) &(reading_memory[i].o_x);
			values = (struct vector *) &(reading_memory[i].w_x);
			integrate_vector(result, values);*/			
		}
	}
	
}
