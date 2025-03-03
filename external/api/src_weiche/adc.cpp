
/*
 *	name:   adc.c
 *	prefix: adc_
 *
 *	this modules implements the interface to a physical A/D-controller.
 *	it samples the input port regularly (autoread == 1) or on demand
 *	(adc_read()). if desired, adc_average() returns the average of
 *	the previous calls and resets the involved variables.
 *
 *	'API':
 *	------
 *	adc_state()  : the interrupt handler
 *	adc_config() : configuring the port
 *	adc_read()   : taking a sample from the physical interface
 *	adc_average(): calculating the average and resetting variables.
 *	adc_set()    : setting autoread and repeat
 *	adc_wait()   : initiate wait cycles
 *
 */

#include "hw-ports.h"
#include "sw-ports.h"
#include "adc.h"

#define NONE		0	// for new_state only
#define READ		1	// reading out the current input value

//	handling the A/D-controller

static int read_adc( ADC *adc )
       {
	  int i = adc->repeat;
	  adc->cnt += i;
	  adc->cycles++;
	  while( i-- > 0 )
	     adc->value += hwp_read( adc->port_id );
	  return adc->cycles;
       }

void adc_state( ADC *adc )
     {
	if ( adc->new_state == READ )
	   adc_read( adc );
	else if ( adc->autoread )
	        if ( adc->wait_cnt > 0 )
		   adc->wait_cnt--;
		else adc_read( adc );
	adc->new_state = NONE;
     }

int adc_config( ADC *adc, int port_id )
    {
       adc->port_id = port_id;
       adc->average = adc->cnt = adc->value = adc->autoread = 0;
       adc->repeat  = 1;
       adc->wait    = 0;
       return swp_register( adc_state, adc );
    }

void adc_read( ADC *adc )
     {
        adc->new_state = READ;	// this way, it is interrupt safe
     }

int adc_average( ADC *adc )	// ought to be called during interrupts only
    {
       adc->average = adc->value / adc->cnt;
       adc->cnt     = adc->value = adc->cycles = 0;
       return adc->average;
    }

void adc_set( ADC *adc, int autoread, int wait, int repeat )
     {
        adc->autoread = autoread;
        adc->repeat   = repeat;
        adc->wait     = wait;
     }

void adc_wait( ADC *adc )
     {
        adc->wait_cnt = adc->wait;
     }

