/*!	\file systimer.h
 * 	\brief HAL system timers API
 * 	
 * 	Provides functions to read system time. Also provides interrupt driven timers which will 'call you back' when the timeout expires :D
 * 	\todo the timers are untested. Reading system time works fine though xP
 */

#ifndef __SYSTIMER_H__
#define __SYSTIMER_H__

/*
 * Interrupt based timeouts have not been implemented
 */

#include "support.h"
#include "error.h"

unsigned int systimer_get_32bit_time ( void );
//long unsigned int systimer_get_64bit_time ( void );

/* initialise hardware timers for use */
int systimer_init();

/* up to 4 timers can be used at once */
int systimer_alloc ( int timeout );

/* returns whether the timer has timed out or not */
int systimer_has_timed_out( int timer );

/* releases a timer */
void systimer_free( int timer );

#endif /* __SYSTIMER_H__ */
