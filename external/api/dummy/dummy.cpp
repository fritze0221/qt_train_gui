#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include "dummy.h"
#include "../src_winterface/winterface.h"

#define MAX 1000
#define SA struct sockaddr 

extern BUF net_buf;
extern int new_argc;
extern char **netw_argv;

void *thread_server(void* p){
    int PORT = *(int*)p;
	char msg[256];
  int speed = 0;
	if( PORT == TRAIN_PORT_Y){
		sprintf(msg,"TRAIN_Y");
	}
	if( PORT == TRAIN_PORT_B){
		sprintf(msg,"TRAIN_B");
	}
	if( PORT == MB_PORT ){
		sprintf(msg,"MAINBOARD");
	}
	int sockfd, connfd; 
    socklen_t len;
    struct sockaddr_in servaddr, cli; 

    CMD cmd_arr[CMD_ARR_ENTRYS];
	cmd_arr_init( cmd_arr);   
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("%s socket creation failed...\n",msg); 
        exit(0); 
    } 
    else
        printf("%s Socket successfully created..\n",msg); 
    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
			// Binding newly created socket to given IP and verification 
			if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
				printf("%s socket bind failed...\n",msg); 
				exit(0); 
			} 
			else
				printf("%s Socket successfully binded..\n",msg); 

	while( 1 ){
			if ((listen(sockfd, 5)) != 0) { 
				printf("%s Listen failed...\n",msg); 
				exit(0); 
			} 
			len = sizeof(cli); 
		   
			// Accept the data packet from client and verification 
			connfd = accept(sockfd, (SA*)&cli, &len); 
			if (connfd < 0) { 
				printf("%s server accept failed...\n",msg); 
				exit(0); 
			} 
		   
			WINT netw;
			netw_init( &netw, connfd );
			while( netw_recv( &netw ) ){
				printf("%s\t",msg);
        int id=	netw_handle_message( &netw, cmd_arr );
        if( id== TCP_SPEED ){
          speed = 1; 
        }
        if( id== REPORT_SPEED ){
          printf("Now i should response with a speed...\n");  
          char buf[16];
          sprintf(buf,"%d",speed);
          write(connfd,buf,16);
        }
 			}
		//	close(connfd); 
	}
	close(sockfd);
}

