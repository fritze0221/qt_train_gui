
/*
 *	name:   tics.h
 *	prefix: tics_
 *
 *      this modules provides tics and their functionality.
 *      in addition to regular tics, it also provides an
 *      algorithm that is inspired by bresenham. that is,
 *      rather than generating 11100, it generates 10101....
 *
 *	'API':
 *	------
 *	tics_newtics()  : set new tics values
 *	tics_take()     : take the new values as current values
 *	tics_isnew()    : testing for new tics values
 *	tics_start()    : start the tics counter
 *	tics_vstart()   : start the tics counter with a given start value
 *	tics_stop()     : stop tics counter
 *	tics_output()   : tics in state, i.e., on or off?
 *	tics_ontics()   : tics on-value
 *	tics_offtics()  : tics off-value
 *	tics_expired()  : has the tics counter expired?
 *	tics_counter()  : get the tics counter value
 *	tics_bresenham(): tics_newtics() and sets mode to bresenhame
 *	tics_regular()  : tics_newtics() and sets mode to regular (PWM)
 *      tics_1ahead()   : looking one next-call ahead
 *
 */

typedef struct {
	  char valid;		// new_on/off_tics are valid
	  char mode;		// bresenham mode of operation
	  int  on_tics;		// on tics
	  int  off_tics;	// off tics
	  int  new_on_tics;	// new on tics
	  int  new_off_tics;	// new off tics
	  int  new_mode;	// new mode
	  int  periode;		// on_tics + off_tics
	  int  accu;		// the required btics-accumulator 
	  int  output;		// the current on/off value
	  int  cnt;		// tics counter
	} TICS, *TIP;

int  tics_newtics( TIP tip, int bresenham, int on_tics, int off_tics );
int  tics_take   ( TIP tip );
int  tics_next   ( TIP tip );

#define tics_isnew(   tip )	((tip)->valid)
#define tics_periode( tip )	((tip)->periode)
#define tics_start(   tip )	((tip)->cnt = tics_periode( tip ))
#define tics_vstart(  tip, cnt ) ((tip)->cnt = (cnt))
#define tics_stop(    tip )	((tip)->cnt = 0)
#define tics_output(  tip )	((tip)->output)
#define tics_ontics(  tip )	((tip)->on_tics)
#define tics_offtics( tip )	((tip)->off_tics)
#define tics_expired( tip )	((tip)->cnt <= 0)
#define tics_counter( tip )	((tip)->cnt)

#define tics_bresenham( tip, on_tics, off_tics )	\
	tics_newtics( tip, 1, on_tics, off_tics )
#define tics_regular( tip, on_tics, off_tics )		\
	tics_newtics( tip, 0, on_tics, off_tics )



