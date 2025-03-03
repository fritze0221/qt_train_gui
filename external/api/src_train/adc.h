
/*
 *	name:   adc.h
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


typedef struct {
	  int  port_id;		// the actual ADC interface
	  int  state;		// current port state
	  int  new_state;	// external requests for state change
          int  repeat;		// number of consecutive read operations
	  int  autoread;	// automatically read at interrupt calls
	  int  cnt;		// the number of read() calls
	  int  value;		// the cumulated read() values
	  int  average;		// average = value/cnt
	  int *copy_out;	// calculate an copy out the average
	} ADC;

void adc_state  ( ADC *adc );
int  adc_config ( ADC *adc );
void adc_read   ( ADC *adc );
void adc_reset  ( ADC *adc );
int  adc_average( ADC *adc );	// ought to be called *only* during interrupts
void adc_set    ( ADC *adc, int autoread, int repeat );

#define adc_readout(  adc, addr )	((adc)->copy_out =  addr)
#define adc_readdone( adc )		((adc)->copy_out == 0   )


