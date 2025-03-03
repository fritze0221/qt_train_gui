
/*
 *	name:   dpwm.c
 *	prefix: dpwm_
 *
 *	this modules implements the classical PWM (puls-width modulation)
 *	functionality.
 *
 *	'API':
 *	------
 *	dpwm_state() : the interrupt handler
 *	dpwm_config(): configuring the port, i.e., address etc.
 *	dpwm_set()   : setting the number of on- and off-tics
 *	dpwm_start() : starting PWM
 *	dpwm_stop()  : stopping PWM; keeping tics
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "tics.h"
#include "dpwm.h"

#define NONE		0	// for new_state only
#define INACTIVE	1	// pwm is inactive, and port is off
#define ACTIVE		2	// pwm is running, and output is both on and off

void dpwm_state( DPWM *pwm )
     {
        int sw_on;
        tics_take( & pwm->sw_tics );
        tics_take(   pwm->hw_tics + 1 );
        tics_take(   pwm->hw_tics + 0 );
        if ( pwm->new_state == INACTIVE )
	   hwp_off( pwm->port_id );
        if ( pwm->new_state == ACTIVE )
        {
	   tics_start(   pwm->hw_tics + 1 );
	   tics_start(   pwm->hw_tics + 0 );
	   tics_start( & pwm->sw_tics );
	   tics_next ( & pwm->sw_tics );
        }
        pwm->new_state && (pwm->state = pwm->new_state);
        pwm->new_state = NONE;
        if ( pwm->state == ACTIVE )
        {
	   sw_on = pwm->sw_tics.output;
	   if ( tics_expired( pwm->hw_tics + sw_on ) )
	   {
	      tics_start( pwm->hw_tics + sw_on );	// restart hw-tics
	      sw_on = tics_next( & pwm->sw_tics );	// advance sw-tics
	   }
	   hwp_write( pwm->port_id, tics_next( pwm->hw_tics + sw_on ) );
        }
     }

int dpwm_config( DPWM *pwm, int port_id )
    {
	pwm->port_id = port_id;
	dpwm_set  ( pwm, 1, 1, 1, 0, 0, 1 );
	dpwm_stop ( pwm );
	dpwm_state( pwm );
	return swp_register( dpwm_state, pwm );
    }

void dpwm_set( DPWM *pwm, int sw_on   , int sw_off, int high_on,
		          int high_off, int low_on, int low_off )
     {
	tics_regular  ( & pwm->sw_tics    ,   sw_on,   sw_off );
	tics_bresenham(   pwm->hw_tics + 1, high_on, high_off );
	tics_bresenham(   pwm->hw_tics + 0,  low_on,  low_off );
     }

void dpwm_start( DPWM *pwm ){ pwm->new_state = ACTIVE; }
void dpwm_stop ( DPWM *pwm ){ pwm->new_state = INACTIVE; }



