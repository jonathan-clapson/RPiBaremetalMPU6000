
#include <string.h>

#include "calibrate.h"
#include "mpu60x0.h"
#include "uart.h"
#include "systimer.h"
#include "integrate.h"

#include "config.h"

//was interrupt flag, maybe get rid of?
int flag;

/* 
 * chip 0 = bottom
 * chip 1 = left
 * chip 2 = front
 * chip 3 = right
 * chip 4 = top
 * chip 5 = back
 */

void applyCalibration(struct reading_memory_type *readings)
{	
	for (int j=0; j<NUM_FACES; j++)
	{		
		double *sen_accels = (double *) &(readings[j].a_x);
		double *sen_gyros = (double *) &(readings[j].w_x);
		
		for(int i=0; i<3; i++){
			sen_accels[i] -= shape_accel_calibration[j][i];
			sen_gyros[i] -= shape_gyro_calibration[j][i];		
		}		
	}	
	
	/* need to do something with temp calibration */
	//sensor->temp -= temp_calibration[sensor_num]
}

/* FIXME: don't read this or try to understand it, it blew joshes mind. Replace this with real rotation code (maybe matrices, quarternion etc?) */
void rotate_to_global(struct reading_memory_type *readings)
{
	/*
	 * x = 1, -1
	 * y= 2, -2
	 * z= 3, -3
	 */
	const int cube_mapping[NUM_FACES][3] = {
		{2, -3, -1}, //bottom
		{-3, -1, 2 }, //left
		{1, -2, -3 }, //front
		{3, -1, -2 }, //right
		{2, 3, 1 }, //top
		{-1, -2, 3 } //back
	};
	
	for(int i=0; i<6; i++){
		double o[3] = {readings[i].o_x, readings[i].o_y, readings[i].o_z};
		double *ptr = (double *) &(readings[i].o_x);
		
		for (int j=0; j<3; j++)
		{
			switch (cube_mapping[i][j])
			{
				case 1:	case 2: case 3: case -1: case -2: case -3:
					break;
				default:
					printf("cube mappings are so broken the world might explode!\n");
					return;
			}
			/* if you don't know what this does, look up c ternary operator */
			int index = cube_mapping[i][j]<0 ? cube_mapping[i][j]*-1.0 : cube_mapping[i][j];
			
			ptr[j] = cube_mapping[i][j]<0 ? -1.0*o[index-1] : o[index-1];			
		}
	};	
}

void main_readings (struct mpu60x0_stateType *mpu60x0_state)
{
	struct reading_memory_type current_readings[NUM_FACES] = {{0}}; /* = malloc(read_rate*read_time*sizeof(struct reading_memory_type)*NUM_FACES); */	
	struct reading_memory_type last_readings[NUM_FACES] = {{0}};
	
	int run_counter[NUM_FACES] = {0};
	
	while(1) {
		
		/* copy reading memory to last readings */
		/*printf("current readings\n");
		uart_put_readings(current_readings, NUM_FACES);*/
		
		memcpy(last_readings, current_readings, sizeof(struct reading_memory_type)*NUM_FACES);
		/*printf("update last readings\n");
		uart_put_readings(last_readings, NUM_FACES);*/
		
		/* grab readings from all sensors */
		for (int i=0; i<NUM_FACES; i++)
		{					
			mpu60x0_get_reading( shape_cs_mappings[i], mpu60x0_state[i], &(current_readings[i]) );
		}
		
		applyCalibration(current_readings);
		
		/* update integration for all sensors */
		for (int i=0; i<NUM_FACES; i++)
		{			
			/* ISO_80000-3 states gravity is  9.80665 m/s2 */			
			current_readings[i].a_x *= 9.80665;
			current_readings[i].a_y *= 9.80665;
			current_readings[i].a_z *= 9.80665;
			
			/*printf("currentReadings: %d\n", i);
			uart_put_readings(current_readings, 1);
			
			current_readings[i].w_x = 1;
			current_readings[i].w_y = 1;
			current_readings[i].w_z = 1;
			
			last_readings[i].w_x = 2;
			last_readings[i].w_y = 2;
			last_readings[i].w_z = 2;
			last_readings[i].a_x = 2;
			last_readings[i].a_y = 2;
			last_readings[i].a_z = 2;*/
			
			unsigned int timestep = current_readings[i].timestamp - last_readings[i].timestamp;
			//unsigned int timestep = 1;
			
			//need one cycle to get previous readings initialised
			if (run_counter[NUM_FACES] < 5)
			{
				run_counter[NUM_FACES]++;
				continue;
			}

			struct vector* result = NULL;
			struct vector* current_values = NULL;
			struct vector* prev_values = NULL;
			
			/* get velocity update */
			//printf("vel\n");
			result = (struct vector *) &(current_readings[i].v_x);
			current_values = (struct vector *) &(current_readings[i].a_x);
			prev_values = (struct vector *) &(last_readings[i].a_x);			
			integrate_vector(result, current_values, prev_values, timestep);
						
			/* get position update */
			//printf("dist\n");
			result = (struct vector *) &(current_readings[i].x);
			current_values = (struct vector *) &(current_readings[i].v_x);
			prev_values = (struct vector *) &(last_readings[i].v_x);
			integrate_vector(result, current_values, prev_values, timestep);
			
			/* get orientation update */
			//printf("orient\n");
			result = (struct vector *) &(current_readings[i].o_x);
			current_values = (struct vector *) &(current_readings[i].w_x);
			prev_values = (struct vector *) &(last_readings[i].w_x);			
			integrate_vector(result, current_values, prev_values, timestep);
			
			/*char tmp[100];			
			sprintf(tmp, "dev: %d, xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
				i, reading_memory[i].a_x, reading_memory[i].a_y, reading_memory[i].a_z, reading_memory[i].temp, \
				reading_memory[i].w_x, reading_memory[i].w_y, reading_memory[i].w_z);
		
			uart_puts((unsigned char*) tmp);*/
		}
		
		struct reading_memory_type swapped_orientations[NUM_FACES];
		memcpy(swapped_orientations, current_readings, sizeof(struct reading_memory_type)*NUM_FACES);
		rotate_to_global(swapped_orientations);
					
		uart_put_readings(swapped_orientations, NUM_FACES);
	}
}
