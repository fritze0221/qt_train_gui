
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

#include "swp-conf.h"
#include "sw-ports.h"
#include <Arduino.h>

typedef struct {
	  SWP_HANDLER port_handler;	// function that is called at every tic
	  void       *port_data;	// pointer to the private handler data
        } SW_PORT;

static SW_PORT swp_tab[ SWP_SIZE ];
static int     swp_size = 0;

static int swp_find( void *port_data )
       {
          int i = 0;
          while( i < swp_size && swp_tab[ i ].port_data != port_data )
             i++;
          return i;
       }

int print_tab( 	)
{
	
	int i;
	for( i = 0; i < swp_size; i++){

		Serial.printf("%d %p	\n", i, swp_tab[ i ].port_handler);

	}
	return 0;
}
int swp__register( SWP_HANDLER port_handler, void *port_data )
    {
       int i;
       if ((i = swp_find( port_data)) == swp_size )	// not yet registered
          if ( swp_size == SWP_SIZE )
	     return 0;				// no further port available
	  else swp_size++; 			// allocate new port
       swp_tab[ i ].port_handler = port_handler;
       swp_tab[ i ].port_data    = port_data;
       return 1;
    }

void swp_handlers()
     {
	SW_PORT *p, *end;
	int k=0;
        for( p = swp_tab, end = p + swp_size; p < end; p++ ){
		int t1,t2;
#ifdef ESP32
	t1 = micros();
#endif
	   p->port_handler( p->port_data );
#ifdef ESP32
	t2 = micros();
//	Serial.printf("swport: %d Zeit in us: %d\n",k,t2-t1);
	k++;
#endif

	}
     }

int swp_move( void *port_data, int new_index )
    {
       SW_PORT h;
       int     i = swp_find( port_data );
       if ( i == swp_size || new_index < 0 || new_index >= swp_size )
	  return 0;
       h = swp_tab[ i ];
       if ( new_index  < i )
          for( ; i > new_index; i-- )
	     swp_tab[ i ] = swp_tab[ i - 1 ];
       else for( ; i < new_index; i++ )
	       swp_tab[ i ] = swp_tab[ i + 1 ];
       swp_tab[ i ] = h;
       return 1;
    }

int swp_remove( void *port_data )
    {
       int i;
       if ((i = swp_find( port_data )) == swp_size )
	  return 0;
       for( swp_size--; i < swp_size; i++ )
	  swp_tab[ i ] = swp_tab[ i + 1 ];
       return 1;
    }

