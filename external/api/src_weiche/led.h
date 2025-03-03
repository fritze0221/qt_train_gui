
#include <cstdint>
#ifdef ESP32
	#include "Adafruit_TLC5947.h"
#endif

#define CHANNELS 24

typedef struct _ld{
	uint16_t arr[CHANNELS];	
	char new_state;
#ifdef ESP32
	Adafruit_TLC5947 *tlc;
#endif
}LD;

//Adafruit_TLC5947 init_led(int pin_data, int pin_clock, int pin_latch);
void led_set(LD* led, int lednum , int val );
void led_state( LD* led );
void led_config( LD* led, Adafruit_TLC5947 *tlc  );
void led_write( LD* led );




