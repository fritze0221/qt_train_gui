
/*
 *	name:   mc.h
 *	prefix: mc_
 *
 *	this modules implements the rather complex motor control port.
 *
 *	'API':
 *	------
 *	mc_config()   : configuring the ports, i.e., motor output and adc input
 *	mc_adcconfig(): configuring A/D controller
 *	mc_pid()      : configuring pid regulator mode
 *	mc_speed()    : configuring speed mode
 *	mc_stream()   : configuring streaming mode
 *	mc_stop()     : stop and set output to off
 *
 */

#include "adc.h"


typedef struct _MC {
	  char state;		// current port state
	  char new_state;	// external requests for state change
	  int  port_id;		// the actual port interface
	  ADC *adc;		// reference to the associated ADC, if any
	  TICS tics;		// PWM tics
	  int  (* stream)( struct _MC *mcp );	// for STREAMING-mode only
	} MC, *MCP;

typedef int  (* MC_STREAM)( MCP mcp );

void mc_state ( MCP mcp );
int  mc_config( MCP mcp, int port_id, ADC *adc );
void mc_speed ( MCP mcp, int bresenham, int on_tics, int off_tics );
void mc_stream( MCP mcp, MC_STREAM stream );
void mc_stop  ( MCP mcp );

