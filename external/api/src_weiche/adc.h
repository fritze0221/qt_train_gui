
/*
 *	name:   adc.h
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

#ifndef INCL_GUARD_ADC
#define INCL_GUARD_ADC

typedef struct {
	  char state;		// current port state
	  char new_state;	// external requests for state change
	  int  port_id;		// the actual ADC interface
	  int  wait;		// number of wait cycles before adc_read()
          int  repeat;		// number of consecutive read operations
	  int  wait_cnt;	// current wait counter
	  int  cnt;		// the number of read() calls
	  int  value;		// the cumulated read() values
	  int  average;		// average = value/cnt
	  int  cycles;		// avoiding '*' and '/': cycles * repeat = cnt
	  int  autoread;	// automatically read at interrupt calls
	} ADC;

void adc_state  ( ADC *adc );
int  adc_config ( ADC *adc, int port_id );
void adc_read   ( ADC *adc );
int  adc_average( ADC *adc );
void adc_set    ( ADC *adc, int autoread, int wait, int repeat );
void adc_wait   ( ADC *adc );

#endif

