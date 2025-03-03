#include "dummy.h"
#include <stdio.h>
#include <pthread.h>
#include "../src_winterface/winterface-config.h"


int main(int argc, char** argv){
    // Now server is ready to listen and verification 
	pthread_t t_train_b, t_train_y, t_mb; 
	int p_y = TRAIN_PORT_Y;
	int p_b = TRAIN_PORT_B;
	int p_mb = MB_PORT;

    pthread_create(&t_train_b , NULL,	thread_server , (void*)&p_y  ); 
    pthread_create(&t_train_y , NULL,	thread_server , (void*)&p_b	 ); 
    pthread_create(&t_mb , NULL,		thread_server , (void*)&p_mb ); 

    pthread_join(t_train_b, NULL); 
    pthread_join(t_train_y, NULL); 
    pthread_join(t_mb, NULL); 

    printf("After Thread\n"); 
	return 0;
}



