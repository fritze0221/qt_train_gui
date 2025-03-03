
/*
 *	swp-conf.h
 *	prefix: swp_
 *
 *	this file jsut defines te number of available software ports.
 *	it should be the number of available hardware ports plus a 
 *	few for handling pure software ports, such as usr and notify.
 *
 */

#include "hwp-conf.h"
#define SWP_SIZE	HWP_SIZE + 16	// number of available software ports

