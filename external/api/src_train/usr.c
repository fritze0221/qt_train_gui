
/*
 *	name:   usr.c
 *	prefix: usr_
 *
 *	this modules implements the very simple on/off port
 *
 *	'API':
 *	------
 *	usr_state() : the interrupt handler
 *	usr_config(): configuring the port, i.e., address etc.
 *      usr_set()   : setting the wait tics
 *	usr_on()    : activating the user port
 *	usr_off()   : deactivating the user port
 *
 */

#include "sw-ports.h"
#include "stics.h"
#include "usr.h"

#define NONE		0		// for new_state only
#define INACTIVE	1		// operation suspended
#define ACTIVE		2		// operation in progress

void usr_state( USR *usr )
     {
        stics_take( & usr->tics );
        (usr->new_state) && (usr->state = usr->new_state);
        usr->new_state = NONE;
        if ( usr->state == ACTIVE && stics_expired( & usr->tics ) )
        {
	   usr->usr_fnc( usr->usr_data );
	   stics_start( & usr->tics );
        }
        (usr->state == ACTIVE) && (stics_step( & usr->tics ));
     }

int usr__config( USR *usr, USR_HANDLER usr_fnc, void *usr_data )
    {
	usr->usr_fnc  = usr_fnc;
	usr->usr_data = usr_data;
	usr_set  ( usr, -1 );
	usr_off  ( usr );
	usr_state( usr );	// taking the tics count etc.
	return swp_register( usr_state, usr );
    }

void usr_set( USR *usr, int tics_wait )
     {
	stics_newtics( & usr->tics, tics_wait );
     }

void usr_on ( USR *usr ){ usr->new_state = ACTIVE;   }
void usr_off( USR *usr ){ usr->new_state = INACTIVE; }

