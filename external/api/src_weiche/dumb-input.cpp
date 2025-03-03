
/*
 *	name:   dumb-input.c
 *	prefix: din_
 *
 *	this module implements the very simple input port. once it is
 *	configured, it reads the input value from the physical port and
 *	updates both, value and value_changes
 *
 *	'API':
 *	------
 *	din_config(): configure the port
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "dumb-input.h"

static void din_state( DIN *inp )
       {
	  int old = inp->value;
	  inp->value_changed = (inp->value = hwp_read( inp->port_id )) != old;
       }

int din_config( DIN *inp, int port_id )
    {
	inp->port_id = port_id;
	return swp_register( din_state, inp );
    }


