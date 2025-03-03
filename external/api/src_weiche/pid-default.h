
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
 *	pid_control(): the actual controller
 *	pid_config (): the configuration tool
 *
 */

//	scaling of speed and regulation periode
#define PID_DURATION	800	// 800 tics
#define MAX_SPEED	500	// maximum speed reading

//	default PID values
#define P_GAIN		3
#define I_GAIN		10

//	limits
#define MAX_IVALUE	100	// not PI but LAG regulator
#define MAX_ACCEL	100	// dummy right now
#define MAX_DECEL	100	// dummy right now

void pid_control( int target, int current, int *on_tics, int *off_tics );
void pid_config ( int p_gain,    int i_gain,    int max_ivalue,
	          int max_speed, int max_accel, int max_decel );


