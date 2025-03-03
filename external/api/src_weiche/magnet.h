
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
 *	mag_config() : configuring the port
 *	mag_trigger(): trigger the output port
 *	mag_tics()   : reset the number of on-tics
 *      mag_status() : port status, i.e., off, busy, alert
 *	mag_clear()  : reset the alert state
 *
 */

#define MAG_OFF		0
#define MAG_BUSY	1
#define MAG_ALERT	2

typedef struct {
	  int   state;
	  int   new_state;
	  int   port_id;		// nr of the output port
	  DIN  *feedback;		// pointer to the input feedback port
	  STICS stics;			// tics for the monoflop
	} MAG;

int  mag_config ( MAG *mag, int port_id, DIN *in_port, int on_tics );
void mag_trigger( MAG *mag );
void mag_tics   ( MAG *mag, int on_tics );
int  mag_status ( MAG *mag );
void mag_clear  ( MAG *mag );


