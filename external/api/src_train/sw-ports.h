
/*
 *	sw-ports.h
 *	prefix: swp_
 *
 *	this file maintains an array of software ports. every
 *	entry consists of a pointer to the 'private' data 
 *	of a software port as well as a pointer to the propper
 *	state handler, which is called at every time tic (interrupt).
 *
 *	'API':
 *	------
 *	swp_register(): registering a software port
 *	swp_handlers(): calling all state handlers within a loop
 *	swp_move()    : move an entry to the specified index
 *      swp_remove()  : remove the specified entry
 *
 */

typedef void (* SWP_HANDLER)( void * );	// pointer to void-function

#define swp_register( port_handler, port_data ) \
	   swp__register( (SWP_HANDLER) port_handler, (void *) port_data )

int print_tab();
int  swp__register( SWP_HANDLER port_handler, void *port_data );
void swp_handlers ();
int  swp_move     ( void *port_data, int new_index );
int  swp_remove   ( void *port_data );


