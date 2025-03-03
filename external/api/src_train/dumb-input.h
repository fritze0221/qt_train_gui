
/*
 *	name:   dumb-input.h
 *	prefix: din_
 *
 *      this module implements the very simple input port. once it is
 *      configured, it reads the input value from the physical port and
 *      updates both, value and value_changes
 *
 *	'API':
 *	------
 *	din_state()  : the interrupt handler
 *	din_config() : configure the port
 *	din_value()  : current input value
 *	din_changed(): has the current input changed?
 *
 */

typedef struct {
	  int port_id;
	  int value;		// the last value read
	  int value_changed;	// any change?
	} DIN;

void din_state ( DIN *inp );
int  din_config( DIN *inp );

#define din_value(   inp )	((inp)->value)
#define din_changed( inp )	((inp)->value_changed)
