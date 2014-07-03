/*!	\file maindebug.c
 * 	\brief This routine continuously takes 'raw' readings and writes them to the uart.
 * 	Check mpu60x0.c for details on the raw reading function
 */
 
#include <string.h>

#include "calibrate.h"
#include "mpu60x0.h"
#include "uart.h"
#include "systimer.h"

#include "config.h"

int flag;

/*!	\fn void main_debug (struct mpu60x0_stateType *mpu60x0_state)
 *	\brief Main routine for obtaining raw sensor data
 * 	
 * 	This routine continuously takes 'raw' readings and writes them to the uart.
 * 
 *	@param[in] mpu60x0_state the current configuration of each sensor (this is an array of mpu60x0_stateType for each sensor to be used in this function) 
 */
void main_debug (struct mpu60x0_stateType *mpu60x0_state)
{
	struct reading_memory_type reading_memory[NUM_FACES] = {{0}};
	
	while(1){
		if (flag)
		{
			uart_puts("got int!\n");
		}
		if (1)
		{	
			for (int i=0; i<NUM_FACES; i++)
			{
				mpu60x0_get_reading_raw( shape_cs_mappings[i], &(reading_memory[i]) );
			}								
			uart_put_readings(reading_memory, NUM_FACES);
		}
		
		wait(1); //WTF???!!!
	}
}
