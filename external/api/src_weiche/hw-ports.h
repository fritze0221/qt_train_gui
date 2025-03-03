
/*
 *	hw-ports.h
 *	prefix: hwp_
 *
 *	interface to hardware components, such as ports.
 *
 *	'API':
 *      ------
 *	hwp_size()  : number of available ports
 *	hwp_init()  ; initializing all ports, i.e., setting to off
 *	hwp_config(): configure a port
 *	hwp_write() : write to pin
 *	hwp_read()  : read from pin
 */

#include "hw-ports-cf.h"

#define HWP_UNUSED	0	// port not in use; technically: HWP_BININPUT
#define HWP_BIN_IN	1	// configured as binary  input  port
#define HWP_INT_IN	2	// configured as integer input  port, i.e., ADC
#define HWP_BIN_OUT	3	// configured as binary  output port
#define HWP_INT_OUT	4	// configured as integer output port

int  hwp_size();
void hwp_init();
void hwp_config( int nr, int mode );
void hwp_write ( int nr, int data );
int  hwp_read  ( int nr );

#define hwp_on(  nr )	hwp_write( nr, 1 )
#define hwp_off( nr )	hwp_write( nr, 0 )


