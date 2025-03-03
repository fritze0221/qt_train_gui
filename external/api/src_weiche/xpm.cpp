
/*
 *	name:   xpm.c
 *	prefix: xpm_
 *
 *	this modules implements a PWM (puls-width modulation) whos
 *	on/off tics are controlled by a user-specified function. 
 *	this module can be thought of as a pwm-stream: at the
 *	end of every on/off-cycle, the xpm_state() function calls
 *	the user-specified function, mentioned above, to copy-in
 *	a pair of new on/off values.
 *
 *	'API':
 *	------
 *	xpm_state()  : the interrupt handler
 *	xpm__config(): configuring an xp port
 *	xpm_start()  : start the port operation
 *	xpm_stop()   : stop the port operation
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "tics.h"
#include "xpm.h"

#define NONE		0	// for new_state only
#define INACTIVE	1	// pwm is inactive, and port is off
#define ACTIVE		2	// pwm is running, and output is both on and off

void xpm_state( XPM *xpm )
     {
        int on, off;
        if ( xpm->new_state == INACTIVE )
	   hwp_off( xpm->port_id );
        if ( xpm->new_state == ACTIVE )
	   tics_stop( & xpm->tics );
        xpm->new_state && (xpm->state = xpm->new_state);
        xpm->new_state = NONE;
        if ( xpm->state == ACTIVE )
        {
	   if ( tics_expired( & xpm->tics ) )
	   {
	      if ( ! xpm->get_tics( xpm->data, & on, & off ) )
	      {
		 hwp_off( xpm->port_id );  // the stream has reached its end
		 xpm->state = INACTIVE;
		 return ;
	      }
	      tics_bresenham( & xpm->tics, on, off );	// sanitizing tics
	      tics_take     ( & xpm->tics );
	   }
	   hwp_write( xpm->port_id, tics_next( & xpm->tics ) );
        }
    }

int xpm__config( XPM *xpm, int port_id, XPM_NXT get_tics, void *data )
    {
	xpm->port_id  = port_id;
	xpm->get_tics = get_tics;
	xpm->data     = data;
	xpm_stop ( xpm );
	xpm_state( xpm );
	return swp_register( xpm_state, xpm );
    }

void xpm_start( XPM *xpm ){ xpm->new_state = ACTIVE; }
void xpm_stop ( XPM *xpm ){ xpm->new_state = INACTIVE; }



