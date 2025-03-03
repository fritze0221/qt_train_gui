
/*
 *	name:   mf.c
 *	prefix: mf_
 *
 *	this module implements the classical monoflop functionality.
 *
 *	'API':
 *	------
 *	mf_state()  : the interrupt handler
 *	mf_config() : configuring the port, i.e., address etc.
 *	mf_set()    : setting the number of on-tics
 *	mf_trigger(): starting the monoflop
 *      mf_ison()   : monoflop still active, i.e., output port is on?
 *	mf_cancel() : stopping it
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "stics.h"
#include "mf.h"

#define NONE		0		// for new_state only
#define INACTIVE	1		// monoflop is inactive
#define ACTIVE		2		// monoflop is active

void mf_state( MFP mfp )
     {
        stics_take( & mfp->stics );
        if ( mfp->new_state == INACTIVE )
	   hwp_off( mfp->port_id );
        if ( mfp->new_state == ACTIVE )
	   stics_start( & mfp->stics );
        (mfp->new_state) && (mfp->state = mfp->new_state);
        mfp->new_state = NONE;
        if ( mfp->state == ACTIVE )
        {
	   if ( stics_expired( & mfp->stics ) )
	      mfp->state = INACTIVE;
	   else stics_step( & mfp->stics );
	   hwp_write( mfp->port_id, mfp->state == ACTIVE );
        }
     }

int mf_config( MFP mfp, int port_id )
    {
	mfp->port_id = port_id;
	mf_set( mfp, 0 );
	mf_cancel( mfp );
	mf_state( mfp );
	return swp_register( mf_state, mfp );
    }

void mf_set( MFP mfp, int on_tics )
     {
	stics_newtics( & mfp->stics, on_tics );
     }

void mf_trigger( MFP mfp ){ mfp->new_state = ACTIVE ; }
int  mf_ison   ( MFP mfp ){ return mfp->state == ACTIVE; }
void mf_cancel ( MFP mfp ){ mfp->new_state = INACTIVE; }


