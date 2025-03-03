
/*
 *	tc.c
 *	prefix: tc_
 *
 *	train control, a controller for a model train. normally, the
 *	train rides at constant speed, which might be changed though ;-)
 *	the interface also offers a feature called 'ramp'. this means,
 *	that the change from the current to the future target speed is
 *	slowly adapted over time. the time span is called duration.
 *
 *	the actual pid-controller is outside this module. the controller
 *	function is registered by tc_setpid(). 
 *
 *	note on tc_rampa() and rampa2(): because of integer arithmetic
 *	both functions calculate:
 *		durtion = (to_target-from_target)*scale/delta_speed.
 *
 *	'API':
 *      ------
 *	tc_state     (): the interrupt handler
 *	tc_config    (): configure the data structure
 *	tc_setpid    (): register a pid-controller
 *	tc_ramp      (): set target speed ramp by duration
 *	tc_rampa     (): set target speed ramp by acceleration
 *	tc_ds2dt     (): convert acceleration to duration
 *	tc_setcurrent(): set current speed
 *	tc_settarget (): set target speed, immediately without ramp
 *	tc_stop      (): stop train
 *
 */

#include "sw-ports.h"
#include "hw-ports.h"
#include "ramp.h"
#include "tics.h"
#include "stics.h"
#include "pwm.h"
#include "emk-adc.h"
#include "pid-default.h"
#include "usr.h"
#include "tc.h"

#define NONE		0	// for new_mode only
#define INACTIVE	1	// ideling....
#define ACTIVE		2	// riding at the specified speed

//	tc_state() handles all motor control commands, such as set speed,
//	set ramp etc. it should be called at each time tic, i.e., it
//	should be registed at the usr software port. furthermore,

void tc_state( TC *tcp )
    {
       int on, off;
       // test ramp setting and update, if required
       if ( rmp_take( & tcp->ramp ) )
          if ( rmp_duration( & tcp->ramp ) == 0 )
	     tcp->periode = 0;		// don't wait, call pid() immediately
       rmp_step( & tcp->ramp, & tcp->target_speed );
       if ( tcp->new_state == ACTIVE || tcp->new_state == INACTIVE )	
	  pwm_set( tcp->pwm, 0, 0, 1 );	// output should be off
       tcp->new_state && (tcp->state = tcp->new_state);
       tcp->new_state = NONE;
       if ( tcp->state == ACTIVE )	// no pid and pwm if INACTIVE
          if ( tcp->periode-- <= 1 )
	     if (( tcp->emk == 0 || emk_emks( tcp->emk ) >= tcp->min_emks )
	         && tcp->pid )	// no pid? then wait one cycle: tc_setpid()
	     {
		(tcp->emk) && (tcp->current_speed = emk_average( tcp->emk ));
		tcp->periode = tcp->pid( tcp->pwm, tcp->target_speed,
					 tcp->current_speed, tcp->pid_data );
// TODO 

         tcp->current_speed_avg = tcp->current_speed_avg + ( tcp->current_speed - tcp->current_speed_avg )*0.1;




	     }
	     else pwm_set( tcp->pwm, 0, 0, 1 );	// output should be off
    }

int tc_config( TC *tcp, PWM *pwm, EMK *emk )
    {
       tcp->target_speed = tcp->current_speed = tcp->current_speed_avg = 0;
       tcp->pwm          = pwm;
       tcp->emk          = emk;
       tcp->min_emks     = 1;
       tcp->periode      = 1;
       tcp->state        = ACTIVE;
       tcp->new_state    = NONE;
       Serial.println(" setpid ");
       tc_setpid( tcp, pid_std, & tcp->pid_std );
       Serial.println(" usr set ");
       usr_set   ( & tcp->tc_cmd, 1 );		// calling every tic
       Serial.println(" usr config ");
       usr_config( & tcp->tc_cmd, tc_state, tcp );
       Serial.println(" usr on ");
       usr_on    ( & tcp->tc_cmd );		// activating tc_state
	Serial.println("tc_config done ");
	return 0;
    }

void tc__setpid( TC *tcp, TC_PID pid, void *pid_data )
     {
	tcp->pid = 0;			// skipping the pid()-call in tc_state
	tcp->pid_data = pid_data;	// reset the private data pointer
	tcp->pid = pid;			// now, pid() can be called in tc_state
     }

void tc_ramp( TC *tcp, int new_target, int duration )
     {
static int last = millis();
  int mil = millis();
  Serial.printf("%d tc ramp called w target_speed: %d\n",mil-last, new_target);
  last = mil;
	rmp_newramp( & tcp->ramp, tcp->target_speed, new_target, duration );
     }

int tc_ds2dt( int from_target, int to_target, int delta_speed, int scale )
    {
       int tmp;
       tmp = (to_target - from_target) * scale;
       tmp /= delta_speed;
       return (tmp < 0)? -tmp: tmp;
    }

