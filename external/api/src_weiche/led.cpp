#include "sw-ports.h"
#include "led.h"

#define STATE_NOPE 0
#define STATE_UPDATE 1 



void led_state( LD* led ){

	if( led->new_state == STATE_UPDATE ){
#ifdef ESP32
//		Serial.printf("writing...\n");
//		led->tlc->write();
#endif
		led->new_state = STATE_NOPE;
	}
}

void led_config( LD*led,Adafruit_TLC5947* tlc ){
#ifdef ESP32
	led->tlc = tlc;
#endif

	for( int k = 0; k < CHANNELS; k++){
		led->arr[k] = 1024;
	}

//	swp_register( led_state, led ); 
}

void led_set( LD* led, int lednum, int val ){
	Serial.printf(" Changing %d to %d \n", lednum, val );

	if( lednum > CHANNELS )
			return;

	if( val > 4095 )
			val = 4095;

	if( val < 0 )
			val = 0;
	
	led->arr[lednum] = val;
	led->tlc->setPWM( lednum, val );
	if( led->new_state != STATE_UPDATE )
		led->new_state = STATE_UPDATE;

}
void led_write( LD* led ){
	if( led->new_state == STATE_UPDATE ){
		Serial.printf("writing...\n");
		led->tlc->write();
	}
	led->new_state=STATE_NOPE;
}
