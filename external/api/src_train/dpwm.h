
/*
 *	name:   dpwm.h
 *	prefix: dpwm_
 *
 *	this modules implements the classical PWM (puls-width modulation)
 *	functionality.
 *
 *	'API':
 *	------
 *	dpwm_state() : the interrupt handler
 *	dpwm_config(): configuring the port, i.e., address etc.
 *	dpwm_set()   : setting the number of on- and off-tics
 *	dpwm_start() : starting PWM
 *	dpwm_stop()  : stopping PWM; keeping tics
 *
 */

typedef struct {
	  int  port_id;		// the actual port interface
	  int  state;		// current port state
	  int  new_state;	// new port state by external requests
	  TICS sw_tics;		// software tics
	  TICS hw_tics[ 2 ];	// [0] if sw_state == OFF; [1] if sw_state == ON
	} DPWM;

void dpwm_state ( DPWM *pwm );
int  dpwm_config( DPWM *pwm );
void dpwm_set   ( DPWM *pwm, int sw_on, int sw_off,
		     int high_on, int high_off, int low_on, int low_off );
void dpwm_start ( DPWM *pwm );
void dpwm_stop  ( DPWM *pwm );


