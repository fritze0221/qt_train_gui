
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

typedef struct _MC {
	  char   state;		// current port state
	  char   new_state;	// external requests for state change
	  int    port_id;	// the actual port interface
	  ADC   *adc;		// reference to the associated ADC, if any
	  TICS   tics;		// PWM tics
	  int   (* stream)( struct _MC *mcp );	// for STREAMING-mode only
	} MC, *MCP;

typedef int  (* MC_STREAM)( MCP mcp );
typedef void (* MC_PID)   ( MCP mcp, int *on_tics, int *off_tics );

int  mc_config   ( MCP mcp, int port_id, int adc_portid );
void mc_adcconfig( MCP mcp, int settle, int wait, int repeat );
void mc_pid      ( MCP mcp, MC_PID pid );
void mc_speed    ( MCP mcp, int on_tics, int off_tics, int sync );
void mc_stream   ( MCP mcp, MC_STREAM stream );
void mc_stop     ( MCP mcp );


