
/*
 *	name:   xpm.h
 *	prefix: xpm_
 *
 *	this modules implements a PWM (puls-width modulation) whos
 *	on/off tics are controlled by a user-specified function. 
 *	this module can be thought of as a pwm-stream: at the
 *	end of every on/off-cycle, the xpm_state() function calls
 *	the user-specified function, mentioned above, to copy-in
 *	a pair of new on/off values.
 *
 *	'API':
 *	------
 *	xpm_state()  : the interrupt handler
 *	xpm__config(): configuring an xp port
 *	xpm_start()  : start the port operation
 *	xpm_stop()   : stop the port operation
 *
 */

typedef int (* XPM_NXT)( void *, int *, int * );  // ptr to call-back function

typedef struct {
	  int   port_id;	// the actual port interface
	  int   state;		// current port state
	  int   new_state;	// new port state by external requests
	  void *data;		// pointer to some private data
	  TICS    tics;		// the tics
	  XPM_NXT get_tics;	// getting a set of new on/off tics
	} XPM, *XPMP;


#define xpm_config( xpm, port_id, get_tics, data )	\
	xpm__config( xpm, port_id, (XPM_NXT) get_tics, (void *) data )

void xpm_state  ( XPM *xpm );
int  xpm__config( XPM *xpm, XPM_NXT get_tics, void *data );
void xpm_start  ( XPM *xpm );
void xpm_stop   ( XPM *xpm );



