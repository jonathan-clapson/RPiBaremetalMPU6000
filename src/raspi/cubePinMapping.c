/*!	\file cubePinMapping.c
 * 	\brief This file stores information about the mapping of sensors when they are mounted to the cube block.
 * 
 * 	The necessary information to describe a sensor configuration should be described by this file in conjunction iwth cubePinMapping.h
 */

#include <stdint.h>
#include <math.h>

#include "gpio.h"
#include "cubePinMapping.h"
#include "config.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767837
//#define PI (4*atan(1))

/*!	\var shape_cs_mappings[NUM_FACES]
 * 	\brief This describes the order in which sensor boards (via chipselects) are mapped into device id's.
 * 	In this case device id 0 is attached to ChipSelect1 
 * 	device id 1 is attached to ChipSelect2 etc...
 */
char shape_cs_mappings[NUM_FACES] = { CS1, CS2, CS3, CS4, CS5, CS6 };

/*!	\fn void shape_set_cs()
 *	\brief This function initialises each ChipSelect pin.
 * 
 * 	It sets each needed pin to operate as an output pin and then sets the output level to logic high (chip select inactive)
 */
void shape_set_cs()
{		
	gpio_function_select(CS1, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS2, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS3, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS4, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS5, GPIO_FUNC_OUTPUT);
	gpio_function_select(CS6, GPIO_FUNC_OUTPUT);
	
	gpio_set_output_level(CS1, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS2, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS3, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS4, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS5, GPIO_OUTPUT_HIGH);
	gpio_set_output_level(CS6, GPIO_OUTPUT_HIGH);	
}

/*! 	\var shape_accel_calibration[NUM_FACES][3]
 *	\brief These are the offsets that need to be removed to reduce error in a static configuration.
 * 
 * 	These calibration values were obtained by mounting the cube in a vice and then running the calibration routine. The +/- 1.0 that is added to some values indicated that axes had gravity acting on it which is not an error.
 */
const double shape_accel_calibration[NUM_FACES][3] = {
	/*{ -0.017555, -0.017217, -1.063714 },
	{ -0.993583, 0.014068, -0.014955 },
	{ 0.072908, -0.997426, 0.333442 },
	{ -1.007712, 0.003051, -0.019994 },
	{ 0.037019, -0.008997, 0.901934 },
	{ 0.045009, -1.008446, -0.021656 }*/
	{ -0.017555, -0.017217, -1.063714+1.0 },
	{ -0.993583+1.0, 0.014068, -0.014955 },
	{ 0.072908, -0.997426+1.0, 0.333442 },
	{ -1.007712+1.0, 0.003051, -0.019994 },
	{ 0.037019, -0.008997, 0.901934-1.0 },
	{ 0.045009, -1.008446+1.0, -0.021656 }
};

/*! 	\var shape_gyro_calibration[NUM_FACES][3]
 *	\brief These are the offsets that need to be removed to reduce error in a static configuration.
 */
const double shape_gyro_calibration[NUM_FACES][3] = { 
	{ 0.243653, -1.563710, -0.439477},
	{ -2.275887, 1.257458, -3.695503},
	{ 1.263745, 1.628734, 0.977232},
	{ 4.296016, 1.591442, -1.437371},
	{ 1.181315, 1.856458, -3.728455},
	{ -1.392942, 0.552481, -1.313207}	
};

/*! 	\var shape_temp_calibration[NUM_FACES]
 *	\brief These are stored here from calibration, I'm not sure if there is actually a use for them.
 */
const double shape_temp_calibration[NUM_FACES] = {
	23.664684,
	25.232215,
	24.427723,
	23.359746,
	23.085322,
	24.694337
};

/*! 	\var shape_orientation[NUM_FACES][3]
 *	\brief Describes how to rotate each face into a global x,y,z coordinate system
 * 
 * 	X Y and Z rotations must be done in XYZ order.
 */
const double shape_orientation[NUM_FACES][3] = {
	  {PI/2,-PI/2,0},
	  {-PI/2,0,-PI/2},
	  {0,PI,0},
	  {PI/2,0,-PI/2},
	  {-PI/2,PI/2,0},
	  {0,0,0}
};
