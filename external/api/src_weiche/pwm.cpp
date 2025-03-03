
/*
 *	name:   pwm.c
 *	prefix: pwm_
 *
 *	this modules implements the classical PWM (puls-width modulation)
 *	functionality.
 *
 *	'API':
 *	------
 *	pwm_state() : the interrupt handler
 *	pwm_config(): configuring the port, i.e., address etc.
 *	pwm_set()   : setting the number of on- and off-tics
 *      pwm_start() : starting PWM
 *      pwm_stop()  : stopping PWM, keeping tics
 *	pwm_full()  : setting PWM to constant 1
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "tics.h"
#include "pwm.h"

#define NONE		0	// for new_state only
#define INACTIVE	1	// pwm is inactive, and output is off
#define ACTIVE		2	// pwm is active, and output is both on and off
#define FULL		3	// pwm is running at full power --> mc == 1

void pwm_state( PWM *pwm )
     {
        tics_take( & pwm->tics );
        if ( pwm->new_state == INACTIVE )
	   hwp_off( pwm->port_id );
        if ( pwm->new_state == ACTIVE )
	   tics_start( & pwm->tics );
        if ( pwm->new_state == FULL )
	   hwp_on( pwm->port_id );
        pwm->new_state && (pwm->state = pwm->new_state);
        pwm->new_state = NONE;
        if ( pwm->state == ACTIVE )
	   hwp_write( pwm->port_id, tics_next( & pwm->tics ) );
     }

int pwm_config( PWM *pwm, int port_id )
    {
	pwm->port_id = port_id;
	pwm_set  ( pwm, 1, 0, 1 );
	pwm_stop ( pwm );
	pwm_state( pwm );
	return swp_register( pwm_state, pwm );
    }

void pwm_set( PWM *pwm, int bresenham, int on_tics, int off_tics )
     {
        tics_newtics( & pwm->tics, bresenham, on_tics, off_tics );
     }

void pwm_start( PWM *pwm ){ pwm->new_state = ACTIVE; }
void pwm_stop ( PWM *pwm ){ pwm->new_state = INACTIVE; }
void pwm_full ( PWM *pwm ){ pwm->new_state = FULL; }

