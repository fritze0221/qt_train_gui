
/*
 *	name:   usr.h
 *	prefix: usr_
 *
 *	this modules implements the very simple on/off port
 *
 *	'API':
 *	------
 *	usr_state() : the interrupt handler
 *	usr_config(): configuring the port, i.e., address etc.
 *	usr_set()   : setting the wait tics
 *	usr_on()    : activating the user port
 *	usr_off()   : deactivating the user port
 *
 */

typedef void (* USR_HANDLER)( void * );// pointer to a usr-specific function

typedef struct {
	  char  state;		// current port state
	  char  new_state;	// handling external requests
	  void *usr_data;	// pointer to the user's private data
	  STICS tics;		// the tics counter
	  USR_HANDLER usr_fnc;	// pointer to the user call-back function
	} USR;

#define usr_config( usr, usr_fnc, usr_data )	\
	usr__config( usr, (USR_HANDLER) usr_fnc, (void *) usr_data )

void usr_state  ( USR *usr );
int  usr__config( USR *usr, USR_HANDLER usr_fnc, void *usr_data );
void usr_set    ( USR *usr, int tics_wait );
void usr_on     ( USR *usr );
void usr_off    ( USR *usr );


