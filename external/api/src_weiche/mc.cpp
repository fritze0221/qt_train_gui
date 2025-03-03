
/*
 *	name:   mc.c
 *	prefix: mc_
 *
 *	this modules implements the rather complex motor control port.
 *
 *	'API':
 *	------
 *	mc_config()   : configuring the ports, i.e., motor output and adc input
 *	mc_adcconfig(): configuring the A/D controller
 *	mc_pid()      : configuring pid regulator mode
 *	mc_speed()    : configuring speed mode
 *	mc_stream()   : configuring streaming mode
 *	mc_stop()     : stop and set output to off
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "tics.h"
#include "adc.h"
#include "mc.h"

#define NONE		0	// for new_state only
#define INACTIVE	1	// mc is inactive, i.e., pwm is off
#define	SPEED		2	// speed control like regular PWM
#define STREAM		3	// asking for on/off every single tic

void mc_state( MCP mcp )
     {
	int out;
        // state pre-processing
        if ( mcp->new_state == INACTIVE )
	   hwp_write( mcp->port_id, 0 );
        if ( mcp->new_state == SPEED )
	   tics_take( & mcp->tics );
        mcp->new_state && (mcp->state = mcp->new_state);
        mcp->new_state = NONE;
        // state processing
	if ( mcp->state == SPEED || mcp->state == STREAM )
	{
	   if ( mcp->state == SPEED )
	        out = tics_next( & mcp->tics );
	   else out = mcp->stream( mcp );
	   if ( out )
	      adc_wait( mcp->adc );
	   hwp_write( mcp->port_id, out );
	}
     }

int mc_config( MCP mcp, int port_id, ADC *adc )
    {
       mcp->port_id = port_id;
       mcp->adc     = adc;
       mc_stop(  mcp );
       mc_state( mcp );		// initializing state, new_state etc.
       return swp_register( mc_state, mcp );
    }

void mc_speed( MCP mcp, int bresenham, int on_tics, int off_tics )
     {
	tics_newtics( & mcp->tics, bresenham, on_tics, off_tics );
        mcp->new_state  = SPEED;
     }

void mc_stream( MCP mcp, MC_STREAM stream )
     {
	mcp->stream    = stream;
	mcp->new_state = STREAM;
     }

void mc_stop( MCP mcp )
     {
	mcp->new_state = INACTIVE;
     }

