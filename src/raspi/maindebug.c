
#include <string.h>

#include "calibrate.h"
#include "mpu60x0.h"
#include "uart.h"
#include "systimer.h"

#include "config.h"

int flag;

void main_debug (struct mpu60x0_stateType *mpu60x0_state)
{
	struct reading_memory_type reading_memory[NUM_FACES] = {{0}}; /* = malloc(read_rate*read_time*sizeof(struct reading_memory_type)*NUM_FACES); */
	
	while(1){
		//if ( falling edge detected say so )
		if (flag)
		{
			uart_puts("got int!\n");
		}
		if (1)
		{			
			//flag = 0;
			
			for (int i=0; i<NUM_FACES; i++)
			{
				mpu60x0_get_reading_raw( shape_cs_mappings[i], &(reading_memory[i]) );

				struct vector* result = NULL;
				struct vector* values = NULL;
				
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
				
				/*char tmp[100];			
				sprintf(tmp, "dev: %d, xAccel: %+06f, yAccel: %+06f, zAccel: %+06f, temp: %+06f, xGyro: %+06f, yGyro %+06f, zGyro %+06f\r\n", \
					i, reading_memory[i].a_x, reading_memory[i].a_y, reading_memory[i].a_z, reading_memory[i].temp, \
					reading_memory[i].w_x, reading_memory[i].w_y, reading_memory[i].w_z);
			
				uart_puts((unsigned char*) tmp);*/
			}		
						
			uart_put_readings(reading_memory, NUM_FACES);
		
			/*char chunk[500];
			
			uart_putc(0x02); //start of text (reading)
			uart_putc(NUM_FACES);
			
			for (int i=0; i<sizeof(reading_memory); i++)
			{
				char* cur_values = (char*) reading_memory;
				sprintf(chunk, "%02X", cur_values[i]);
				uart_puts(chunk);
			}
			uart_putc(0x03); //end of text (reading)
			uart_putc('\r');
			uart_putc('\n');*/
			
			/*gpio_set_output_level(16, GPIO_OUTPUT_HIGH);
			wait(1000*1000);
			gpio_set_output_level(16, GPIO_OUTPUT_LOW);
			wait(1000*1000);*/
		}
		
		wait(1); //WTF???!!!
	}
}
