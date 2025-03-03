
/*
 *	name:   counter.c
 *	prefix: cnt_
 *
 *	this module implements a counter input port. this can be used,
 *	for instance, to count the number of axels a train has.
 *
 *	'API':
 *	------
 *	cnt_state()      : the interrupt handler
 *	cnt_config()     : configuring the port
 *	cnt_setbdelay()  : setting the bunce delay
 *	cnt_setpositive(): set the trigger (count) edge
 *	cnt_count()      : reading the count value
 *	cnt_reset()      : resetting the counter
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "counter.h"

void cnt_state( CNT *cnt )
     {
        int value, old = cnt->value;
        value = hwp_read( cnt->port_id );
	if ( cnt->bcount <= 0 && value != old )
	{
	   cnt->value  = value;
	   cnt->bcount = cnt->bdelay;
	   cnt->count += cnt->positive == value;
	}
	if ( cnt->bcount > 0 )
	   cnt->bcount--;
     }

int cnt_config( CNT *cnt )
    {
	cnt->value    = hwp_read( cnt->port_id );
	cnt->positive = 1;
	cnt->count    = cnt->bcount = 0;
	cnt->bdelay   = 10;
	return swp_register( cnt_state, cnt );
    }

