#include "weiche.h"


void weiche_set( SP** weiche, int id, int val ){

	if( id > WEICHEN_CNT || id < 0 )
		return;

	if( val > 0 ){
		sp_on( weiche[id] );
	}else{
		sp_off( weiche[id]  );
	}

	return;
}


