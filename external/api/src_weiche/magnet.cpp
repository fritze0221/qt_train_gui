
/*
 *	name:   magnet.c
 *	prefix: mag_
 *
 *	this module implements the handling of electric magnets,
 *	such as switches, that also provide feedback about their
 *	physical position.
 *
 *	'API':
 *	------
 *	mag_config() : configuring the port
 *	mag_trigger(): trigger the output port
 *	mag_tics()   : reset the number of on-tics
 *	mag_status() : port status, i.e., off, busy, alert
 *	mag_clear()  : reset the alert state
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "stics.h"
#include "dumb-input.h"
#include "magnet.h"

#define NONE		0		// for new_state only
#define INACTIVE	1		// monoflop is inactive
#define ACTIVE		2		// monoflop is active
#define ALERT		3		// monoflop expired without feedback

static int status[] = { 0, MAG_OFF, MAG_BUSY, MAG_ALERT };

static void mag_state( MAG *mag )
       {
	  stics_take( & mag->stics );
	  if ( mag->new_state == ACTIVE )
	     stics_start( & mag->stics );
	  if ( mag->new_state == INACTIVE )
	     hwp_off( mag->port_id );
	  (mag->new_state) && (mag->state = mag->new_state);
	  mag->new_state = NONE;
	  if ( mag->state == ACTIVE )
          {
	     if ( mag->feedback->value == 1 )		// end position reached
	        mag->state = INACTIVE;
	     else if ( stics_expired( & mag->stics ) )
	        mag->state = ALERT;
	     else stics_step( & mag->stics );
	     hwp_write( mag->port_id, mag->state == ACTIVE );
          }
       }

int mag_config( MAG *mag, int port_id, DIN *in_port, int on_tics )
    {
	mag->port_id  = port_id;
	mag->feedback = in_port;
	stics_newtics( & mag->stics, on_tics );
        hwp_off( port_id );
	return swp_register( mag_state, mag );
    }

void mag_trigger( MAG *mag )
     {
        mag->new_state = ACTIVE;
     }

void mag_tics( MAG *mag, int on_tics )
     {
	stics_newtics( & mag->stics, on_tics );
     }

int mag_status( MAG *mag )
    {
       return status[ mag->state ];
    }

void mag_clear( MAG *mag )
     {
        mag->new_state = INACTIVE;		// clear alert
     }



