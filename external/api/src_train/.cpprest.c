
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
#include "mc.h"

#define NONE		0	// for new_state only
#define INACTIVE	1	// mc is inactive, i.e., pwm is off
#define	PWM_SPEED	2	// speed control like regular PWM
#define	PBM_SPEED	3	// speed control like bresemham PWM (PBM)
#define STREAM		4	// asking for on/off every single tic

//	handling the A/D-controller

static int adc_read( MC_ADC *adc )
       {
	  int i = adc->repeat;
	  adc->cnt += i;
	  while( i-- > 0 )
	     adc->value += hwp_adcread( adc->port_id );
	  adc->tics = adc->wait;
	  return 1;
       }

#define adc_average( adc ) 	((adc)->value / (adc)->cnt)
#define adc_reset(   adc )	((adc)->cnt = (adc)->value = (adc)->tics = 0)

void mc_state( MCP mcp )
     {
        int out = mcp->output;
        // state pre-processing
        if ( mcp->new_state == INACTIVE )
	   hwp->write( mcp->port_id, (mcp->output = 0));
        if ( mcp->new_state == PWM_SPEED )
	   tics_take( & mcp->tics );
        if ( mcp->new_state == PBM_SPEED )
	   btics_take( & mcp->btics );
        mcp->new_state && (mcp->state = mcp->new_state);
        mcp->new_state = NONE;
        // state processing
        if ( mcp->output == 0 )	// output port *was* off
	   adc_read( & mcp->adc );
        if ( mcp->state == PWM_SPEED )
        {
	   if ( tics_expired( & mcp->tics ) )
	      tics_start( & mcp->tics );
	   mcp->output = tics_next( & mcp->tics );
        }
        if ( mcp->state == PBM_SPEED )
	   mcp->output = btics_next( & mcp->btics );
        if ( mcp->state == STREAM )
	   mcp->output = mcp->stream( mcp );
        hwp->write( mcp->port_id, mcp->output );
     }

//	configuration

int mc_config( MCP mcp, int port_id, ADC *adc )
    {
       mcp->port_id     = port_id;
       mcp->adc.port_id = adc_portid;
       mc_stop(  mcp );
       mc_state( mcp );		// initializing state, new_state etc.
       return swp_register( mc_state, mcp );
    }

void mc_adcconfig( MCP mcp, int settle, int wait, int repeat )
     {
        MC_ADC *adc = & mcp->adc;
        adc->settle  = settle;
        adc->wait    = wait;
        adc->repeat  = repeat;
     }

//	mode selection and parametrization

void mc_pid( MCP mcp, MC_PID pid )
     {
        mcp->new_state  = INACTIVE;
	mcp->pid        = pid;
	mcp->new_state  = PID;
     }

void mc_speed( MCP mcp, int on_tics, int off_tics, int sync )
     {
        mcp->new_state  = INACTIVE;
	mcp->speed_sync = sync;
	tics_newtics( & mcp->tics, on_tics, off_tics );
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

