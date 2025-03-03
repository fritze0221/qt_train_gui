#include "hw-ports.h"
#include "sw-ports.h"
#include "sp.h"
#include "dr.h"

//#include <Arduino.h>
//extern Serial;

#define FORWARD 1
#define BACKWARD -1

void dr_config(DR* dr, SP* dr_0, SP* dr_1 ){
	dr->dr_0 = dr_0;
	dr->dr_1 = dr_1;
	dr_set( dr, FORWARD );
	swp_register( sp_state, dr->dr_0 );
	swp_register( sp_state, dr->dr_1 );
	
}
void dr_set( DR *dr, int set_to ){
//	Serial.printf("%s I am called! ",__func__);
//	if( set_to != dr->curr_dr ){

		if( set_to == FORWARD ){
			sp_off( dr->dr_0 );
			sp_on( dr->dr_1 );	
			dr->curr_dr = FORWARD;
		}
		if(set_to == BACKWARD){
			sp_on( dr->dr_0 );
			sp_off( dr->dr_1 );	
			dr->curr_dr = BACKWARD;
		}
//	Serial.printf("direction set to: %d\n",dr->curr_dr);
//	}
}

