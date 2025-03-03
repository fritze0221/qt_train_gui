
/*
 *	hw-ports.h
 *	prefix: hwp_
 *
 *	interface to hardware components, such as ports.
 *
 *	'API':
 *      ------
 *	hwp_size()  : number of available ports
 *	hwp_init()  : initializing all ports, i.e., setting to off
 *	hwp_config(): configure a port
 *	hwp_write() : write to pin
 *	hwp_read()  : read from pin
 */

#include "hw-ports.h"

typedef struct {
	  int address;		// the hardware address
	  int mode;		// mode of operation, i.e., in, out, ...
	  int last_data;	// last value read or written
	} HWP_PORT;

static HWP_PORT hwp_tab[ HWP_SIZE ];

/****************************************/
/*	the LINUX primitives		*/
/****************************************/

#ifdef LINUX_DEV

   #include <stdio.h>

   #define INPUT	"input"
   #define OUTPUT	"output"
   #define pinMode( addr, mode ) \
   	   printf( "pinMode: addr=0x%X mode='%s'\n", addr, mode )

   #define digitalWrite( addr, val ) \
   	   printf( "digitalWrite: port=0x%X value=%d\n", addr, val )
   #define analogWrite(  addr, val ) \
   	   printf( "analogWrite: port=0x%X value=%d\n", addr, val )

   int linux_input( char *name, int address )
       {
	  int val;
          printf( "%sRead: from address 0x%X: ", name, address );
          scanf( "%d", & val );
	  return val;
       }
   #define digitalRead( addr )	linux_input( "digital", addr )
   #define analogRead(  addr )	linux_input( "analog" , addr )

#endif

/****************************************/
/*	the ESP32 primitives		*/
/****************************************/

#ifdef ESP32
   #include <Arduino.h>
//void analogWrite(int,int){};
#endif

/****************************************/
/*	the I/O functions		*/
/****************************************/

int hwp_size() { return HWP_SIZE ; }

void hwp_init()
     {
	int i, addr_list[] = HWP_ADDRLIST;
	int adc_list[] = HWP_ADCLIST, dac_list[] = HWP_DACLIST;
	for( i = 0; i < HWP_SIZE; i++ )		// init address und mode only
	{
	   hwp_tab[ i ].address = addr_list[ i ];
	   hwp_tab[ i ].mode    = HWP_UNUSED;
	}
	for( i = 0; i < sizeof( adc_list )/sizeof( *adc_list ); i++ )
	   hwp_config( adc_list[ i ], HWP_INT_IN );
	for( i = 0; i < sizeof( dac_list )/sizeof( *dac_list ); i++ )
	   hwp_config( dac_list[ i ], HWP_INT_OUT );
	for( i = 0; i < HWP_SIZE; i++ )
	   if ( hwp_tab[ i ].mode == HWP_UNUSED )
	      hwp_config( i, HWP_BIN_IN );
     }

void hwp_config( int nr, int mode )
     {
        HWP_PORT *p = hwp_tab + nr;
	p->mode = mode;
	if ( mode == HWP_BIN_OUT || mode == HWP_INT_OUT )
	{
	   p->last_data = -1;
	   pinMode( p->address, OUTPUT );
	   hwp_write( nr, 0 );
	}
	if ( mode == HWP_BIN_IN || mode == HWP_INT_IN )
	{
	   pinMode( p->address, INPUT  );
	   hwp_read( nr );
	}
	if ( mode == HWP_UNUSED )
	   pinMode( p->address, INPUT  );
     }

void hwp_write( int nr, int data )
     {
	HWP_PORT *p = hwp_tab + nr;
	if ( p->last_data != data )
	   if ( p->mode == HWP_INT_OUT )
//	        ledcWrite ( p->address, data );
	       digitalWrite(p->address, data );
	   else digitalWrite( p->address, data );
	p->last_data = data;
     }

int hwp_read( int nr )
    {
       HWP_PORT *p = hwp_tab + nr;
       if ( p->mode == HWP_INT_IN ){
	    p->last_data = analogRead ( p->address );
       }
       else if ( p->mode == HWP_BIN_IN )
               p->last_data = digitalRead( p->address );
       return p->last_data;
    }



