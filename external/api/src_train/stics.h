
/*
 *	name:   tics.h
 *	prefix: stics_
 *
 *	this modules provides tics and their functionality
 *
 *	'API':
 *	------
 *	stics_newtics()	set new stics value
 *	stics_take()	take the new value as current value
 *	stics_isnew()	testing for new stics value
 *	stics_start()	start the stics counter
 *	stics_vstart()	start the stics counter with a given start value
 *	stics_stop()	stop stics counter
 *	stics_step()	decrease counter by 1
 *	stics_expired()	has the stics counter expired?
 *	stics_counter()	get the stics counter value
 *
 */

typedef struct {
	  char valid;		// new_stics[] are valid
	  int  tics;		// on tics
	  int  new_tics;	// new stics value
	  int  cnt;		// stics counter
	} STICS, *STIP;

int stics_newtics( STIP tip, int stics );
int stics_take   ( STIP tip );

#define stics_isnew(   tip )		((tip)->valid)
#define stics_start(   tip )		((tip)->cnt = (tip)->tics)
#define stics_vstart(  tip, val )	((tip)->cnt = (val))
#define stics_stop(    tip )		((tip)->cnt = 0)
#define stics_step(    tip )		((tip)->cnt--)
#define stics_expired( tip )		((tip)->cnt <= 0)
#define stics_counter( tip )		((tip)->cnt)


