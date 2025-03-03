
/*
 *	name:   usr.h
 *	prefix: usr_
 *
 *	this modules implements the very simple on/off port
 *
 *	'API':
 *	------
 *	usr_config(): configuring the port, i.e., address etc.
 *	usr_set()   : setting the wait tics
 *	usr_on()    : activating the user port
 *	usr_off()   : deactivating the user port
 *
 */


#ifndef INCL_GUARD_USR
#define INCL_GUARD_USR

typedef void (* USR_HANDLER)( void * );// pointer to a usr-specific function

typedef struct {
	  char  state;		// current port state
	  char  new_state;	// handling external requests
	  void *usr_data;	// pointer to the user's private data
	  int   port_nr;	// handler to the actual hardware port
	  TICS  tics;		// the tics counter
	  USR_HANDLER usr_fnc;	// pointer to the user call-back function
	} USR;

#define usr_config( usr, usr_fnc, usr_data )	\
	usr__config( usr, (USR_HANDLER) usr_fnc, (void *) usr_data )

int  usr__config( USR *usr, USR_HANDLER usr_fnc, void *usr_data );
void usr_set    ( USR *usr, int tics_wait );
void usr_on     ( USR *usr );
void usr_off    ( USR *usr );

#endif
