
/*
 *	name:   smart-input.h
 *	prefix: sin_
 *
 *	this module implements a 'smart' input port, which features,
 *      in addition to reading values, triggers and other functions
 *
 *	'API':
 *	------
 *	sin_config (): configuring the smart-input port
 *	sin_memory (): setting an 'external' memory location for value
 *	sin_trigger(): setting a triger condition and trigger function
 *
 */

typedef void (* SIN_FNC)( void * );

typedef struct _smart_input {
	  int     port_id;	// the actual port interface
	  int     value;	// the last value read
	  int    *memory;	// location for automatically storing value
	  int     trigger;	// trigger criterion
	  SIN_FNC fnc;		// call back trigger function 
	  void   *data;		// private data passed to fnc
	} SIN;

#define SIN_TNEVER	0	// do not call fnc()
#define SIN_THIGH	1	// call on high, i.e., value == 1
#define SIN_TLOW	2	// call on low, i.e., value == 0
#define SIN_TRISING	3	// call on rising edge, i.e., value 0 --> 1
#define SIN_TFALLING	4	// call on falling edge, i.e., value 1 --> 0
#define SIN_TCHANGE	5	// call, if value has changed

#define	sin_trigger( inp, trigger, fnc, data ) \
	sin__trigger( inp, trigger, (SIN_FNC) fnc, (void *) data )

int  sin_config  ( SIN *inp, int port_id );
void sin_memory  ( SIN *inp, int *memory );
void sin__trigger( SIN *inp, int trigger, SIN_FNC fnc, void *data );

