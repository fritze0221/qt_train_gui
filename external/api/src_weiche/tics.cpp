
/*
 *	name:   tics.c
 *	prefix: tics_
 *
 *	this modules provides tics and their functionality.
 *	in addition to regular tics, it also provides an
 *	algorithm that is inspired by bresenham. that is,
 *	rather than generating 11100, it generates 10101....
 *
 *	'API':
 *	------
 *      tics_newtics()  : set new tics values
 *      tics_take()     : take the new values as current values
 *      tics_isnew()    : testing for new tics values
 *      tics_start()    : start the tics counter
 *      tics_vstart()   : start the tics counter with a given start value
 *      tics_stop()     : stop tics counter
 *      tics_output()   : tics in state, i.e., on or off?
 *      tics_ontics()   : tics on-value
 *      tics_offtics()  : tics off-value
 *      tics_expired()  : has the tics counter expired?
 *      tics_counter()  : get the tics counter value
 *      tics_bresenham(): tics_newtics() and sets mode to bresenhame
 *      tics_regular()  : tics_newtics() and sets mode to regular (PWM)
 *	tics_1ahead()   : looking one next-call ahead
 *
 */

#include "tics.h"

int tics_newtics( TIP tip, int bresenham, int on_tics, int off_tics )
    {
       tip->valid        = 0;	      // invalidating preivous, not-taken calls
       tip->new_on_tics  = ( on_tics < 0)? 0:  on_tics;     // assure  on >= 0
       tip->new_off_tics = (off_tics < 0)? 0: off_tics;     // assure off >= 0
       tip->new_mode     = bresenham;
       return tip->valid = 1;
    }

int tics_take( TIP tip )
    {
       if ( tip->valid )
       {
	  tip->accu     = 0;
	  tip->on_tics  = tip->new_on_tics;
	  tip->off_tics = tip->new_off_tics;
	  tip->mode     = tip->new_mode;
	  tip->periode  = tip->on_tics + tip->off_tics;
	  tip->cnt      = tip->periode;
	  tip->output   = 0;	// value actually not vaild --> tics_next()
       }
       tip->valid = 0;
       return 1;
    }

int tics_next( TIP tip )
    {
       if ( tics_expired( tip ) )
          tics_start( tip );
       if ( tip->mode )			// i.e., bresenham
	  if (tip->output = (tip->accu < tip->on_tics))
	       tip->accu += tip->off_tics;
	  else tip->accu -= tip->on_tics;
       else tip->output = tip->cnt > tip->off_tics;
       tip->cnt--;
       return tip->output;
    }

int tics_1ahead( TIP tip )
    {
	  return (tip->mode)? tip->accu < tip->on_tics
                 :(tics_expired( tip ))? tip->on_tics>0: tip->cnt>tip->off_tics;
    }



