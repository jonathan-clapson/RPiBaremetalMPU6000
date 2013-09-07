#include "support.h"
#include "error.h"

#include "systimer.h"

#define SYSTIMER_NUM_TIMERS 4

const int sysclk_freq = 250e6;

const volatile unsigned int SYSTIMER_BASE = 0x20003000;

const volatile unsigned int SYSTIMER_CS	= 0x20003000; /* System Timer Control/Status */
const volatile unsigned int SYSTIMER_CLO = 0x20003004;	/* System Timer Counter Lower */
const volatile unsigned int SYSTIMER_CHI = 0x20003008;	/* System Timer Counter Higher */
const volatile unsigned int SYSTIMER_C0 = 0x2000300c;	/* System Timer Compare 0 */
const volatile unsigned int SYSTIMER_C1 = 0x20003010;	/* System Timer Compare 1 */
const volatile unsigned int SYSTIMER_C2 = 0x20003014;	/* System Timer Compare 2 */
const volatile unsigned int SYSTIMER_C3 = 0x20003018; /* System Timer Compare 3 */

int timers[SYSTIMER_NUM_TIMERS]; /* a bitflag based char could be used for better memory efficiency, this is nicer to read */

unsigned int systimer_get_32bit_time ( void )
{
	//return read_mmap_register(SYSTIMER_CLO);
	unsigned int x = *(unsigned int *)SYSTIMER_CLO;
	return x;
}

long unsigned int systimer_get_64bit_time ( void )
{
	/* 
	 * FIXME: i am unsure how to implement this as I have yet to work out how the timer increments
	 * there is no way to pause the free running timer but it must be read in two reads :S 
	 * this means it is necessary to read it twice to ensure that any rollover of the lower 32bits is accounted for
	 */
	 return 0;
}

/* functions below here are untested */

/* 
 * initialise hardware timer use 
 */
int systimer_init()
{
	for (int i=0; i<SYSTIMER_NUM_TIMERS; i++)
		timers[i] = 0;
	return ERR_NOERR;	
}

/* 
 * systimer_alloc is used to set a timeout in hardware 
 * up to 4 timers can be used at once
 */
int systimer_alloc ( int timeout )
{	
	//find an unused timer
	int timer = -1;	
	for (int i=0; i<SYSTIMER_NUM_TIMERS; i++)
	{
		if (timers[i] == 0)
		{
			timer = i;
			break;
		}
	}
	
	//if timer is still -1, none are free, return an error
	if (timer == -1) return -1;
	
	//set timer to in use
	timers[timer] = 1;
	
	//set timeout
	int current_time = systimer_get_32bit_time();
	write_mmap_register(SYSTIMER_C0 +4*timer, current_time + timeout);
	
	//reset timeout flag
	write_mmap_register( SYSTIMER_CS, (1<<timer) );
	
	return timer;
}

/* 
 * returns whether the timer has timed out or not 
 */
int systimer_has_timed_out( int timer )
{
	int status = read_mmap_register(SYSTIMER_CS);
	
	if ( status & (1<<timer) )
		return 1;
		
	return 0;	
}

/*
 * releases a timer
 */
void systimer_free( int timer )
{
	timers[timer] = 0;	
}
