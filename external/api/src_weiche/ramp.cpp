
/*
 *	ramp.c
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

#include "ramp.h"

RMP *rmp_set( RMP *rmp, int from_target, int to_target, int duration )
    {
       int speed_dif = to_target - from_target;
       (duration < 0) && (duration = 0);
       rmp->current_target = (duration)? from_target: to_target;
       rmp->increment      = (speed_dif < 0)? -1: 1;
       rmp->delta_target   = (speed_dif < 0)? - speed_dif: speed_dif;
       rmp->delta_tics     = duration;
       rmp->tics           = duration;		// init tics counter
       rmp->accumulator    = 0;			// init accumulator
       return rmp;
    }

//	last call yields zero, but still updates target_speed
int rmp_step( RMP *rmp, int *new_target )
    {
       *new_target = rmp->current_target;
       if ( rmp->tics-- > 0 )
	  for( rmp->accumulator += rmp->delta_target;
		  rmp->accumulator >= rmp->delta_tics;
		     rmp->accumulator -= rmp->delta_tics )
	     rmp->current_target += rmp->increment;
       return rmp->tics >= 0;
    }

