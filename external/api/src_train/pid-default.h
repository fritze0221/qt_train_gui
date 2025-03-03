
/*
 *	pid-default.h
 *	prefix: pid_		// like pi-default
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

typedef struct _pid_std {		// all the data for the regulator
	  struct _pid_std *new_pid;	// new configuration available?
	  double p_gain;		// proportional gain
	  double i_gain;		// integrator gain
	  int    i_value;		// accumulated integrator value
	  int    max_ivalue;		// maximal integrator value
	  int    max_tics;		// number of tics per regulation cycle
	} PID_STD;

int  pid_std( PWM *pwm, int target, int current, PID_STD *pid_data );
void pid_config( PID_STD *pid, double p_gain, double i_gain,
                               int max_ivalue, int max_tics );
void pid_strconfig( PID_STD *pid, char *config_str );


