
/*
 *	name:   stics.c
 *	prefix: stics_
 *
 *	this modules provides tics and their functionality
 *
 *	'API':
 *	------
 *	stics_newtics()	set new tics value
 *	stics_take()	take the new value as current value
 *	stics_isnew()	testing for new tics values
 *	stics_start()	start the tics counter
 *      stics_vstart()  start the tics counter with a given start value
 *	stics_stop()	stop tics counter
 *	stics_step()	decrease counter by 1
 *	stics_expired()	has the tics counter expired?
 *	stics_counter()	get the tics counter value
 */

#include "stics.h"

int stics_newtics( STIP tip, int tics )
    {
       tip->valid    = 0;	   // invalidating preivous, non-taken calls
       tip->new_tics = (tics < 0)? 0: tics;		// assure tics  >= 0
       return tip->valid = 1;
    }

int stics_take( STIP tip )
    {
       (stics_isnew( tip )) && (tip->cnt = tip->tics = tip->new_tics);
       return tip->valid = 0;
    }

