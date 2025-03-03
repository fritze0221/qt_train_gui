
/*
 *	tc.h
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

typedef int (* TC_PID     )( PWM *, int, int, void * );

typedef struct {
	  int     state;	// current control mode
	  int     new_state;	// new control mode
	  int     target_speed;	// the speed with which we want to go
	  int     current_speed;// the current speed measured by some task
	  int     current_speed_avg;// the current speed measured by some task
	  RMP     ramp;		// the data that is processed at every tic
	  PWM    *pwm;		// motor control unit
	  int     periode;	// duration of one (on_tics + off_tics) cycle
	  EMK    *emk;		// ADC to measure the motor's idle speed
	  int     min_emks;	// minimum for emk to be valid
	  TC_PID  pid;		// the actual PID controller function
	  void   *pid_data;	// pid's private data
	  PID_STD pid_std;	// data for the default pid-controller
	  USR     tc_cmd;	// data for the usr call-back function tc_cmds()
	} TC;

void tc_state  ( TC *tcp );
int  tc_config ( TC *tcp, PWM *pwm, EMK *emk );
void tc__setpid( TC *tcp, TC_PID pid, void *pid_data );
void tc_ramp   ( TC *tcp, int new_target, int duration );
int  tc_ds2dt  ( int from_target, int to_target, int delta_speed, int scale );

#define tc_setpid( tcp, pid, pid_data ) \
	tc__setpid( tcp, (TC_PID) pid, (void *) pid_data )

#define tc_setcurrent( tcp, speed )	((tcp)->current_speed = speed)

#define tc_rampa( tcp, new_target, delta_speed, scale )	\
	tc_ramp( tcp->target_speed, new_target, \
	         tc_ds2dt(tcp, new_target, delta_speed, scale))

#define tc_settarget( tcp, new_speed )	tc_ramp( tcp, new_speed, 0 )
#define tc_stop( tcp )			tc_settarget( tcp, 0 )
	// all interrupts are still running









