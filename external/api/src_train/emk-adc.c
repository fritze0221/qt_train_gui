
/*
 *	name:   emk-adc.c
 *	prefix: emk_
 *
 *	this module is a variation of the regular A/D-controller.
 *	it is intendet to care or the pecularities of an electrical
 *	motor for which we want to measure (estimate) its current 
 *	speed. to this end, it constantly monitors the motor output.
 *	in case the output is zero (off, the motor is idle), it
 *	samples its input, after the specified wait tics have 
 *	expired. these wait tics are due to the additional capacitor
 *	the requires a settling time in order to reach a dynamic
 *	equillibrium. thus, this module should be usedin the context
 *	of a motor control.
 *
 *	'API':
 *	------
 *	emk_state  (): the interrupt handler
 *	emk_config (): configuring the port
 *	emk_set    (): setting wait_tics and repeat
 *	emk_reset  (): reset value and cnt
 *	emk_average(): calculate the average and reset the values
 *	emk_emks   (): how often were samples taken
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "emk-adc.h"

#define NONE		0	// for new_state only
#define READ		1	// reading out the current input value

//	handling the A/D-controller

void emk_state( EMK *emk )
     {
	int i;
	if ( hwp_read( emk->motor_id ) )
	   emk->wait_cnt = emk->wait_tics;
	else if ( emk->wait_cnt )
		emk->wait_cnt--;
	if ( emk->wait_cnt <= 0 )
	   for( i = emk->repeat, emk->cnt += i, emk->emks++; i > 0; i-- )
	      emk->value += hwp_read( emk->port_id );
     }

int emk_config( EMK *emk, int motor_id )
    {
       emk->motor_id  = motor_id;
       emk->repeat    = 1;
       emk->wait_tics = 1;
       emk_reset  ( emk );
       emk_average( emk );
       return swp_register( emk_state, emk );
    }

void emk_set( EMK *emk, int wait_tics, int repeat )
     {
        emk->repeat    = repeat;
        emk->wait_tics = wait_tics;
     }

void emk_reset( EMK *emk )
     {
        emk->cnt = emk->value = emk->emks = 0;
     }

int emk_average( EMK *emk )
    {
       emk->average = (emk->cnt == 0)? 0: emk->value / emk->cnt;
       emk_reset( emk );
       return emk->average;
    }

