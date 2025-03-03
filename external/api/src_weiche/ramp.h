
/*
 *	ramp.h
 *	prefix: rmp_
 *
 *	this simple module provides a continuous adaptation of the 
 *	target speed over time.
 *
 *	'API':
 *      ------
 *	rmp_set (): setup the ramp data structure
 *	rmp_step(): update the data structure after each tic
 *
 */

typedef struct {
	  int current_target;	// current target speed (after update)
	  int delta_target;	// abs( total amount of speed change )
	  int delta_tics;	// time duration for change
	  int accumulator;	// the bresenham accumulator
	  int increment;	// the speed increment, which is -1 or 1
	  int tics;		// the ramp time counter
	} RMP;

RMP *rmp_set ( RMP *ramp, int from_target, int to_target, int duration );
int  rmp_step( RMP *rmp, int *new_target );


