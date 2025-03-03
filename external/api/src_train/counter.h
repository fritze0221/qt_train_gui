
/*
 *	name:   count.h
 *	prefix: cnt_
 *
 *	this module implements a counter input port. this can be used,
 *	for instance, to count the number of axels a train has.
 *
 *	'API':
 *	------
 *	cnt_state()      : the interrupt handler
 *	cnt_config()     : configuring the port
 *	cnt_setbdelay()  : setting the bunce delay
 *	cnt_setpositive(): set the trigger (count) edge
 *	cnt_count()      : reading the count value
 *	cnt_reset()      : resetting the counter
 *
 */

typedef struct {
	  int port_id;		// the actual port interface
	  int value;		// the last value read
	  int positive;		// count on positive edge
	  int count;		// counter value
	  int bdelay;		// delay to skip input bunces
	  int bcount;		// bunce delay counter
	} CNT;

void cnt_state ( CNT *cnt );
int  cnt_config( CNT *cnt );

#define cnt_setbdelay(   cnt, val )	((cnt)->bdelay   = val)
#define cnt_setpositive( cnt, val )	((cnt)->positive = val)
#define cnt_count( cnt )		((cnt)->count)
#define cnt_reset( cnt )		((cnt)->count = 0)
