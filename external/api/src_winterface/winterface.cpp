#include "winterface.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>


//#define MAINBOARD
//#define TRAIN
//#define ESP32

#ifdef ESP32

#include "hw-ports.h"
#include "sw-ports.h"
#include "io-board.h"

#include "tics.h"
#include "stics.h"
#include "ramp.h"
#include "pwm.h"
#include "usr.h"
#include "led.h"
//#include "../src_weiche/pid-default.h"

#ifdef TRAIN
#include "dr.h"
#include "emk-adc.h"
#include "pid-default.h"
#include "tc.h"
#endif

#else

#include "../Adafruit_TLC5947/Adafruit_TLC5947.h"

#include "../src_weiche/hw-ports.h"
#include "../src_weiche/sw-ports.h"
#include "../src_weiche/io-board.h"
#include "../src_weiche/led.h"

#include "../src_weiche/tics.h"
#include "../src_weiche/stics.h"
#include "../src_weiche/ramp.h"
#include "../src_weiche/pwm.h"
#include "../src_weiche/usr.h"
#include "../src_weiche/pid-default.h"
#include "../src_weiche/weiche.h"

#ifdef TRAIN
#include "../src_train/pid-default.h"
#include "../src_train/emk-adc.h"
#include "../src_train/dr.h"
#include "../src_train/tc.h"
#endif

#endif // ESP32

#ifdef MAINBOARD

#include "../src_weiche/weiche.h"
extern LD* led;
extern SP *weichen[2];

#endif

#ifdef TRAIN
extern TC* tcp;
extern DR* dr;
#endif






//#include "tc.h"


int netw_int_len_helper(int x)
{
	if ( x >= 1000000000 ) return 10;
	if ( x >= 100000000 ) return 9;
	if ( x >= 10000000 ) return 8;
	if ( x >= 1000000 ) return 7;
	if ( x >= 100000 ) return 6;
	if ( x >= 10000 ) return 5;
	if ( x >= 1000 ) return 4;
	if ( x >= 100 ) return 3;
	if ( x >= 10 ) return 2;
	if ( x >= 1 ) return 1;
	return 0;
}
void netw_clear_arg( WINTP netw)
{
	for( int k =0 ;k < MAX_ARGC ; k++)
		netw->argv[k] = netw->endl;	
	
	netw->argc=0;
}
void netw_clear_buf( WINTP netw)
{
	memset(netw->buf.payload,'\0',N_BUFFER_SIZE);
}
void netw_clear( WINTP netw){
	netw_clear_arg( netw );
	netw_clear_buf( netw );
}
void netw_init( WINTP netw, int socket)
{
	netw->buf.header.size=0;
	memset( netw->buf.payload, 0 , N_BUFFER_SIZE );
	netw->buf.payload[0]='\0';
#ifndef ESP32
	netw->socket = socket;
#endif
	netw->argc=0;
	netw->endl= &netw->buf.payload[N_BUFFER_SIZE-1];
	netw->buf.header.size = N_BUFFER_SIZE;
	netw_clear_arg( netw );
	netw_clear_buf( netw ) ;
}
void netw_close( WINTP netw )
{
#ifndef ESP32
 shutdown(netw->socket, 0 );
#endif
}
void netw_analyze_payload( WINTP netw)
{
#ifdef ESP32
	Serial.printf("Payload |");
#else
	printf("Payload |");
#endif
	for(int k=0; k < N_BUFFER_SIZE; k++)
	{
		if( netw->buf.payload[k] == '\0' )
		{
#ifdef ESP32
			Serial.printf("_");
#else
			printf("_");
#endif
		}else{
#ifdef ESP32
			Serial.printf("%c",netw->buf.payload[k]);
#else
			printf("%c",netw->buf.payload[k]);
#endif
		}
	}
#ifdef ESP32
	Serial.printf("|\n");
#else
	printf("|\n");

#endif

}
void netw_analyze_arg( WINTP netw)
{
	printf("Analyze arg:\n");
	for(int k = 0; k < MAX_ARGC; k++ ){
		if( netw->argv[k][0] == '\0' )
		{
#ifdef ESP32
			Serial.printf("%d:_\n",k);
#else
			printf("%d:_\n",k);
#endif
		}else{
#ifdef ESP32
			Serial.printf("%d:%s\n",k,netw->argv[k]);
#else
			printf("%d:%s\n",k,netw->argv[k]);
#endif
		}
	}
}
void netw_generate_arg(WINTP netw)
{
	netw_clear_arg( netw );
	char *p = &netw->buf.payload[0];
	while( *p != '\0' ){
		netw->argv[netw->argc]=p;
		p = p+strlen(p)+1;		
		netw->argc++;
	}
}
int netw_check_string_safety( WINTP netw , int length_to_add)
{
	int space_left = N_BUFFER_SIZE;
	if( netw->argc > 0 ){ 
		space_left =  (netw->endl - netw->argv[ netw->argc-1 ])  - strlen(netw->argv[ netw->argc -1 ]) - 1; 
	}
//	printf("Space Left: %d \n",space_left);
	if( space_left > ( length_to_add) 
	&& ((netw->argc+1) <= MAX_ARGC )	 )
	{
		return 1;

	}
	printf("Debug Error! String not safe! \n");
	return 0;
}
void netw_set_argv( WINTP netw )
{
	if(netw->argc>0){
		netw->argv[netw->argc]=netw->argv[netw->argc-1]+1+strlen(netw->argv[netw->argc-1]);
	}else{ // argc == 0
		netw->argv[0] = &netw->buf.payload[0];
	}
}
char* netw_add_int(WINTP netw, int val )
{
	if( netw_check_string_safety( netw , netw_int_len_helper(val)) ){
		netw_set_argv(netw);
		sprintf(netw->argv[netw->argc],"%d",val);		
		netw->argc++;
	}
//	netw_analyze_payload(netw);
	return netw->argv[netw->argc];
}
char* netw_add_double(WINTP netw, double val )
{
	if( netw_check_string_safety( netw , DOUBLE_PRECISION+netw_int_len_helper(val))){
		netw_set_argv(netw);
		sprintf(netw->argv[netw->argc],DOUBLE_PRECISION_PRINTF,val);		
		netw->argc++;
	}
//	netw_analyze_payload(netw);
	return netw->argv[netw->argc];
}
char* netw_add_string(WINTP netw, char* val )
{
	if( netw_check_string_safety( netw, strlen(val)) ){
		netw_set_argv(netw);
		sprintf(netw->argv[netw->argc],"%s",val);		
		netw->argc++;
	}
//	netw_analyze_payload(netw);
	return netw->argv[netw->argc];
}


/*	send / recV	*/
void run_test(WINTP netw, int what)
{
	for( int k =0;k < 10; k++ )
	{
		if( what == 0)
		{
//			run_test_send( netw, k );
		}else{
//			run_test_recv( netw, k );

		}
	}
}
int netw_recv_header(WINTP netw)
{
#ifdef ESP32
#else
	recv(netw->socket, &(netw->buf.header), sizeof(netw->buf.header), 0);
#endif
	return netw->buf.header.size;
}
void netw_send(WINTP netw)
{
//	send(netw->socket, &netw->buf.header, sizeof(netw->buf.header),0);	
//	send(netw->socket, &netw->buf.payload, netw->buf.header.size,0);	
#ifdef ESP32
		for( int k = 0; k < N_BUFFER_SIZE; k++)
		{
			netw->client.write(	netw->buf.payload[k] );
		}
#else
	if( netw->socket ){
		send(netw->socket, &netw->buf.payload, N_BUFFER_SIZE,0);
	}else{
		printf("%s error: Socket zero ? \n",__func__);
	}
#endif

}
int netw_recv(WINTP netw )
{
//	netw_recv_header(netw);
//	recv(netw->socket, &netw->buf.payload, netw->buf.header.size,0);
#ifdef ESP32
//		Serial.printf("|");
		if( netw->client.available()>=N_BUFFER_SIZE)
		for( int k = 0; k < N_BUFFER_SIZE; k++)
		{
			netw->buf.payload[k] = netw->client.read();	
//			Serial.printf("%c",netw->buf.payload[k]);
		}
//		Serial.printf("|");
#else
	if( recv(netw->socket, &netw->buf.payload, N_BUFFER_SIZE,0) == 0 )
		return 0;
#endif
	netw_generate_arg(netw);
	return netw->buf.header.size;
}


int netw_handle_message(WINTP netw, PCMD arr)
{
	
	if( netw->argc > 0 )
	{
		int identifier = atoi( netw->argv[0] );	
		arr[identifier].function(netw->argc,netw->argv);	
    return identifier;
	}
  return -1;
}

void cmd_nope(int argc, char **argv){
#ifdef ESP32
	Serial.printf("%s CALLED! with argument: %d\n",__func__, atoi(argv[1]));
#else
	printf("%s CALLED! with argument: %d\n",__func__, atoi(argv[1]));
#endif
	
}


#ifdef TRAIN
	#include "winterface-train.cpp"
#endif
#ifdef MAINBOARD
	#include "winterface-mb.cpp"
#endif


void cmd_print_message(int argc, char**argv)
{
#ifdef ESP32
	Serial.printf("%s CALLED!\n",__func__);
		for(int k = 1; k < argc; k++ )
			Serial.printf("%s\n",argv[k]);	
#else
		//.... 
#endif	
};
void cmd_arr_init( PCMD arr	)
{
	for( int k=0;k<CMD_ARR_ENTRYS;k++)
	{
			arr[k].function = cmd_nope ;
	}
	
#ifdef TRAIN
	arr[ TCP_SPEED].function = cmd_tc_speed; 
	arr[ TCP_STOP].function = cmd_tc_stop; 
	arr[ TCP_RAMP].function = cmd_tc_ramp; 
	arr[ TRAIN_SET_DIR].function = cmd_set_dir; 
	arr[ REPORT_SPEED ].function = cmd_tc_report; 
#endif

#ifdef MAINBOARD
	arr[ WEICHE ].function = cmd_mainboard_weichen; 
	arr[ LED_SET ].function = cmd_mainboard_led_set; 
	arr[ LED_WRITE ].function = cmd_mainboard_led_write; 
#endif 
	arr[ CMD_SERIAL_PRINT_MSG ].function =  cmd_print_message; 
}





/* TEST FUNCTIONS */

/*
int test_netw_generate_arg(WINTP netw)
{
	netw_init(netw,0);
	netw_clear_buf(netw);
	netw_add_int(netw, 3);
	netw_add_int(netw, 4);
	netw_add_int(netw, 5);
	netw_add_double(netw, 15.12451);
	netw_add_string(netw, "Quadratlatsche");
	
	netw_clear_arg(netw);
	netw_analyze_payload(netw);
	netw_generate_arg(netw);
	netw_analyze_arg(netw);


	return 1;
	
}
void run_test_winterface(WINTP netw)
{
//	char buf[256]= "1\02\0";
	int test_case = 0;
	printStatus(test_case++, test_netw_add_int( netw) );
	printStatus(test_case++, test_netw_add_int2( netw) );
	printStatus(test_case++, test_netw_add_double( netw) );
	printStatus(test_case++, test_netw_add_string( netw) );
	printStatus(test_case++, test_netw_generate_arg( netw ));	
}
void run_test_send( WINTP netw, int testnr )
{
	char teststring[]="Lululululalala";
	switch(testnr)
	{
	case 0:
		netw_add_int(netw, 5 );
		netw_add_double(netw, 3.1456);
		netw_add_string(netw,"Quatsch");

		netw_analyze_payload( netw );
		netw_send(netw);
		break;
	case  1:
		break;
	case 2:
		break;

	}
}
void run_test_recv( WINTP netw, int testnr )
{
	int success=1;
	switch(testnr)
	{
	case 0:
		netw_recv( netw );
		netw_analyze_payload( netw );
		break;
	case 1:
		break;
	case 2:
		break;

	break;

	}
//	printStatus( testnr, success );
}
void printStatus( int testnr, int success )
{

	if( success )
	{
		printf("Success! Testnr: %d \n",testnr);
	}else{
		printf(" Failed! Testnr: %d \n",testnr);
	}

}
int test_netw_add_string(WINTP netw ){
	netw_init(netw,0);
	char test[64] = "pups!";
//	sprintf(test,"%d",val);
	netw_add_string(netw, test);
	netw_add_string(netw, test);
	netw_analyze_payload(netw);

	if( !strcmp( netw->argv[ 0], netw->buf.payload ) )
	{
		return 1;
	}else{
		printf("%s\n",netw->buf.payload);		
		return 0;
	}
}
int test_netw_add_double(WINTP netw ){
	netw_init(netw,0);
	double val = 5;
	char test[64] = "";
	sprintf(test,"%f",val);
	netw_add_double(netw, val);
	val= 59.12;
	netw_add_double(netw, val);
	netw_analyze_payload(netw);	
	netw_analyze_arg(netw);
	if( !strcmp( netw->argv[ 0], netw->buf.payload ) )
	{
		return 1;
	}else{
		printf("%s\n",netw->buf.payload);		
		return 0;
	}
}
int test_netw_add_int(WINTP netw ){
	netw_init(netw,0);
	int val = 5;
	char test[64] = "";
	sprintf(test,"%d",val);
	netw_add_int(netw, val);
	if( !strcmp( netw->argv[ 0], netw->buf.payload ) )
	{
		return 1;
	}else{
		printf("%s\n",netw->buf.payload);		
		return 0;
	}
}
int test_netw_add_int2(WINTP netw ){
	netw_init(netw,0);
	int val = 5;
	char test[64] = "";
	sprintf(test,"%d#%d#",val,val);
	netw_add_int(netw, 15);
	netw_add_int(netw, 167);
	if( !strcmp( netw->argv[ 0], "15")  
	&&  !strcmp( netw->argv[ 1], "167" )){
		return 1;
	}else{
		return 0;
	}
}
int test_netw_add_int3(WINTP netw ){
	netw_init(netw,0);
	int val = 5;
	char test[64] = "";
	sprintf(test,"%d#%d#",val,val);
	netw_add_int(netw, 0);
	netw_add_int(netw, 1);
	netw_add_int(netw, 2);
	netw_add_int(netw, 3);
	netw_add_int(netw, 4);
	netw_add_int(netw, 5);
	netw_add_int(netw, 6);
	netw_add_int(netw, 7);
	netw_add_int(netw, 8);
	netw_add_int(netw, 9);
	netw_add_int(netw, 10);
	netw_add_int(netw, 11);
	netw_analyze_payload(netw);
//	netw_analyze_arg(netw);

	for(int k = 0; k < 6; k++)
	{	
		if( atoi(netw->argv[ k ]) != k)  
		{
			printf("Schrott\n");
			return 0;
		}
	}
	return 1;
}
*/
