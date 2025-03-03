
/*
 *	sw-ports.c
 *	prefix: swp_
 *
 *	this file maintains an array of software ports. every
 *	entry consists of a pointer to the 'private' data 
 *	of a software port as well as a pointer to the propper
 *	state handler, which is called at every time tic (interrupt).
 *
 *	'API':
 *	------
 *	swp__register(): registering a software port
 *	swp_handlers() : calling all state handlers within a loop
 *      swp_move()     : move an entry to the specified index
 *	swp_remove()   : remove the specified entry
 *
 */

#include "sw-ports-cf.h"
#include "sw-ports.h"

typedef struct {
	  SWP_HANDLER port_handler;	// function that is called at every tic
	  void       *port_data;	// pointer to the private handler data
        } SW_PORT;

static SW_PORT port_tab[ SWP_SIZE ];
static int     port_size = 0;

static int find( void *port_data )
       {
          int i = 0;
          while( i < port_size && port_tab[ i ].port_data != port_data )
             i++;
          return i;
       }

int swp__register( SWP_HANDLER port_handler, void *port_data )
    {
       int i;
       if ((i = find( port_data)) == port_size )	// not yet registered
          if ( port_size == SWP_SIZE )
	     return 0;				// no further port available
	  else port_size++; 			// allocate new port
       port_tab[ i ].port_handler = port_handler;
       port_tab[ i ].port_data    = port_data;
       return 1;
    }

void swp_handlers()
     {
	SW_PORT *p, *end;
        for( p = port_tab, end = p + port_size; p < end; p++ )
	   p->port_handler( p->port_data );
     }

int swp_move( void *port_data, int new_index )
    {
       SW_PORT h;
       int     i = find( port_data );
       if ( i == port_size || new_index < 0 || new_index >= port_size )
	  return 0;
       h = port_tab[ i ];
       if ( new_index  < i )
          for( ; i > new_index; i-- )
	     port_tab[ i ] = port_tab[ i - 1 ];
       else for( ; i < new_index; i++ )
	       port_tab[ i ] = port_tab[ i + 1 ];
       port_tab[ i ] = h;
       return 1;
    }

int swp_remove( void *port_data )
    {
       int i;
       if ((i = find( port_data )) == port_size )
	  return 0;
       for( port_size--; i < port_size; i++ )
	  port_tab[ i ] = port_tab[ i + 1 ];
       return 1;
    }

