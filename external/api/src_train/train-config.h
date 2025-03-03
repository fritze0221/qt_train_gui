
/*
 *	train-config.h
 *	prefix: init_
 *
 *	here, we define the various initialization values that
 *	depend on the chosen train.
 *
 *	'API':
 *      ------
 */

//------------ the nice tender train --------------

#define TENDER_WAIT	1
#define TENDER_REPEAT	2

	// pid_std values
#define TENDER_STD_PGAIN	0.1
#define TENDER_STD_IGAIN	0.1
#define TENDER_STD_MAXIVALUE	1000
#define TENDER_STD_MAXTICS	8

#define TENDER_STD		"0.1 0.2 200 16"

//------------ the very simple, low-cost train --------------

// they are all phantasie values....

#define LOWCOST_WAIT	4
#define LOWCOST_REPEAT	2
#define LOWCOST_IGAIN	1
#define LOWCOST_PGAIN	1

//------------ the definition of some initializaation macros ---------

#define conf_emk( p, x )	emk_set( p, x ## _WAIT, x ## _REPEAT )
#define conf_pidstd( p, x )	pid_config( p, x ## _PGAIN, x ## _IGAIN, \
					    x ## _MAXIVALUE, x ## _MAXTICS )

// example conf_emk( p, TENDER )


