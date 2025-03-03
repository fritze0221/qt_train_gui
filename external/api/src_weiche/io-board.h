
/*
 *	name:   io-board.h
 *	prefix: iob_
 *
 *	this modules implements the basic data structure for the
 *	general purpose board. it basically consists of an array
 *	of unions, which collect all possible input and output
 *	data.
 *
 *	the parameter io_mode in iob_portconfig() refers to the
 *	modes defines in hw-ports.h
 *
 *	'API':
 *	------
 *	iob_init()       : initializes the data structure, and calls hw_config()
 *	iob_portconfig() : requesting a data entry linked to port number
 *	iob_getentry()   : returns pointer to data structure
 *	iob_portrelease(): marks a port as unused
 *	iob_size()       : returns the number of available gpio ports
 *
 */

#define iob_portrelease( nr )	iob_pconfig( nr, HWP_UNUSED )

void  iob_init      ();
void *iob_portconfig( int nr, int io_mode );
void *iob_getentry  ( int nr );
int   iob_size      ();


