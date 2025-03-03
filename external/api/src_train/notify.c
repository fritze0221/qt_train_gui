
/*
 *	name:   notify.c
 *	prefix: nfy_
 *
 *	this is like the unix alram system call. but since this name
 *	is already in use, we call this module notify. in contrast
 *	to the usr module, notify is a one time notifier, not a
 *	periodic one. this module allows for up to 10 (NR_OF_NOTIFIERS)
 *	of open notifications (alarms). 
 *
 *	the call-back function has two parameters, a pointer to a
 *	private data structure and a pre-specied number, which might
 *	allow for further 'alarm' discriminations. every notification
 *	is assigned an internal id, which may be used for canceling
 *	an open notifier.
 *
 *	if the notifier returns a value not equal to zero, the
 *	notifier is re-registered with this number interpreted as
 *	the new tics value.
 *
 *	'API':
 *	------
 *	nfy_state() : the interrupt handler
 *	nfy_init()  : initializing and registering this module
 *	nfy_set()   : setting a notification (an alarm)
 *	nfy_cancel(): canceling a notification
 *
 */

#include "sw-ports.h"
#include "notify.h"

typedef struct {
	   int   tics;			// waiting time
	   int   number;		// a user-specified number
	   void *data;			// pointer to private data
	   NFY_HANDLER notifier;	// the call-back function.
	} NFY;

static NFY nfy_tab[ NR_OF_NOTIFIERS ];	// index 0 is unused !

void nfy_state( void *data )
     {
        NFY *p;
        for( p = nfy_tab + 1; p < nfy_tab + NR_OF_NOTIFIERS; p++ )
	   if ( p->notifier && p->tics-- <= 1 )
	      if ((p->tics = p->notifier( p->data, p->number )) == 0)
		 p->notifier = 0;
     }

int nfy_init()
    {
       int i;
       for( i = 0; i < NR_OF_NOTIFIERS; i++ )
	  nfy_tab[ i ].notifier = 0;
       return swp_register( nfy_state, 0 );
    }

int nfy__set( NFY_HANDLER notifier, int tics, void *data, int number )
    {
       NFY *p = nfy_tab + NR_OF_NOTIFIERS - 1;	// set to last entry
       while( p->notifier )
          p--;
       if ( p > nfy_tab )
       {
	  p->tics     = tics;
	  p->number   = number;
	  p->data     = data;
	  p->notifier = notifier;
       }
       return p - nfy_tab;
    }

void nfy_cancel( int id )
     {
        (id >= 0 && id < NR_OF_NOTIFIERS) && (nfy_tab[ id ].notifier = 0);
     }






