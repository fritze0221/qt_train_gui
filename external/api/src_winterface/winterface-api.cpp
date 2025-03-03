#include "winterface-api.h"

WINTP connection_init(const char *TRAIN , int PORT){
	int sockD = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servAddr;
    	servAddr.sin_family = AF_INET;
    	servAddr.sin_port = htons(PORT); // use some unused port number
// 	servAddr.sin_addr.s_addr = INADDR_ANY;
//	servAddr.sin_addr.s_addr = inet_addr("192.168.2.22");

	if( strncmp( TRAIN, TRAIN_Y, strlen(TRAIN_Y) ) || strncmp( TRAIN, TRAIN_B, strlen(TRAIN_B) )){
		servAddr.sin_addr.s_addr = inet_addr(TRAIN);
	}else{
		printf("Func: %s %d: kann nichts mit der IP %s anfangen. Brauche %s oder %s \n",__func__,__LINE__, TRAIN, TRAIN_Y, TRAIN_B ); 
	}
    	int connectStatus = connect(sockD, (struct sockaddr*)&servAddr,sizeof(servAddr));
    	if (connectStatus == -1) {
    	    printf("Func: %s %d: %s \n",__func__, __LINE__, strerror(errno));
    	}
	WINTP netw = (WINTP)malloc(sizeof(WINT));
	netw_init( netw, sockD );
	return netw;
}	
void connection_close(WINTP conn){
 netw_close( conn );
 free( conn ); 
}
int catch_response( WINTP train){
  char buf[16];
#ifndef ESP32
  recv( train->socket, &buf, 16 ,0);
#endif
//  printf("%s\n",buf);
  return atoi( buf );
}

int train_setSpeed( WINTP train, int speed){
	int ret=0;
	if( speed >= 0 && speed < TRAIN_MAX_SPEED ){
		
		if( train ){
			netw_add_int( train, TCP_SPEED );
			netw_add_int( train, speed );
			netw_send( train );
			netw_clear_buf(train);
			netw_clear_arg(train);
			ret++;
		}else{
			printf("func: %s %d  >train< is not a valid WINTP ? \n",__func__,__LINE__);
		}
	}else{
		printf("func: %s %d  >speed< is not a valid number ? 0 >= speed <= %d  \n",__func__,__LINE__, TRAIN_MAX_SPEED);
	}
	return ret;
}

int train_setRamp( WINTP train, int speed, int time_ms){

	int ret=0;
	if( speed >= 0 && speed < TRAIN_MAX_SPEED ){
		
		if( train ){
			netw_add_int( train, TCP_RAMP );
			netw_add_int( train, speed );
			netw_add_int( train, time_ms );
			netw_send( train );
			netw_clear_buf(train);
			netw_clear_arg(train);
			ret++;
    }else{
			printf("func: %s %d  >train< is not a valid WINTP ? \n",__func__,__LINE__);
		}
	}else{
		printf("func: %s %d  >speed< is not a valid number ? 0 >= speed <= %d  \n",__func__,__LINE__, TRAIN_MAX_SPEED);
	}
	return ret;

}
int train_setDirection( WINTP train, int dr){
	int ret=0;
	if( dr > -2 && dr < 2 ){
		
		if( train ){
			netw_clear_arg(train);
			netw_add_int( train, TRAIN_SET_DIR );
			netw_add_int( train, dr );
			netw_send( train );
			netw_clear_buf(train);
			netw_clear_arg(train);
			ret++;
		}else{
			printf("func: %s %d  >train< is not a valid WINTP ? \n",__func__,__LINE__);
		}
	}else{
		printf("func: %s %d  >speed< is not a valid number ? 0 >= speed <= %d  \n",__func__,__LINE__, TRAIN_MAX_SPEED);
	}
	return ret;
}

int train_reportSpeed( WINTP train ){
		if( train ){
			netw_clear_arg(train);
			netw_add_int( train, REPORT_SPEED );
			netw_send( train );
			netw_clear_buf(train);
			netw_clear_arg(train);

      return catch_response(train);

    }else{
			printf("func: %s %d  >train< is not a valid WINTP ? \n",__func__,__LINE__);
		}
    return -1;
}

int mainboard_setWeiche( WINTP mainboard, int weiche_id, int val ){
	int ret =0;

	if( mainboard ){
		netw_add_int( mainboard, WEICHE );
		netw_add_int( mainboard, weiche_id );
		netw_add_int( mainboard, val );
		netw_send( mainboard );
		netw_clear_buf( mainboard );
		netw_clear_arg( mainboard );
		ret++;
	}
	else{
		printf("func: %s %d  >mainboard< is not a valid WINTP ? \n",__func__,__LINE__);
	}
	return ret;
}

int mainboard_setLed( WINTP mainboard, int led_id, int val ){
	int ret=0;
	if( mainboard ){
		netw_add_int( mainboard, LED_SET );
		netw_add_int( mainboard, led_id );
		netw_add_int( mainboard, val );
		netw_send( mainboard );
		netw_clear_buf( mainboard );
		netw_clear_arg( mainboard );
		ret++;
	}
	else{
		printf("func: %s %d  >mainboard< is not a valid WINTP ? \n",__func__,__LINE__);
	}
	return ret;
}
int mainboard_writeLed( WINTP mainboard ){
	int ret=0;
	if( mainboard ){
		netw_add_int( mainboard, LED_WRITE );
		netw_send( mainboard );
		netw_clear_buf( mainboard );
		netw_clear_arg( mainboard );
		ret++;
	}
	else{
		printf("func: %s %d  >mainboard< is not a valid WINTP ? \n",__func__,__LINE__);
	}
	return ret;
}
