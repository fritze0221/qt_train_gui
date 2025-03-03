
/*
 *	name:   pwm.h
 *	prefix: pwm_
 *
 *	this modules implements the classical PWM (puls-width modulation)
 *	functionality.
 *
 *	'API':
 *	------
 *	pwm_state() : the interrupt handler
 *	pwm_config(): configuring the port, i.e., address etc.
 *	pwm_set()   : setting the number of on- and off-tics
 *	pwm_start() : starting PWM
 *	pwm_stop()  : stopping PWM; keeping tics
 *	pwm_full()  : setting PWM to constant 1
 *
 */

typedef struct {
	  char state;		// current port state
	  char new_state;	// new port state by external requests
	  int  port_id;		// the actual port interface
	  TICS tics;		// values for current and new tics
	} PWM;

void pwm_state ( PWM *pwm );
int  pwm_config( PWM *pwm, int port_id );
void pwm_set   ( PWM *pwm, int bresenham, int on_tics, int off_tics );
void pwm_start ( PWM *pwm );
void pwm_stop  ( PWM *pwm );
void pwm_full  ( PWM *pwm );

#define pwm_bresenham( pwm, on_tics, off_tics )	\
	pwm_set( pwm, 1, on_tics, off_tics )
#define pwm_regular( pwm, on_tics, off_tics )	\
	pwm_set( pwm, 0, on_tics, off_tics )

