

/*
 *	name:   notify.h
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

#define NR_OF_NOTIFIERS		10

typedef int (* NFY_HANDLER)( void *, int ); // pointer to a call-back function

#define nfy_set( notifier, tics, data, number )	\
	nfy__set( (NFY_HANDLER) notifier, tics, (void *) data, number )

void nfy_state( void *data );
int  nfy_init();
int  nfy__set( NFY_HANDLER notifier, int tics, void *data, int number );
void nfy_cancel( int id );


