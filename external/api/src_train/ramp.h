
/*
 *	ramp.h
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
 *	rmp_isnew()   : testing for new ramp values
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

typedef struct {
	  int valid;		// new target and duration values
	  int current_target;	// current target speed (after update)
	  int delta_target;	// abs( total amount of speed change )
	  int delta_tics;	// time duration for change
	  int accumulator;	// the bresenham accumulator
	  int increment;	// the speed increment, which is -1 or 1
	  int tics;		// the ramp time counter
	  int new_from;		// new from_target
	  int new_to;		// new to_target
	  int new_duration;	// new duration
	} RMP;

int rmp_newramp( RMP *rmp, int from_target, int to_target, int duration );
int rmp_take   ( RMP *rmp );
int rmp_inew   ( RMP *rmp, int from_target, int to_target, int duration );
int rmp_step   ( RMP *rmp, int *new_target );

#define rmp_expired(  rmp )	((rmp)->tics  < 0)
#define rmp_active(   rmp )	((rmp)->tics >= 0)
#define rmp_isnew(    rmp )	((rmp)->valid)
#define rmp_duration( rmp )	((rmp)->delta_tics)

