
/*
 *	name:   dir-input.c
 *	prefix: dir_
 *
 *	this module simulated a standard track contact, which
 *	indicates not only whether a track is occupied but also
 *	in which direction the train rides.
 *
 *	'API':
 *      ------
 *	dir_state()  : the interrupt handler
 *	dir_config() : configuring the port
 *	dir_value()  : port's current input value, but not the partner's one
 *	dir_changed(): has the direction changed?
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "dumb-input.h"
#include "dir-input.h"

void dir_state( LRIN *dir )
     {
	int old = dir->direction;
	int in_1 = dir->value = hwp_read( dir->port_id );
	int in_2 = din_value( dir->partner );
	if ( in_1 == 0 && in_2 == 0 )
	   dir->direction = DIR_OFF;
	if ( in_1 == 1 && in_2 == 0 )
	   dir->direction = DIR_LEFT;
	if ( in_1 == 0 && in_2 == 1 )
	   dir->direction = DIR_RIGHT;
        dir->changed = dir->direction != old;
     }

int dir_config( LRIN *dir, DIN *partner )
    {
       dir->partner   = partner;
       dir->direction = DIR_OFF;
       return swp_register( dir_state, dir );
    }

