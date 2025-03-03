
/*
 *	name:   magnet.h
 *	prefix: mag_
 *
 *	this module implements the handling of electric magnets,
 *	such as switches, that also provide feedback about their
 *	physical position.
 *
 *	'API':
 *	------
 *	mag_state()  : the interrupt handler
 *	mag_config() : configuring the port
 *	mag_trigger(): trigger the output port
 *	mag_tics()   : reset the number of on-tics
 *	mag_busy()   : port is active, i.e., output is set to on
 *	mag_alert()  : activation time has expired during operation
 *	mag_clear()  : reset the alert state
 *
 */

typedef struct {
	  int   port_id;		// nr of the output port
	  int   state;
	  int   new_state;
	  DIN  *feedback;		// pointer to the input feedback port
	  STICS stics;			// tics for the monoflop
	} MAG;

void mag_state  ( MAG *mag );
int  mag_config ( MAG *mag, DIN *in_port, int on_tics );
void mag_trigger( MAG *mag );
void mag_tics   ( MAG *mag, int on_tics );
int  mag_busy   ( MAG *mag );
int  mag_alert  ( MAG *mag );
void mag_clear  ( MAG *mag );

