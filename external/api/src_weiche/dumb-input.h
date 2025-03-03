
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
 *	din_config(): configure the port
 *
 */

typedef struct {
	  int port_id;		// the actual port interface
	  int value;		// the last value read
	  int value_changed;	// any change?
	} DIN;

int din_config( DIN *inp, int port_id );

