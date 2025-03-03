
/*
 *	name:   dir-input.h
 *	prefix: dir_
 *
 *	this module simulated a standard track contact, which
 *	indicates not only whether a track is occupied but also
 *	in which direction the train rides.
 *
 *	'API':
 *      ------
 *	dir_state()  : the interrupt handler
 *	dir_config() : configuring the port
 *	dir_value()  : port's current input value, but not the partner's one
 *	dir_changed(): has the direction changed?
 *
 */

typedef struct {
	  int  port_id;		// the first input id
	  int  value;		// actually, not used
	  int  direction;	// current direction, i.e., DIR_OFF....
	  int  changed;		// any change
	  DIN *partner;		// the second input id
	} LRIN;

#define DIR_OFF		0
#define DIR_LEFT	1
#define DIR_RIGHT	2

void dir_state ( LRIN *dir );
int  dir_config( LRIN *dir, DIN *partner );

#define dir_value(   dir )	((dir)->direction)
#define dir_changed( dir )	((dir)->changed)
