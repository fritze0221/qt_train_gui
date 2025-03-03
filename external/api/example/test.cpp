#include <stdio.h>
#include <unistd.h>
#include "../src_winterface/winterface-api.h"


void tester(){


#define TEST_HARDWARE  // Wenn aktiv, werden alle Befehle mit der Modellbahn durchgefuehrt

#define TEST_TRAIN_Y
//#define TEST_TRAIN_B
#define TEST_TRAIN_MB



#ifdef TEST_TRAIN_Y
#ifdef TEST_HARDWARE
	printf("Testing TRAIN_Y...\n");
	WINTP train_y = connection_init( TRAIN_Y, TRAIN_PORT_Y );
#else
	printf("Simulate TRAIN_Y \n");
	WINTP train_y = connection_init( DUMMY_Y, TRAIN_PORT_Y );
#endif
	printf("Forward...\n");
	train_setDirection( train_y,  FORWARD ); // 0 .. 1 
	train_setSpeed( train_y, 400 ); 
	train_setSpeed( train_y, 400 ); 
  int speed = train_reportSpeed( train_y );
  printf("My speed is.. %d\n",speed);
	sleep(1);
	printf("Stop and backwards...\n");
	train_setSpeed( train_y, 0 ); 
	train_setDirection( train_y,  BACKWARD ); // 0 .. 1 
	train_setSpeed( train_y, 400 ); 
	sleep(1);
	train_setSpeed( train_y, 0 ); 
#endif

#ifdef TEST_TRAIN_B
#ifdef TEST_HARDWARE
	printf("Testing TRAIN_B...\n");
	WINTP train_b = connection_init( TRAIN_B, TRAIN_PORT_B );
#else
	printf("Simulate TRAIN_B...\n");
	WINTP train_b = connection_init( DUMMY_B, TRAIN_PORT_B );
#endif
	printf("Forward...\n");
	train_setDirection( train_b,  FORWARD ); // 0 .. 1 
	train_setSpeed( train_b, 800 ); 
  int speed = train_reportSpeed( train_b );
  printf("My speed is.. %d\n",speed);
	sleep(1);
	printf("Stop and backwards...\n");
	train_setSpeed( train_b, 0 ); 
	train_setDirection( train_b,  BACKWARD ); // 0 .. 1 
	train_setSpeed( train_b, 800 ); 
	sleep(1);
	train_setSpeed( train_b, 0 ); 

#endif

#ifdef TEST_TRAIN_MB
#ifdef TEST_HARDWARE
	printf("Testing MB...\n");
	WINTP mb = connection_init( MB, MB_PORT );
#else
	printf("Simulate MB...\n");
	WINTP mb = connection_init( DUMMY_MB, MB_PORT );
#endif

	mainboard_setWeiche( mb, 1, 1 ); // 0 .. 1 
	mainboard_setWeiche( mb, 0, 1 ); // 0 .. 1 
	sleep(2);
	mainboard_setWeiche( mb, 1, 0 ); // 0 .. 1 
	mainboard_setWeiche( mb, 0, 0 ); // 0 .. 1 

	for( int i = 0; i < 5; i++ ){
		for( int k=0; k < 12; k++){
			mainboard_setLed( mb, k, 4095 ); // 0 .. 1 
			if( k == 11 ) 	// Windrad
				mainboard_setLed( mb, k, 800 ); // 0 .. 1 

		}
		mainboard_writeLed( mb );
		sleep(1);
		for( int k=0; k < 12; k++){
			mainboard_setLed( mb, k, 0 ); // 0 .. 1 
		}
		mainboard_writeLed( mb );
		sleep(1);
	}
#endif


}

int main(int argc, char**argv){

	tester();



}
