
/*
 *	name:   adc.c
 *	prefix: adc_
 *
 *	this module implements the interface to a physical A/D-controller.
 *	here, we keep it very simple. the moule's intention ist to monitor
 *	an analog input port. the module takes samples regularly
 *	(autoread == 1) or on demand (adc_read()). on request, it returns
 *	the average of the previous calls and resets the involved variables.
 *
 *	'API':
 *	------
 *	adc_state()   : the interrupt handler
 *	adc_config()  : configuring the port
 *	adc_read()    : taking a sample from the physical interface
 *	adc_reset()   : reset all the 'read' values to 0
 *	adc_average() : calculating the average and resetting variables.
 *	adc_set()     : setting autoread and repeat
 *	adc_readout() : request a read out to memory location and reset
 *	adc_readdone(): is adc_readout() done or still in progress
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "adc.h"

#define NONE		0	// for new_state only
#define READ		1	// reading out the current input value

void adc_state( ADC *adc )
     {
	int i;
	if ( adc->new_state == READ || adc->autoread )
	   for( i = adc->repeat, adc->cnt += i; i > 0; i-- )
	      adc->value += hwp_read( adc->port_id );
	if ( adc->copy_out )
	{
	   *(adc->copy_out) = adc_average( adc );
	   adc->copy_out = 0;
	}
	adc->new_state = NONE;
     }

int adc_config( ADC *adc )
    {
       adc->average = adc->cnt = adc->value = adc->autoread = 0;
       adc->copy_out = 0;
       adc->repeat  = 1;
       return swp_register( adc_state, adc );
    }

void adc_read( ADC *adc )
     {
        adc->new_state = READ;	// this way, it is interrupt safe
     }

void adc_reset( ADC *adc )	// ought to be called *only* during interrupts
     {
        adc->cnt = adc->value = 0;
     }

int adc_average( ADC *adc )	// ought to be called *only* during interrupts
    {
       adc->average = (adc->cnt == 0)? 0: adc->value / adc->cnt;
       adc_reset( adc );
       return adc->average;
    }

void adc_set( ADC *adc, int autoread, int repeat )
     {
        adc->autoread = autoread;
        adc->repeat   = repeat;
     }

