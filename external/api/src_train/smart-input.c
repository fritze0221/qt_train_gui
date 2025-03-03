
/*
 *	name:   smart-input.c
 *	prefix: sin_
 *
 *	this module implements a 'smart' input port, which features,
 *      in addition to reading values, triggers and other functions
 *
 *	'API':
 *	------
 *	sin_state  (): the interrupt handler
 *	sin_config (): configuring the smart-input port
 *	sin_memory (): setting an 'external' memory location for value
 *	sin_trigger(): setting a triger condition and trigger function
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "smart-input.h"

void sin_state( SIN *inp )
     {
        int value, old = inp->value, trigger = inp->trigger;
        value = inp->value = hwp_read( inp->port_id );
        inp->memory && (*(inp->memory) = value);
        if ( inp->fnc && trigger
	     && (   (trigger == SIN_THIGH && value > 0)
		 || (trigger == SIN_TLOW && value <= 0)
		 || (trigger == SIN_TRISING && value > 0 && old <= 0)
		 || (trigger == SIN_TFALLING && value <= 0 && old > 0)
		 || (trigger == SIN_TCHANGE && value != old)))
	   inp->fnc( inp->data );
     }

int sin_config( SIN *inp )
    {
	inp->value   = hwp_read( inp->port_id );
	inp->memory  = 0;
	inp->trigger = SIN_TNEVER;
	inp->fnc     = 0;
	return swp_register( sin_state, inp );
    }

void sin_memory( SIN *inp, int *memory )
     {
        inp->memory = memory;
     }

void sin__trigger( SIN *inp, int trigger, SIN_FNC fnc, void *data )
     {
        inp->trigger = trigger;
        inp->data    = data;
	inp->fnc     = fnc;
     }


