
/*
 *	pid-default.c
 *	prefix: pid_		// like pid default
 *
 *	here, we implement a standard PI controller for the model train.
 *	all the relevant parameters are set to reasonable standard values,
 *	but might be changed by calling pid_config().
 *
 *	the actual controller, pid_control(), has four values, the
 *	target speed, the current speed, and pointers to the new
 *	on and off tics values.
 *
 *	'API':
 *      ------
 *	pid_control() : the actual controller
 *	pid_config () : the configuration tool
 *	pid_strconfig(): initialize by string argument
 *
 */

// #include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "tics.h"
#include "pwm.h"
#include "pid-default.h"

int pid_std( PWM *pwm, int target, int current, PID_STD *pid_data )
    {

       if ( pid_data->new_pid )
	   *pid_data = *(pid_data->new_pid);

  current-=166; // TODO bias of standing train...
  if( current < 0 ) 
     current = 0;

  int out, diff = target - current, max_iv = pid_data->max_ivalue;
  int on_tics, off_tics;
  static int i_sum=0;
  static int curr_sum=0;
/*
	if ( (current == 0) && (target > 0) )
	{
		Serial.printf("Ruck!\n");
		on_tics = 15;
		off_tics = 5;
	  	pwm_set( pwm, 0, on_tics, off_tics );		// regular
		return on_tics+off_tics;
	}
*/

	i_sum += diff;
	if( i_sum > max_iv ) i_sum = max_iv;
	if( i_sum < -max_iv ) i_sum = -max_iv;

	on_tics = diff * pid_data->p_gain  + i_sum * pid_data->i_gain  ;
	if ( on_tics  < 0 )
		on_tics = 0;

	int max_tics;
	if( target > 2000 )
		max_tics = pid_data->max_tics*6;
	else
		max_tics = pid_data->max_tics;


	if( on_tics > max_tics )
		on_tics = max_tics;

	off_tics = max_tics - 4 -  on_tics ;
	if ( off_tics < 0 )
		off_tics = 0;
  curr_sum += current;
static int cnt = 0;
	if( (cnt++) == 1000 ){
    curr_sum/=1000;
		Serial.printf("target = %d current: %d  i-val: %d out: %d  on: %d  off: %d\n",target,curr_sum, i_sum, out,on_tics,off_tics);
		cnt = 0;
    curr_sum = 0;
	}

        pwm_set( pwm, 1 , on_tics, off_tics );		// bresenham
	return off_tics + on_tics;
       }

void pid_config( PID_STD *pid, double p_gain, double i_gain,
		               int max_ivalue, int max_tics )
     {
	static PID_STD new_pid;
        new_pid.i_value    = 0;		// reset integrator accumulator
        new_pid.p_gain     = p_gain;	// proportinal gain
        new_pid.i_gain     = i_gain;	// integral gain
        new_pid.max_ivalue = max_ivalue;// maximal integrator value (limit)
	new_pid.max_tics   = max_tics;
	new_pid.new_pid    = 0;
	pid->new_pid = & new_pid;
     }

void pid_strconfig( PID_STD *pid, char *config_str )
     {
        double p_gain, i_gain;
        int    max_ivalue, max_tics;
	/*
	sscanf( config_str, "%le%lf%d%d",
	        & p_gain, & i_gain, & max_ivalue, & max_tics );
	*/
	p_gain = atof( config_str );
	while( *config_str && *config_str != ' ' )
	   config_str++;
	while( *config_str == ' ' )
	   config_str++;
	i_gain = atof( config_str );
	while( *config_str && *config_str != ' ' )
	   config_str++;
	while( *config_str == ' ' )
	   config_str++;
	max_ivalue = atoi( config_str );
	while( *config_str && *config_str != ' ' )
	   config_str++;
	while( *config_str == ' ' )
	   config_str++;
	max_tics = atoi( config_str );
        pid_config( pid, p_gain, i_gain, max_ivalue, max_tics );
     }

/*

typedef struct ....
	   int start;

	   init: pid_start = 0;

pid_std:
       int tmp;
       ....

       // perhaps current == 0 ---> current <= small_value
       if ( pid_data->start || (current == 0 && target > 0))
       {
	  pid_data->start++;
	  tmp = pid_data->start + 6;
	  pwm_set( pwm, 0, pid_data->start, 6 );// 6 is just random here
	  if ( current > 0)		// are we riding?
	     pid_data->start = 0;	// ok, returning to normal operation
	  return tmp;
       }
       ....


*/
