
/*
 *	name:   mf.h
 *	prefix: mf_
 *
 *	this modules implements the classical monoflop functionality.
 *
 *	'API':
 *	------
 *	mf_state()  : the interrupt handler
 *	mf_config() : configuring the port, i.e., address etc.
 *	mf_set()    : setting the number of on-tics
 *	mf_trigger(): starting the monoflop
 *	mf_ison()   : monoflo still active, i.e., output port == 1?
 *	mf_cancel() : stopping it
 *
 */

typedef struct {
	  int   port_id;	// the actual port interface
	  int   state;		// current port state
	  int   new_state;	// new port state by external requests
	  STICS stics;		// tics data structure
	} MF, *MFP;

void mf_state  ( MFP mfp );
int  mf_config ( MFP mfp );
void mf_set    ( MFP mfp, int on_tics );
void mf_trigger( MFP mfp );
int  mf_ison   ( MFP mfp );
void mf_cancel ( MFP mfp );

