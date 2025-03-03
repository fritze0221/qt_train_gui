
/*
 *	name:   sp.c
 *	prefix: sp_
 *
 *	this modules implements the very simple on/off port
 *
 *	'API':
 *	------
 *	sp_state() : the interrupt handler
 *	sp_config(): configuring the port, i.e., address etc.
 *	sp_on()	   : setting the output port to 1
 *	sp_off()   : setting the output port to 0
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "sp.h"

#define NONE		0		// for new_state only
#define ON		1		// output is on (set)
#define OFF		2		// output port is off (clear)

void sp_state( SPP sp )
     {
	if ( sp->new_state )
	{
	   sp->state     = sp->new_state;
	   sp->new_state = NONE;
	   hwp_write( sp->port_id, sp->state == ON );
	}
     }

int sp_config( SPP sp )
    {
	sp_off( sp );
	sp_state( sp );
	return swp_register( sp_state, sp );
    }

void sp_on ( SPP sp ){ sp->new_state = ON;  }
void sp_off( SPP sp ){ sp->new_state = OFF; }

