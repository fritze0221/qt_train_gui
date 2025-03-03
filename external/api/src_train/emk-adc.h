
/*
 *	name:   emk-adc.h
 *	prefix: emk_
 *
 *	this module is a variation of the regular A/D-controller.
 *	it is intendet to care or the pecularities of an electrical
 *	motor for which we want to measure (estimate) its current 
 *	speed. to this end, it constantly monitors the motor output.
 *	in case the output is zero (off, the motor is idle), it
 *	samples its input, after the specified wait tics have 
 *	expired. these wait tics are due to the additional capacitor
 *	the requires a settling time in order to reach a dynamic
 *	equillibrium. thus, this module should be usedin the context
 *	of a motor control.
 *
 *	'API':
 *	------
 *	emk_state  (): the interrupt handler
 *	emk_config (): configuring the port
 *	emk_set    (): setting wait_tics and repeat
 *	emk_reset  (): reset value and cnt
 *	emk_average(): calculate the average and reset the values
 *	emk_emks   (): how often were samples taken
 *
 */

typedef struct {
	  int port_id;		// the actual ADC interface
	  int motor_id;		// the motor control port
	  int wait_tics;	// the number of motor idle tics 
	  int wait_cnt;
          int repeat;		// number of consecutive read operations
	  int cnt;		// the number of read() calls, i.e., samples
	  int emks;		// cnt = emks * repeat
	  int value;		// the cumulated read() values
	  int average;		// average = value/cnt
	} EMK;

void emk_state  ( EMK *emk );
int  emk_config ( EMK *emk, int motor_id );
void emk_set    ( EMK *emk, int wait_tics, int repeat );
void emk_reset  ( EMK *emk );
int  emk_average( EMK *emk );

#define emk_emks( emk )	((emk)->emks)

