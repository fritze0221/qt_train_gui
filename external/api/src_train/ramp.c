
/*
 *	ramp.c
 *	prefix: rmp_
 *
 *	this simple module provides a continuous adaptation of the 
 *	target speed over time.
 *
 *	'API':
 *      ------
 *	rmp_newramp() : set new ramp values
 *	rmp_take()    : setup the ramp data structure
 *	rmp_inew()    : setup a new ramp in interrupt mode
 *	rmp_step()    : update the data structure after each tic
 *	rmp_expired() : whether or not the ramp-object has expired
 *	rmp_active()  : i.e., ! rmp_expired()
 *      rmp_isnew()   : testing for new ramp values
 *	rmp_duration(): duration of the ramp execution
 *
 *	NOTES:
 *	------
 *	rmp_newramp():
 *	   should be called by 'background' processes.
 *	   the new values are temporarily stored and taken during
 *	   interrupt handling by rmp_take().
 *
 *	rmp_inew():
 *	   should be called during interrupt handling, e.g.,
 *	   tc_state(), since the new values are directly taken within
 *	   this function call.
 *
 */

#include "ramp.h"

int rmp_newramp( RMP *rmp, int new_from, int new_to, int new_duration )
    {
       rmp->valid = 0;
       rmp->new_from     = new_from;
       rmp->new_to       = new_to;
       rmp->new_duration = new_duration;
       return rmp->valid = 1;
    }

static void rmp_setup( RMP *rmp, int from_target, int to_target, int duration )
       {
	  int speed_dif = to_target - from_target;
	  (duration < 0) && (duration = 0);
	  rmp->current_target = (duration)? from_target: to_target;
	  rmp->increment      = (speed_dif < 0)? -1: 1;
	  rmp->delta_target   = (speed_dif < 0)? - speed_dif: speed_dif;
	  rmp->delta_tics     = duration;
	  rmp->tics           = duration;	// init tics counter
	  rmp->accumulator    = 0;		// init accumulator
	  rmp->valid          = 0;
       }

int rmp_inew( RMP *rmp, int from_target, int to_target, int duration )
    {
       rmp_setup( rmp, from_target, to_target, duration );
       return 1;
    }

int rmp_take( RMP *rmp )
    {
       int valid = rmp->valid;
       if ( valid )
	  rmp_setup( rmp, rmp->new_from, rmp->new_to, rmp->new_duration );
       return valid;
    }

//	last call yields zero, but still updates target_speed
int rmp_step( RMP *rmp, int *new_target )
    {
       if ( rmp->tics < 0 )
          return 0;
       *new_target = rmp->current_target;
       if ( rmp->tics-- > 0 )
	  for( rmp->accumulator += rmp->delta_target;
		  rmp->accumulator >= rmp->delta_tics;
		     rmp->accumulator -= rmp->delta_tics )
	     rmp->current_target += rmp->increment;
       return rmp->tics >= 0;
    }

