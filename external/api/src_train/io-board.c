
/*
 *	name:   io-board.c
 *	prefix: iob_
 *
 *	this modules implements the basic data structure for the
 *	general purpose board. it basically consists of an array
 *	of unions, which collect all possible input and output
 *	data.
 *
 *	the parameter io_mode in iob_portconfig() refers to the
 *	modes defines in hw-ports.h
 *
 *	NOTE: it is important to understand that this module also
 *	initializes *all* port_id's. to this end, the port_id
 *	*must* be the first komponent of all ports!
 *
 *	'API':
 *	------
 *	iob_init()       : initializes the data structure, and calls hw_config()
 *	iob_portconfig() : requesting a data entry linked to port number
 *	iob_getentry()   : returns pointer to data structure
 *	iob_portrelease(): marks a port as unused
 *	iob_size()       : returns the number of available gpio ports
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "io-board.h"

#include "tics.h"
#include "stics.h"

#include "adc.h"
#include "counter.h"
#include "dumb-input.h"
#include "dir-input.h"
#include "dpwm.h"
#include "emk-adc.h"
#include "magnet.h"
#include "mf.h"
#include "pwm.h"
#include "smart-input.h"
#include "sp.h"
#include "xpm.h"
#include "dr.h"

typedef union {
	  ADC  adc;
	  CNT  cnt;
	  LRIN lrin;
	  DPWM dpwm;
	  DIN  din;
	  EMK  emk;
	  MAG  magnet;
	  MF   mf;
	  PWM  pwm;
	  SIN  sin;
	  SP   sp;
	  XPM  xpm;
	  DR dr;
	  int  port_id;
	} GPIO_DATA, *GDP;

typedef struct {
	  GPIO_DATA   data;	// the actual port data, i.e., MF, PWM, ...
	  // int         type;	// the current port type, i.e., MF, PWM, ...
	} GPIO_PORT, *GPP;

static GPIO_PORT iop_tab[ HWP_SIZE ];	// one port for every HW-connector

void iob_dummy( void *data ){}	// dummy function

void iob_init()
    {
       int i;
       hwp_init();			// initialize the hardware interface
       for( i = 0; i < HWP_SIZE; i++ )
       {
          // iop_tab[ i ].type = GP_UNUSED;
	  iop_tab[ i ].data.port_id = i;
	  swp_register( iob_dummy, iop_tab + i );
       }
    }

void *iob_portconfig( int nr, int io_mode )
     {
        if ( nr < 0 || nr >= HWP_SIZE )
           return 0;
	if ( io_mode == HWP_UNUSED )
	   swp_register( iob_dummy, iop_tab + nr );
        hwp_config( nr, io_mode );
        // iop_tab[ nr ].type = io_mode; ???
        return (void *) (iop_tab + nr);
     }

void *iob_getentry( int nr )
     {
        return (nr < 0 || nr >= HWP_SIZE)? 0: (void *) (iop_tab + nr);
     }

int iob_size(){ return HWP_SIZE; }




